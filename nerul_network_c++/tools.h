#include <iostream>
#include <vector>


using namespace std;

int StrToInt(char ch[]);

void IntToStr(int num, char out[],bool nega = false);//第三个参数为强制添加负号

float StrToFloat(char ch[]);

void FloatToStr(float num,char* out);

void savetxt(vector<vector<float>> x,vector<vector<float>> y);

void loadtxt(vector<vector<float>> &x,vector<vector<float>> &y);