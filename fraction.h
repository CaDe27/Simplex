#include <iostream>
#include <string.h>
using namespace std;
#define loop(i, a, b) for(int i = a; i < b; ++i)

template <typename T>
T gcd(T a, T b){
    if(a == 0 || b == 0) return a+b;
    else return gcd(b, a%b);
}

struct fraction{
    int64_t num, den;
    fraction(int64_t numerator, int64_t denominator){
        int64_t g = gcd(numerator, denominator);
        num = numerator/g; den = denominator/g;
    }
    fraction(int64_t numerator){ num = numerator; den = 1;}
    fraction(){num = 0; den = 1;}

    fraction operator+(const fraction &other){return fraction(num*other.den + den*other.num, den*other.den).fixSign();}
    fraction operator-(const fraction &other){return fraction(num*other.den - den*other.num, den*other.den).fixSign();}
    fraction operator*(const fraction &other){return fraction(num*other.num, den*other.den).fixSign();}
    fraction operator/(const fraction &other){return fraction(num*other.den, den*other.num).fixSign();}
    fraction fixSign(){ 
        if(den < 0) {num *= -1, den *= -1;} 
        return *this;
    }

    bool operator < (const fraction &other){ return num*other.den < den*other.num;}
    bool operator > (const fraction &other){ return num*other.den > den*other.num;}
    bool operator <= (const fraction &other){ return num*other.den <= den*other.num;}
    bool operator == (const fraction &other){ return num == other.num && den == other.den;}
    string toString(){return den == 1? to_string(num) : to_string(num)+"/"+to_string(den);}
};

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
