#include <iostream>
#include <vector>
#include "simplex.h"
using namespace std;

int main(){
    if(fopen("problem.txt", "r")) freopen("problem.txt", "r", stdin);
    unordered_map<string, int> positiveVariables;
    unordered_map<string, int > freeVariables;
    vector< vector< fraction > > A = read<fraction>(positiveVariables, freeVariables);
    
    twoPhaseSimplexMethod(A);
}






