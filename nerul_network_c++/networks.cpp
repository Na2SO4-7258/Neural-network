#include "networks.h"
#include <random>
#include <stdlib.h>
#include <time.h>
#include <cstring>

Node::Node(){
    this->value = 0;
    this->weight = vector<float>(0);
    this->weight_delta = vector<float>(0);
    this->bias = 0;
    this->bias_delta = 0;
    this->error = 0;
    Ini();
}

void Node::Ini(){
    for(auto &i:weight_delta){
        i = 0;
    }
    value = 0;
    bias_delta = 0;
    error = 0;
}

float Node::activation(){
    float a;
    float b;
    switch(flag){
        case 1:
            return 1/(1 + exp(-value));
        case 2:
            a = exp(value);
            b = exp(-value);
            return (a - b)/(a + b);
        case 3:
            return max(float(0.1 * value),value); 
    }
    return 0;
}

float Node::deri(){
    switch(flag){
        case 1:
            return value*(1-value);
        case 2:
            return 1 - value*value;
        case 3:
            return (value >= 0)?1:0.1;
    }
    return 0;
}

Net::Net(){}

Net::Net(vector<int> shape,int flag){
    Nodes = {};
    this->shape = shape;
    int len_lay = shape.size();
    for(int i = 0;i < len_lay;i++){
        vector<Node> vec(shape[i]);
        Nodes.push_back(vec);
    }

    mt19937 rd;
    rd.seed(random_device()());
    uniform_real_distribution<double> dis(-1,1);
    for(int i = 0;i < len_lay;i++){
        for(int j = 0;j < shape[i];j++){
            Nodes[i][j].flag = flag;
            Nodes[i][j].bias = dis(rd);
            Nodes[i][j].bias_delta = 0;
            if(i != len_lay - 1){
                for(int k = 0;k < shape[i + 1];k++){
                    Nodes[i][j].weight.push_back(dis(rd));
                    Nodes[i][j].weight_delta.push_back(0);
                }
            }
        }
    }

}

void Net::show(){
    for(auto i : Nodes){
        for(auto j : i){
            cout<<j.bias<<"    ";
            for(auto k : j.weight){
                cout<<k<<"    ";
            }
            cout<<"  "<<endl;
        }
    }
}

void Net::reset(){
    int len = shape.size();
    for(int i = 0;i < len;i++){
        for(int j = 0;j < shape[i];j++){
            Nodes[i][j].Ini();
        }
    }
}

vector<float> Net::perdict(vector<float> input){
    if(input.size() != shape[0]){
        cout<<"loss input"<<endl;
        return {};
    }
    for(int i = 0;i < shape[0];i++){//读入数值
        Nodes[0][i].value = input[i];
    }
    int len = shape.size();
    for(int i = 1;i < len;i++){
        for(int j = 0;j < shape[i];j++){
            float sum = 0;
            for(int k = 0;k < shape[i - 1];k++){
                sum += Nodes[i - 1][k].value * Nodes[i-1][k].weight[j];//计算上一个节点的输出
            }
            sum -= Nodes[i][j].bias;
            Nodes[i][j].value = sum; 
            Nodes[i][j].value = Nodes[i][j].activation(); 
        }
    }
    vector<float> out(0);
    for(int i = 0;i < shape[len - 1];i++){
        out.push_back(Nodes[len-1][i].value);
    }
    return out;
}

void Net::FindPath(int layer,vector<int> buff,vector<vector<int>> &out){
    if(layer == shape.size() - 1){
        out.push_back(buff);
    }else{
        int num = 0;
        for(auto i : Nodes[layer+1]){
            vector<int> copy = buff;
            copy.push_back(num);
            FindPath(layer + 1,copy,out);
            num++;
        }
    }
}

