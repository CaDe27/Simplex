#include <iostream>
#include <string.h>
#include "library.h"
using namespace std;

struct fraction{
    int64_t num, den;
    fraction(int64_t numerator, int64_t denominator){
        int64_t g = gcd(numerator, denominator);
        num = numerator/g; den = denominator/g;
    }
    fraction(int64_t numerator){ 
        num = numerator; 
        den = 1;
    }
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


