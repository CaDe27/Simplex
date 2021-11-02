#include <iostream>
#include <vector>
#include "simplex.h"
using namespace std;

void displayVariableValues(vector< vector< fraction > > &A, 
                           vector<int> &canonical, 
                           unordered_map<string, int> positiveVariables,
                           unordered_map<string, int> freeVariables)
{
    int lastColumn = A[0].size() - 1;
    vector<fraction> indexValues(lastColumn + 1);
    for(int i = 0; i < canonical.size(); ++i){
        indexValues[ canonical[i] ] = A[i][lastColumn]; 
    }

    unordered_map<string, int> :: iterator it;
    string variableName;
    int indxOfVariable;
    fraction value;
    for(it = positiveVariables.begin(); it != positiveVariables.end(); ++it){
        variableName = it -> first;
        indxOfVariable = it -> second;
        value = indexValues[indxOfVariable];
        cout<<"\t"<<variableName<<" = "<< value.toString()<<"\n";
    }

    for(it = freeVariables.begin(); it != freeVariables.end(); ++it){
        variableName = it -> first;
        indxOfVariable = it -> second;
        value = indexValues[indxOfVariable] - indexValues[indxOfVariable + 1];
        cout<<"\t"<<variableName<<" = "<< value.toString()<<"\n";
    }

}

int main(){
    if(fopen("problem.txt", "r")) freopen("problem.txt", "r", stdin);
    unordered_map<string, int> positiveVariables;
    unordered_map<string, int > freeVariables;
    bool isMinProblem;

    vector< vector< fraction > > A = read<fraction>(positiveVariables, freeVariables, isMinProblem);
    vector<int> canonical;
    bool reachedOptimal = twoPhaseSimplexMethod(A, canonical);
    if(reachedOptimal){
        fraction optimalValue;
        cout<<endl;
        int lastRow = A.size() - 1, lastColumn = A[0].size() - 1;
        optimalValue = isMinProblem? fraction(-1)*A[lastRow][lastColumn] : A[lastRow][lastColumn];
    
        cout<<"optimal value is:"<<optimalValue.toString()<<"\n";
        cout<<"final state of variables"<<endl;
        displayVariableValues(A, canonical, positiveVariables, freeVariables);
    }
    else{
        cout<<"problem didn't reach optimal value"<<endl;
        print(A);
    }
}