void Net::train(vector<vector<float>> x,vector<vector<float>> y,float rate,int time){
    vector<vector<vector<int>>> path;
    for(int i = 0;i < shape.size() - 1;i++){
        vector<vector<int>> vec;
        FindPath(i,{},vec);
        path.push_back(vec);
    }
    path.push_back({});
    for(int num = 0;num < time;num++){
        printf("\n");
        cout<<num+1<<"times train: "<<endl;
        reset();
        int num_len = x.size();//训练数据几组
        float max_error = 0;
        for(int num1 = 0;num1 < num_len;num1++){
            auto data = x[num1];
            //正向传播
            for(int i = 0;i < shape[0];i++){//读入数值
                Nodes[0][i].value = data[i];
            }
            int len = shape.size();
            for(int i = 1;i < len;i++){
                for(int j = 0;j < shape[i];j++){
                    float sum = 0;
                    for(int k = 0;k < shape[i - 1];k++){//遍历上一层所有节点
                        sum += Nodes[i - 1][k].value * Nodes[i-1][k].weight[j];//计算上一个节点的输出
                    }
                    sum -= Nodes[i][j].bias;
                    Nodes[i][j].value = sum;
                    Nodes[i][j].value = Nodes[i][j].activation(); 
                }
            }

            //误差的逆向传播
            int lay = shape.size() - 1;//最后一层的指标
            len = shape[lay];//输出的y有几个
            //计算最后一层的误差
            for(int i = 0;i < len;i++){
                Nodes[lay][i].error = y[num1][i] - Nodes[lay][i].value;
                max_error = max(abs(Nodes[lay][i].error),max_error);
            }
            for(int i = lay - 1;i>=0;i--){//从后往前遍历每层
                for(int j = 0;j < shape[i];j++){//遍历第i层所有节点
                    float sum = 0;
                    for(int k = 0;k < shape[i + 1];k++){//遍历下一层所有节点
                        sum += Nodes[i+1][k].error * Nodes[i][j].weight[k];//误差等于下一层所有节点误差乘以权重总和
                    } 
                    Nodes[i][j].error = sum;
                }
            }
            
            //更新最后一层偏置值
            int LastLayer = shape.size()-1;
            for(int i = 0;i < shape[LastLayer];i++){
                Nodes[LastLayer][i].bias_delta -= Nodes[LastLayer][i].error*Nodes[LastLayer][i].deri();
            }

            for(int i = 0;i < shape.size() - 1;i++){
                for(int j = 0;j < shape[i];j++){
                    if(i != 0){//更新偏置值
                        double sum = 0.f;
                        float part = -Nodes[i][j].error*Nodes[i][j].deri();
                        for(auto nodes : path[i]){
                            float part_copy = part;
                            for(int k = 0;k < nodes.size();k++){
                                part_copy *= Nodes[i+1+k][nodes[k]].deri();
                            }
                            sum += part_copy;
                        }
                        Nodes[i][j].bias_delta += sum;
                    }

                    for(int k = 0;k < Nodes[i][j].weight.size();k++){//更新权重
                        double sum = 0.f;
                        float part = Nodes[i][j].value*Nodes[i + 1][k].error*Nodes[i + 1][k].deri();
                        if(path[i + 1].size() == 0){
                            sum += part;
                        }
                        for(auto nodes : path[i + 1]){
                            float part_copy = part;
                            for(int m = 0;m < nodes.size();m++){
                                part_copy *= Nodes[i+2+m][nodes[m]].deri();
                            }
                            sum += part_copy;
                        }
                        Nodes[i][j].weight_delta[k] += sum;
                    }
                }
            }

            //逆向传播
            // for(int i = 0;i < shape[2];i++){//输出层偏置
            //     double bias_delta = -Nodes[2][i].error*Nodes[2][i].deri();
            //     Nodes[2][i].bias_delta += bias_delta;
            // }
            // for(int i = 0;i < shape[1];i++){//隐藏层到输出层w值
            //     for(int j = 0;j < shape[2];j++){
            //         double sum = 0.f;
            //         float part = Nodes[2][j].error*Nodes[1][i].value*Nodes[2][j].deri();
            //         if(path[2].size() == 0){
            //             sum += part;
            //         }
            //         for(auto nodes : path[2]){
            //             float part_copy = part;
            //             for(int num : nodes){
            //                 part_copy *= Nodes[2][num].deri();
            //             }
            //             sum += part_copy;
            //         }
            //         Nodes[1][i].weight_delta[j] += sum;
            //     }
            // }
            // for(int i = 0;i < shape[1];i++){//隐藏层偏置
            //     double sum = 0;
            //     for(int j = 0;j < shape[2];j++){
            //         sum += -Nodes[2][i].error*Nodes[2][j].deri()*Nodes[1][i].deri();
            //     }
            //     Nodes[1][i].bias_delta += sum;
            // }
            // for(int i = 0;i < shape[0];i++){//输入层的w
            //     for(int j = 0;j < shape[1];j++){
            //          double sum = 0.f;
            //         float part = Nodes[1][j].error*Nodes[0][i].value*Nodes[1][j].deri();
            //         for(auto nodes : path[1]){
            //             float part_copy = part;
            //             for(int num : nodes){
            //                 part_copy *= Nodes[2][num].deri();
            //             }
            //             sum += part_copy;
            //         }
            //         Nodes[0][i].weight_delta[j] += sum;
            //     }
            // }
        }
        
        //修改权重与偏置值
        if(fabs(max_error) < 0.01){
            cout<<"success"<<endl;
            break;
        }

        int len = shape.size();
        for(int i = 0;i < len;i++){
            for(int j = 0;j < shape[i];j++){
                if(i != 0){
                    Nodes[i][j].bias += rate*Nodes[i][j].bias_delta/num_len;
                }
                if(i != len - 1){
                    for(int k = 0;k < shape[i+1];k++){//遍历权重
                        Nodes[i][j].weight[k] += rate*Nodes[i][j].weight_delta[k]/num_len;
                    }
                }
            }
        }
    }
}

