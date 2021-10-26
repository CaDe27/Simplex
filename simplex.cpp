#include <iostream>
#include <vector>
#include "fraction.h"
using namespace std;

#define loop(i, a, b) for(int i = a; i < b; ++i)

//multiplies the row by the rescaling factor
template <class dataType>
void rescaleRow(vector<vector<dataType> > &A, int row, dataType rescale){
    int col = A[row].size();
    loop(j, 0, col)
        A[row][j] = A[row][j]*rescale;
}

//simulates an elementary operation to sum the rescaled pivotRow to the destination row
template <class dataType>
void sumMultipleOfRow(vector<vector<dataType> > &A, int pivotRow, int row, dataType rescale){
    int cols = A[0].size();
    loop(i, 0, cols)
        A[row][i] = A[row][i] - rescale*A[pivotRow][i];
}

/*
    it returns the first column (variable) that has a negative 
    relative cost
    if there is no such column, the method has reached an answer
    and we return -1 as the new pivot
*/
template<class dataType>
int newPivotColumn(vector<vector<dataType> > &A){
    int columns = A[0].size(), lastRow = A.size() - 1, 
        newPivot = -1, columnIterator = 0;
    dataType relativeCost;
    while(columnIterator < columns - 1 && newPivot == -1){
        relativeCost = A[lastRow][columnIterator];
        if(relativeCost < 0)
            newPivot = columnIterator;
        ++columnIterator;
    }
    return newPivot;
}

/*
    returns the row to be used as pivot for a certain column
    it takes the row that has the minimum bi/xi
    if there are multiple, it takes the first row that has the value 
*/
template<class dataType>
int newPivotRow(vector< vector<dataType> > &A, int column){
    int rows = A.size(), pivotRow = -1, lastColumn = A[0].size() - 1;
    dataType epsilon = -1;
    loop(rowIterator, 0, rows-1){
        if(A[rowIterator][column] > dataType(0) && (pivotRow == -1 || A[rowIterator][lastColumn]/A[rowIterator][column] < epsilon)){
            pivotRow = rowIterator;
            epsilon = A[rowIterator][lastColumn]/A[rowIterator][column]; 
        }
    }
    return pivotRow;
}

/*
    given a matrix representing the standard form of a Lineal Programming problem, 
    its solves the problem using the simplex method
    (it either reaches an optimal value or stops if there is no optimal)
*/
template <class dataType>
void simplex(vector<vector<dataType> > &A, vector<int> &canonical){
    //select the variable used as a pivot
    int rows = A.size(), columns = A[0].size();
    int indexNewVariable, pivotRow = -1;
    bool hasReachedOptimal = false, noOptimalExists = false;
    do{
        cout<<endl;
        cout<<endl;
        cout<<"State of the simplex matrix:"<<endl;
        print(A);
        //we select the variable that enters the basis
        indexNewVariable = newPivotColumn(A);
        cout<<"variable that enters the basis: "<<indexNewVariable<<endl;
        
        //if the row is -1, there is no negative relative cost
        // and the method has reached an optimal value
        if(indexNewVariable == -1){
            hasReachedOptimal = true;
            cout<<"we reached an optimal"<<endl;
        }
        //we select the row to use as pivot
        else{
            pivotRow = newPivotRow(A, indexNewVariable);
            //if the pivotRow == -1, all entries are negative or zero
            //and the problem has no optimal solution
            if(indexNewVariable == -1){
                cout<<"there is no row to use as pivot, the problem has no optimal value"<<endl;
                noOptimalExists = true;
            }
        }
        if(hasReachedOptimal || noOptimalExists) break;

        cout<<"pivot row"<<pivotRow<<"\n";
        //we store the new basic variable and the canonical vector
        //it represents
        canonical[pivotRow] = indexNewVariable;

        //rescale row for the pivot to be 1
        cout<<"Elementary operations for this step:"<<endl;
        cout<<"\tR"<<pivotRow<<" <- ("<<(dataType(1)/A[pivotRow][indexNewVariable]).toString()<<")R"<<pivotRow<<endl;
        rescaleRow(A, pivotRow, dataType(1)/A[pivotRow][indexNewVariable]);

        //make all the other cells in the column 0
        loop(i, 0, rows){
            if(i != pivotRow){
                cout<<"\tR"<<i<<" <- R"<<i<<" - ("<<A[i][indexNewVariable].toString()<<")R"<<pivotRow<<endl;
                sumMultipleOfRow(A, pivotRow, i, A[i][indexNewVariable]);
            }
        }
    }while(!(hasReachedOptimal || noOptimalExists));
}

