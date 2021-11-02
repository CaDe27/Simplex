#include "fraction.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <utility>

using namespace std;

string eraseSpaces(string &str){
    string resp = "";
    string :: iterator it;
    for(it = str.begin(); it != str.end(); ++it){
        if( *it != ' ') resp += *it;
    }
    return resp; 
}

//given a fraction number format  "number/number" return the corresponding fraction
fraction readFraction(string &s, int indx1, int indx2){
    int slashIndx = s.find('/', indx1), numerator, denominator;
    if(slashIndx == -1 || slashIndx > indx2){
        numerator = stringToInt(s, indx1, indx2);
        denominator = 1;
    }
    else{
        numerator = stringToInt(s, indx1, slashIndx - 1), 
        denominator = stringToInt(s, slashIndx + 1, indx2);
    }
    return fraction(numerator, denominator);
}

void readVariableNames(bool positiveVariables, unordered_map<string, int> &variablesMap, int &nextVariable){
    string line, variableName;
    getline(cin, line);
    line = eraseSpaces(line);
    int itStartVariable= line.find(':') + 1, itEndVariable;
    while(itStartVariable < line.size() ){
        //we adjust the iterators to the respective indexes
        itEndVariable = line.find(',',itStartVariable + 1);
        if(itEndVariable == -1)
            itEndVariable = line.size();

        //we extract the variable name
        variableName = line.substr(itStartVariable, itEndVariable - itStartVariable);       

        //we map the variable to an index (index of the simplex column) or to the
        //first index if it's a free variable (we represent it as the substraction of two positive
        // variables) 
        variablesMap[variableName] = nextVariable;
        if(positiveVariables)
            nextVariable = nextVariable + 1;
        else
            nextVariable = nextVariable + 2;

        itStartVariable = itEndVariable + 1;
    }
    
}


template <class dataType>
void readNormalVariable(string &line, int startIndx, int endIndx, dataType &coef, string &variable){
    //we look for the first character that is a letter
    int i = startIndx;
    while(i < endIndx && !isLetter(line[i]))
        ++i;
    //case when the coefficient is omitted because it is one
    coef = i > startIndx? readFraction(line, startIndx, i-1) : fraction(1);
    variable = line.substr(i, endIndx - i + 1);
}

template <class dataType>
void readLinearExp(string &line, 
                   vector<dataType> &coefficients,
                   unordered_map<string, int> &positiveVariables, 
                   unordered_map<string, int> &freeVariables){
    int itStartVariable = 0, itEndVariable;
    int signSymbol, absSymbol, maxSymbol, plusSymbol, minusSymbol;
    dataType coeff, sign;
    string variableName;
    
     
    //we add this in case the first term doesn't have an explicit '+' sign 
    if(line[itStartVariable] != '+' && line[itStartVariable] != '-')
        line = '+' + line;
    
    //we read every term
    while(itStartVariable < line.size()){
        sign = line[itStartVariable] == '+'? dataType(1) : dataType(-1);
        ++itStartVariable;

        //we adjust the iterators to the respective indexes
        absSymbol = line.size(); //myFind(line, '|', itStartVariable);
        maxSymbol = line.size(); //myFind(line, '(', itStartVariable);
        plusSymbol = myFind(line, '+', itStartVariable);
        minusSymbol = myFind(line, '-', itStartVariable);
        signSymbol = min(plusSymbol, minusSymbol);

        //normal variable reading
        if(signSymbol <= min(absSymbol, maxSymbol)){
            readNormalVariable(line, itStartVariable, signSymbol - 1, coeff, variableName);
            //check if its a positive variable
            int indx;
            if(positiveVariables.find(variableName) != positiveVariables.end()){
                indx = positiveVariables.find(variableName) -> second;
                coefficients[indx] = sign*coeff;
            }
            //or if its free variable
            else{
                indx = freeVariables.find(variableName) -> second;
                coefficients[indx] = sign*coeff;  
                coefficients[indx + 1] = dataType(-1)*sign*coeff;
            }
        }
        //absolute value
        else if(absSymbol < maxSymbol){
            throw "haven't implemented abs values reading";
        }
        //max
        else{
            throw "haven't implemented min/max values reading";
        }
        itStartVariable = signSymbol;
    }
}

//vector< vector <fraction> > 
template <class dataType> 
vector< vector<dataType> > read(unordered_map<string, int> &positiveVariables, 
          unordered_map<string, int> &freeVariables, bool &isMinProblem){
    
    //we read the positive variables 
    int nextVariable = 0;
    readVariableNames(true, positiveVariables, nextVariable);
    readVariableNames(false, freeVariables, nextVariable);
    
    string line;
    //read type of problem
    getline(cin, line);
    line = eraseSpaces(line);
    line = line.substr(line.find(':') + 1);
    dataType problemMultiplier = line == "min"? dataType(1) : dataType(-1);
    isMinProblem = line == "min";

    //read cost function 
    getline(cin, line);
    line = eraseSpaces(line);
    line = line.substr(line.find(':') + 1);
    vector<dataType> costFunction(nextVariable + 1);
    readLinearExp<dataType>(line, costFunction, positiveVariables, freeVariables);
    loop(i, 0, nextVariable + 1)
        costFunction[i] = costFunction[i]*problemMultiplier;

    //read restrictions
    getline(cin, line);
    
    vector< vector<dataType> > A; 
    getline(cin, line);
    int greaterSymbol, lessSymbol, equalSymbol, restrictionSymbol, indx = 0;
    dataType bi;

    while(line != "fin"){
        line = eraseSpaces(line);
        greaterSymbol = myFind(line, '>', 0);
        lessSymbol = myFind(line, '<', 0);
        equalSymbol = myFind(line, '=', 0);
        restrictionSymbol = min(greaterSymbol, min(lessSymbol, equalSymbol));

        A.push_back( vector<dataType> (nextVariable + 1) );
        bi = readFraction(line, equalSymbol + 1, line.size() - 1);
        line = line.substr(0, restrictionSymbol);
        readLinearExp<dataType> (line, A[indx], positiveVariables, freeVariables);
        A[indx][nextVariable] = bi;
        
        //if its an enaquality we add a new variable 
        if(restrictionSymbol != equalSymbol){
            //we resize all vectors and adjust the last column
            loop(i, 0, indx + 1){
                A[i].resize(nextVariable + 2);
                A[i][nextVariable + 1] = A[i][nextVariable];
                A[i][nextVariable] = dataType(0);
            }
            //  if its a <= we add a slack variable
            A[indx][nextVariable]= restrictionSymbol == greaterSymbol? dataType(-1) : dataType(1);
            ++nextVariable;
        }
        ++indx; 
        getline(cin, line);
    }
    costFunction.resize(nextVariable + 1);
    A.push_back(costFunction);

    return A; 
}