void Net::Save(char name[]){
    FILE* f = fopen(name,"wb");
    char ch[20] = {0};

    IntToStr(Nodes[0][0].flag,ch);//存储使用哪种激活函数
    fwrite(ch,1,strlen(ch),f);
    fwrite(",\n",1,2,f);

    for(int nums:shape){//存储几层几个节点
        char c[10] = {0}; 
        IntToStr(nums,c);
        fwrite(c,1,strlen(c),f);
        fwrite(",",1,1,f);
    }

    fwrite("\n",1,1,f);
    for(auto i : Nodes){//存储每个节点的偏置和权重
        for(auto j : i){
            FloatToStr(j.bias,ch);
            fwrite(ch,1,strlen(ch),f);
            fwrite(",",1,1,f);
            for(float k:j.weight){
                FloatToStr(k,ch);
                fwrite(ch,1,strlen(ch),f);
                fwrite(",",1,1,f);
            }
            fwrite("\n",1,1,f);
        }
    }
    fclose(f);
}

Net Load(char name[]){
    int p = 0;//指针
    int acti = 0;//使用激活函数种类的标签
    vector<int> shape;//神经网络节点和层数
    FILE* f = fopen(name,"r");
    if(f == nullptr){
        cout<<"unexit"<<endl;
        return Net({{1}},1);
    }
    char ch[500] = {0};//接收数据缓存区
    char data[500] = {0};//解析数据缓存区
    fgets(ch,99,f);

    //解析激活函数标签
    while(ch[p] != ','){
        data[p]= ch[p];
        p++;
    }
    acti = StrToInt(data);
    
    //解析网络形状
    fgets(ch,499,f);
    p = 0;
    while(ch[p] != '\n'){
        int p1 = 0;
        char da[10] = {0};
        while(ch[p] != ','){
            da[p1] = ch[p];
            p1++;
            p++;
        }
        shape.push_back(StrToInt(da));
        p++;
    }

    //解析每个节点的权值和偏置
    Net net(shape,acti);
    for(auto &i : net.Nodes){
        for(auto &j: i){
            char da[10] = {0};
            fgets(ch,499,f);
            p = 0;
            while(ch[p] != ','){
                da[p] = ch[p];
                p++;
            }
            j.bias = StrToFloat(da);
            p++;
            for(auto &k:j.weight){
                char d[10] = {0};
                int pd = 0;
                while(ch[p] != ','){
                    d[pd] = ch[p];
                    p++;
                    pd++;
                }
                p++;
                k = StrToFloat(d);
            }
        }
    }
    return net;
}

vector<vector<float>> pooling(vector<vector<float>> vec,int step){
    vector<vector<float>> out;
    int isize = vec.size();
    int jsize = vec[0].size();
    for(int i = 0;i <= isize - 2;i += step){
        vector<float> part;
        for(int j = 0;j <= jsize-2;j += step){
            float max_num = 0;
            for(int i1 = i;i1 <= i+1;i1++){
                for(int j1 = j;j1 <= j+1;j1++){
                    max_num = max(max_num,vec[i1][j1]);
                }
            }
            part.push_back(max_num);
        }
        out.push_back(part);
    }
    return out;
}

vector<vector<float>> convolution(vector<vector<float>> vec,vector<vector<float>> stone,int step){
    vector<vector<float>> out;
    int stone_size = stone.size();
    int isize = vec.size() - stone_size;
    int jsize = vec[0].size() - stone_size;
    for(int i = 0;i <= isize;i += step){
        vector<float> part;
        for(int j = 0;j <= jsize;j += step){
            float sum = 0;
            for(int i1 = i;i1 < i+stone_size;i1++){
                for(int j1 = j;j1 < j+stone_size;j1++){
                    sum += vec[i1][j1]*stone[i1-i][j1-j];
                }
            }
            part.push_back(sum);
        }
        out.push_back(part);
    }
    return out;
}