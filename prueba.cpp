#include <iostream>
#include <vector>

using namespace std;
#include <stdio.h>
 
void init(vector<vector<int> > &a, int rows, int cols){
    for(int i = 0; i < rows; ++i){
        for(int j = 0; j < cols; ++j){
            a[i][j] = i*cols + j;
        }
    }
}

void func(vector<int> &v){
    v[0] = 2;
    v[1] = 1;
}

int main(){
    vector<int> a;
    for(int i = 0; i < 10; ++i)
        a.push_back(i);
    func(a);
    for(int i =0; i < 10; ++i)
        cout<<a[i]<<" ";
    cout<<endl;
    return 0;
}