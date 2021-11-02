#include <iostream>
#include <string.h>
#include <vector>

using namespace std;
#define loop(i, a, b) for(int i = a; i < b; ++i)

template <typename T>
T gcd(T a, T b){
    if(a == 0 || b == 0) return a+b;
    else return gcd(b, a%b);
}

template <class dataType>
void print(vector<vector<dataType> > &A){
    int rows = A.size();
    if(rows == 0){
        cout<<"Matrix has no rows\n";
        return;
    }
    int cols = A[0].size();
    cout<<"Current state of A\n";
    loop(i, 0, rows){
        loop(j, 0, cols)
            cout<<A[i][j].toString()<<"\t\t";
        cout<<"\n";
    }
    cout<<"\n";
} 

int myFind(string &line, char c, int indxStart){
    int resp = line.find(c, indxStart);
    return resp == -1? line.size() : resp;
}

bool isLetter(char c){
    return ('a' <= c && c<= 'z') || ('A' <= c && c <= 'Z');
}

//given a string that contains only numbers, return the respective int value 
int stringToInt(string &s, int indx1, int indx2){
    int resp = 0;
    loop(i, indx1, indx2 + 1)
        resp = 10* resp + (s[i] - '0');
    return resp;
}