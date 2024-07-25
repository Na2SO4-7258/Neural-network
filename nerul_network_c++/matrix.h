#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>

using namespace std;

typedef class Matrix{
public:
    int row_len;
    int list_len;
    vector<vector<float>> matrix;

    Matrix();
    Matrix(int x,int y);//x卫行数，y为列数
    Matrix(int x,int y,vector<float> vec);//会按行按列输入vec中的值
    void show();//打印矩阵
    float value(int x,int y);//返回第x行第y列的值
    void add(Matrix m);//加上矩阵m
    void sub(Matrix m);//减去矩阵m
    void dot(Matrix m);//叉乘矩阵m
    void product(Matrix m);//点乘矩阵m
    float sum();//所有元素和
    Matrix list(int n);//抽出第n行列并以数组返回
    void normalize();//归一化
}Matrix;