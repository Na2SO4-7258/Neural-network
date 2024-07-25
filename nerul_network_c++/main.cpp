#include <iostream>
#include <stdio.h>
#include <vector>
#include "matrix.h"
#include "math.h"
#include "networks.h"
#include <random>
#include <windows.h>
#include "sharp.h"

using namespace std;

//线性回归
typedef class Liner{
    public:
    int para;//参数个数
    Matrix theta;//参数矩阵

    Liner(int n){
        para = n;
        theta = Matrix(n,1);
    }
    Matrix perdict(Matrix x){//根据x值预测y值
        x.dot(theta);
        return x;
    }
    void learn(Matrix x,Matrix y,float a,int n){//训练模型，参数为步长和迭代次数
        int m = x.list_len;
        for(int i = 0;i < n;i++){
            for(int j = 0;j < theta.row_len;j++){
                Matrix y1 = y;
                Matrix x1 = x;
                y1.sub(perdict(x));
                y1.product(x.list(j));
                theta.matrix[j][0] += a/m*y1.sum();
            }
       }
    }
}Liner;

//逻辑回归
typedef class Logic{
    public:
    int para;//参数个数
    Matrix theta;//参数矩阵

    Logic(){
    }
    Logic(int n){
        para = n;
        theta = Matrix(n,1);
    }
    void Ini(int n){
        para = n;
        theta = Matrix(n,1);
    }
    Matrix perdict(Matrix x){//根据x值预测y值
        x.dot(theta);
        for(int i = 0;i < x.row_len;i++){
           x.matrix[i][0] = 1/(1 + exp(-1 * x.matrix[i][0]));
        }
        return x;
    }
    void learn(Matrix x,Matrix y,float a,int n){//训练模型，参数为步长和迭代次数
        int m = x.list_len;
        for(int i = 0;i < n;i++){
            Matrix y1 = y;
            y1.sub(perdict(x));
            cout<<"-";
            for(int j = 0;j < theta.row_len;j++){
                Matrix y2 = y1;
                y2.product(x.list(j));
                theta.matrix[j][0] += a/m*y2.sum();
            }
       }
       cout<<" "<<endl;
    }
}Logic;

//通过下棋获得训练数据
void get_data(){
    vector<vector<float>> x(0);
    vector<vector<float>> y(0);
    Sharp sharp(1);
    int color = 1;
    a:while(!sharp.win){
        char* name = (char*)"data//sharp2.txt";
        Net n = Load(name);
        int place = 0;
        sharp.show();
        while(1){
            if(color == 2){
                x.push_back(sharp.board);
                cout<<"::"<<n.perdict(sharp.board)[0]<<endl;
            }else{
                vector<float> vec(9);
                for(int i = 0;i < 9;i++){
                    if(sharp.board[i] == 0){
                        vec[i] = 0;
                    }else if(sharp.board[i] == 1){
                        vec[i] = 2;
                    }else{
                        vec[i] = 1;
                    }
                }
                 x.push_back(vec);
            }
            scanf("%d",&place);
            
            vector<float> vec = {(float)place};
            y.push_back(vec);

            if(place > 0 && place <= 9 && sharp.board[place - 1] == 0){
                break;
            }
            cout<<"error"<<endl;
        }
        sharp.board[place - 1] = color;
        color = (color == 1)?2:1;
        sharp.Update();
    }
    sharp.show();
    cout<<sharp.win<<"win"<<endl;
    cout<<"again? 1 or 0"<<endl;
    int num;
    scanf("%d",&num);
    if(num){
        sharp.Ini();
        cout<<"-------------------"<<endl;
        goto a;
    }else{
        savetxt(x,y);
    }
}

// vector<vector<float>> x1 = {{1,1},{1,2},{1,3},{2,1},{2,2},{2,3},{3,1},{3,2},{3,3}};
// vector<vector<float>> y1 = {{1},{0},{0},{0},{1},{0},{0},{0},{1}};
int main(){
    vector<vector<float>> x1;
    vector<vector<float>> y1;
    for(float i = 0;i < 100;i++){
        x1.push_back({i,i+1});
        y1.push_back({4*i + 2});
    }
    Net n({2,5,1},3);
    n.train(x1,y1,0.00015,3000);
    cout<<n.perdict({20,30})[0]<<endl;
    // n.Save("data//sharp2.txt"); 
    system("Pause");
}    