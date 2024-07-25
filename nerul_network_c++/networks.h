#include <iostream>
#include <stdio.h>
#include <vector>
#include "tools.h"
#include <math.h>

using namespace std;

typedef class Node{//节点
    public:
        float value;//输出值
        vector<float> weight;//往下一个点输出的权重
        vector<float> weight_delta;//权重修改值记录
        float  bias;//偏置值
        float bias_delta;
        float error;//传递来的误差
        int flag;//决定使用哪个激活函数,1为sigmoid,2为ReLU

        Node();
        void Ini();
        float activation();//激活函数
        float deri();//对激活函数求导
}Node;

typedef class Net{//神经网络
    public:
        vector<int> shape;//记录有几层每层有几个
        vector<vector<Node>> Nodes;//节点矩阵

        Net();
        Net(vector<int> shape,int flag);//第二个参数是决定使用哪个激活函数
        void show();//展示整个神经网络
        void reset();//重置所有节点的修改缓存
        vector<float> perdict(vector<float> input);//预测
        void FindPath(int layer,vector<int> buff,vector<vector<int>> &out);//找到该层节点到终点所有途径路径,第三个参数是储存结果的值
        void train(vector<vector<float>> x,vector<vector<float>> y,float rate,int time);//训练,参数为数据，标签，学习率，训练次数
        void Save(char name[]);
}Net;

Net Load(char name[]);//加载神经网络模型
vector<vector<float>> pooling(vector<vector<float>> vec,int step);//池化，参数为被池化对象，步长
vector<vector<float>> convolution(vector<vector<float>> vec,vector<vector<float>> stone,int step);//卷积，参数为对象，卷积核，步长；
