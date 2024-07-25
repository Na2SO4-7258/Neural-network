#include "sharp.h"

Sharp::Sharp(int a){
    for(int i = 0;i <  9;i++){
        board.push_back(0);
        win = 0;
    }
}

void Sharp::Update(){
    vector<vector<int>> vec = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(auto i : vec){
        if(board[i[0]] != 0 && board[i[0]] == board[i[1]] && board[i[1]] == board[i[2]]){
            win = board[i[0]];
            return;
        }
    }
    bool flag = true;
    for(float i:board){
        if(i == 0){
            flag = false;
        }
    }
    if(flag){
        win = 3;
    }
}

void Sharp::show(){
    int n = 0;
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            cout<<board[n];
            n++;
        }
        printf("\n");
    }
}

void Sharp::Ini(){
    win = 0;
    for(int i = 0;i < 9;i++){
        board[i] = 0;
    }
}