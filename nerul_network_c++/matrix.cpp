#include "matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int x,int y){
    this->row_len = x;
    this->list_len = y;
    this->matrix =  vector<vector<float>>(row_len,vector<float>(list_len,0));
}

Matrix::Matrix(int x,int y,vector<float> vec){
    this->row_len = x;
    this->list_len = y;
    this->matrix =  vector<vector<float>>(row_len,vector<float>(list_len,0));
    int num = 0;
    if(vec.size() != x*y){
        cout<<"error"<<endl;
        return;
    }
    for(int i = 0;i < x;i++){
        for(int j = 0;j < y;j++){
            this->matrix[i][j] = vec[num];
            num++;
        }
    }
}

void Matrix::show(){
    for(int i = 0;i < this->row_len;i++){
        for(int j = 0;j < this->list_len;j++){
            printf("%f",this->matrix[i][j]);
            printf(" ");
        }
        printf("\n");
    }
}

float Matrix::value(int row,int list){
    return this->matrix[row + 1][list + 1];
}

void Matrix::add(Matrix m){
    int leni = this->row_len;
    int lenj = this->list_len;
    if(leni != m.row_len || lenj != m.list_len){
        cout<<"error"<<endl;
        return;
    }
    for(int i = 0;i < leni;i++){
        for(int j = 0;j < lenj;j++){
            this->matrix[i][j] += m.matrix[i][j];
        }
    }
}

void Matrix::sub(Matrix m){
    int leni = this->row_len;
    int lenj = this->list_len;
    if(leni != m.row_len || lenj != m.list_len){
        cout<<"error"<<endl;
        return;
    }
    for(int i = 0;i < leni;i++){
        for(int j = 0;j < lenj;j++){
            matrix[i][j] -= m.matrix[i][j];
        }
    }
}

void Matrix::dot(Matrix m){
    if(this->list_len != m.row_len){
        cout<<"error"<<endl;
        return;
    }
    vector<vector<float>> vec (this->row_len,vector<float>(m.list_len,0));
    for(int i = 0;i < this->row_len;i++){
        for(int j = 0;j < m.list_len;j++){
            float out = 0;
            for(int i2 = 0;i2 < this->list_len;i2++){
                    out += this->matrix[i][i2] * m.matrix[i2][j];
            }
            vec[i][j] = out;
        }
    }
    this->matrix = vec;
    this->list_len = m.list_len;
}

void Matrix::product(Matrix m){
    if(this->row_len != m.row_len){
        cout<<"error"<<endl;
        return;
    }
    for(int i = 0;i < this->row_len;i++){
       for(int j = 0;j < this->list_len;j++){
          this->matrix[i][j] *= m.matrix[i][0];
       }  
    }
}

float Matrix::sum(){
    float out = 0;
    for(auto i : this->matrix){
        for(float j : i){
            out += j;
        }
    }
    return out;
}

Matrix Matrix::list(int n){
   vector<float> vec (row_len,0);
    for(int i = 0;i < row_len;i++){
        vec[i] = matrix[i][n];
    }
    return Matrix(row_len,1,vec);
}

void Matrix::normalize(){
    float sum = 0;
    float mean = 0;
    float stand = 0;
    for(int i = 0;i < list_len;i++){
        for(int j = 0;j < row_len;j++){
          sum += matrix[j][i]; 
        }
        mean = sum/row_len;
        sum = 0;
        for(int j = 0;j < row_len;j++){
          float a = (matrix[j][i] - mean);
          sum += a*a; 
        }
        sum = sum/row_len;
        stand = sqrt(sum);
        if(stand != 0){
           for(int j = 0;j < row_len;j++){
             matrix[j][i] = (matrix[j][i] - mean) / stand;
            }
        }
    }
}