template<class dataType>
void validateData(vector< vector<dataType> > &A){
    int rows = A.size();
    if(rows == 0)
        throw "There are no restrictions in the problem";
    int columns = A[0].size();
    if(columns == 0)
        throw "There are no columns in matrix";
}

template<class dataType>
void transformToFirstPhaseMatrix(vector< vector<dataType> > &A, vector<int> &canonical){
    cout<<"Original standard problem matrix\n";
    print(A);

    int rows = A.size(), restrictions = rows - 1, costRow = rows - 1;
    //we add columns to have the canonical basis from start 
    //the number of columns we have is equal to the amount of restrictions
    int columns = A[0].size(), newColumns = columns + restrictions, 
        originalBiColumn = columns - 1, newBiColumn = newColumns - 1;
    
    //we resize the matrix to add the columns
    loop(i, 0, rows)
        A[i].resize(newColumns);

    //we reassign the restriction-rows' coefficients 
    loop(i, 0, costRow){
        //we reassing bi to the last column 
        A[i][newBiColumn] = A[i][originalBiColumn];
        //we fill the new variables coefficients 
        fill(A[i].begin() + originalBiColumn, A[i].begin() + newBiColumn, dataType(0));
        A[i][originalBiColumn + i] = 1; 
    }
    //we fill cost function row
    fill(A[costRow].begin(), A[costRow].begin() + columns - 1, dataType(0));
    fill(A[costRow].begin() + columns - 1, A[costRow].end() - 1, dataType(1));
    cout<<"For first phase problem we consider the cost function: SumOfTheNewVariables \n";
    cout<<"First phase problem matrix\n";
    print(A);

    cout<<"Before executing simplex, we need to make relative cost of basis variables zero\n";
    cout<<"This is accomplished by adding each restriction row to the cost row\n";

    //we fill the cost function of the first phase problem
    //for the first variables and the bi column, the cost is the minus sum of the column elements
    loop(j, 0, originalBiColumn){
        dataType sum = 0;
        loop(i, 0, restrictions)
            sum = sum + A[i][j];
        A[costRow][j] = sum*(-1);
    }  
    //for bi column
    dataType sum = 0;
    loop(i, 0, restrictions)
        sum = sum + A[i][newBiColumn];
    A[costRow][newBiColumn] = sum*(-1);

    //for bi and the new variables te cost is cero
    loop(j, originalBiColumn, newBiColumn)
        A[costRow][j] = 0;
    
    //we initialize the indexes of the basic variables
    //specifying what variable corresponds to which canonical vector
    loop(i, 0, restrictions)
        canonical[i] = originalBiColumn + i;
    
    cout<<"First phase matrix\n";
    print(A);
}

