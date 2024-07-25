#include "tools.h"
#include <cstring>
#include <math.h>

int StrToInt(char ch[]) {
    bool flag = 0;//是否为负数
    int out = 0;
    int pt = 0;
    if(ch[0] == '-'){
        flag = 1;
        pt++;
    }
    while (ch[pt]) {
        out += ch[pt] - '0';
        out *= 10;
        pt++;
    }
    out /= 10;
    if(flag){
        out = -out;
    }
    return out;
}

void IntToStr(int num, char out[],bool nega) {
    bool flag = 0;//输入是否为负数
    if(num < 0){
        num = -num;
        flag = true;
    }else{
        if(num == 0){
            if(nega){
                out[0] = '-';
                out[1] = '0';
            }else{
                out[0] = '0';
            }
            return;
        }
    }
    string str = "";
    while (num > 0) {
        int n = num % 10;
        num /= 10;
        str += (n + 0x30);
    }
    int len = str.size();
    int p = 0;
    if(flag || nega){
        p = 1;
        out[0] = '-';
    }
    for (int i = len - 1; i >= 0; i--) {
        out[p] = str[i];
        p++;
    }
}

float StrToFloat(char num[]){
    bool flag = 0;//是否为负数
    float out = 0;
    char inti_char[10] = {0};
    int i = 0;
    int j = 0;
    if(num[0] == '-'){
        flag = 1;
        i++;
    }
    while(num[i] != '.'){
        inti_char[j] = num[i];
        i++;
        j++;
    }
    i++;
    out += StrToInt(inti_char);//计算整数部分
    char f_part[20] = {0};
    int p = 0;
    while(num[i]){
        f_part[p] = num[i];
        i++;
        p++;
    }
    float f = StrToInt(f_part);
    for(int i = 0;i < p;i++){
        f /= 10;
    }
    out += f;
    if(flag){
        out = -out;
    }
    return out;
}

void FloatToStr(float num,char* out){
    bool nega = num < 0;
    char i[20] = {0};
    char f[20] = {0};
    int int_part = num;
    IntToStr(int_part,i,nega);
    int p = 0;
    while(i[p]){
        p++;
    }
    i[p] = '.';
    num -= int_part;
    num = fabs(num);
    num *= 100000;
    IntToStr(num,f);
    strcat(i,f);
    p = 0;
    while(i[p]){
        out[p] = i[p];
        p++;
    }
}

void savetxt(vector<vector<float>> x,vector<vector<float>> y){
    FILE* f = fopen("data//d.txt","r");
    char buffer[1024 * 1024];
    fread(buffer,1,1024*1024,f);
    fclose(f);
    f = fopen("data//d.txt","w");
    fwrite(buffer,1,strlen(buffer),f);
    int len = x.size();
    for(int i = 0;i < len;i++){
        for(auto j:x[i]){
            char ch[10] = {0};
            FloatToStr(j,ch);
            fwrite(ch,1,strlen(ch),f);
            fwrite(",",1,1,f);
        }
        fwrite(".",1,1,f);
        for(auto j:y[i]){
            char ch[10] = {0};
            FloatToStr(j,ch);
            fwrite(ch,1,strlen(ch),f);
            fwrite(",",1,1,f);
        }
        fwrite("\n",1,1,f);
    }

}

void loadtxt(vector<vector<float>> &x,vector<vector<float>> &y){
    FILE* f = fopen("data//d.txt","r");
    for(int i = 0;i <130;i++){
        char ch[500] = {0};
        fgets(ch,499,f);
        int p = 0;
        vector<float> vec;
        for(int j = 0;j < 9;j++){
            char ch1[10] = {0};
            int p1 = 0;
            while (ch[p] != ','){
                ch1[p1] = ch[p];
                p++;
                p1++;
            }
            p++;
            vec.push_back(StrToFloat(ch1));
        }
        x.push_back(vec);
        p++;
        int p1 = 0;
        char ch1[10] = {0};
        while(ch[p1] != ','){
            ch1[p1] = ch[p];
            p++;
            p1++;
        }
        y.push_back({StrToFloat(ch1)});
    }
}