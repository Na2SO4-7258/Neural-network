#include <iostream>
#include <vector>

using namespace std;

typedef class Sharp{
    public:
        vector<float> board;
        int win;

        Sharp(int a);
        void Update();
        void show();
        void Ini();
}Sharp;