template<class dataType>
bool transformToSecondPhaseMatrix(vector< vector<dataType> > &A, vector<int> &canonical, vector<dataType> &costFunctionCoefficients){
    int rows = A.size(), columns = A[0].size() - (rows - 1), firstPhaseColumns = A[0].size();
    dataType optimalValueOfFirstPhase = A[rows - 1][firstPhaseColumns - 1];
    
    //if the first phase doesn't have 0 as the answer, then the solution space of the 
    //original problem is empty
    bool solutionSpaceIsEmpty = !(optimalValueOfFirstPhase == dataType(0));

    //we take bi to original bi column and resize the row
    loop(i, 0, rows){
        A[i][columns - 1] = A[i][firstPhaseColumns - 1];
        A[i].resize(columns);
    }

    //we put the original costFunction
    int costRow = rows - 1;
    A[costRow] = costFunctionCoefficients;

    cout<<"\nSecond phase matrix\n";
    print(A);
    cout<<"\nBefore executing simplex, we need to transform the relative cost of basic variables to zero\n";
    cout<<"Operations to do so:\n";
    if (!solutionSpaceIsEmpty){
        //we make the basis vectors relative costs equal to zero 
        loop(i, 0, rows - 1){
            cout<<"\tR"<<(rows-1)<<" <- R"<<(rows - 1)<<" - ("<<A[rows - 1][ canonical[i] ].toString()<<")R"<<i<<endl;
            sumMultipleOfRow(A, i, rows - 1, A[rows - 1][ canonical[i] ]);
            print(A);
        }
    }
    return solutionSpaceIsEmpty;
}

/*
    Parameters:
        A - simplex matrix in standard form

    Returns:
        vector containing the answer to the problem
        represented by A
*/
template <class dataType> 
void twoPhaseSimplexMethod(vector< vector<dataType> > &A){
    validateData(A);
    int rows = A.size(), restrictions = rows - 1, costRow = rows - 1, columns = A[0].size();
    vector<dataType> costFunctionCoefficients = A[costRow];
    vector<int> canonical(restrictions);
    transformToFirstPhaseMatrix(A, canonical);
    cout<<"\n ========== We start the first phase problem\n";
    simplex(A, canonical);
    //we construct the second phase matrix 
    //we do it anyway since A is modified with the method
    bool solutionSpaceEmpty = transformToSecondPhaseMatrix(A, canonical, costFunctionCoefficients);
    cout<<"\n ========== We start the second phase problem\n";
    if(solutionSpaceEmpty)
        cout<<"The solution space is empty and we cannot proceed\n";
    else{
        simplex(A, canonical);
        cout<<endl;
        //we fill the original variables values
        vector<dataType> originalV(columns - 1, dataType(0));
        loop(i, 0, rows - 1)
            originalV[ canonical[i] ] = A[i][columns - 1];
        
        cout<<"State of the standard problem variables: "<<endl;
        loop(i, 0, columns - 1)
            cout<<originalV[i].toString()<<" ";
        cout<<endl;
        cout<<"optimal value is:"<<(dataType(-1)*A[rows-1][columns - 1]).toString()<<"\n";
    }
}
    
/*
void readFraction(fraction &a){

}

void readRow(vector< vector<fraction> > &A, int row, int variables){

}

void readProblem(vector< vector<fraction> > &A){
    int variables, restrictions;
    cin>>variables>>restrictions;
    A.resize(restrictions + 1);
    loop(i, 0, restrictions + 1)
        A[i].resize(variables + 1);
    
    readRow(A, restrictions);
    loop(i, 0, restrictions)
        readRow(A, i);

}
*/

int main(){
    vector< vector<fraction> >A;
    /*this represents the problem:
        min z = x1 + 2x2 
        sa. 
            3x1 + 4x2 <= 20
            2x1 - x2 >= 2
        which has min at (1,0) with z = 1
    */
    fraction a1[] = {fraction(3), fraction(4), fraction(1), fraction (0), fraction(20) };
    fraction a2[] = {fraction(2), fraction(-1), fraction(0), fraction (-1), fraction(2)};
    fraction a3[] = {fraction(1), fraction(2), fraction(0), fraction (0), fraction(0)};
    
    int size = sizeof(a1)/sizeof(a1[0]);
    A.push_back(vector<fraction> (a1, a1+size));
    A.push_back(vector<fraction> (a2, a2+size));
    A.push_back(vector<fraction> (a3, a3+size));
    twoPhaseSimplexMethod(A);
    //simplex(A);
}