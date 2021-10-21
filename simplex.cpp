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

template <class dataType>
void simplex(vector<vector<dataType> > &A){
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
            cout<<"pivot row: "<<pivotRow<<endl;
            if(indexNewVariable == -1){
                cout<<"there is no row to use as pivot, the problem has no optimal value"<<endl;
                noOptimalExists = true;
            }
        }
        if(hasReachedOptimal || noOptimalExists) break;

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






int main(){
    vector< vector<fraction> >A;

    fraction a1[] = {fraction(-1), fraction(3), fraction(1), fraction (0), fraction(3) };
    fraction a2[] = {fraction(2), fraction(-1), fraction(0), fraction (1), fraction(4)};
    fraction a3[] = {fraction(-1), fraction(-1), fraction(0), fraction (0), fraction(0)};
    
    int size = sizeof(a1)/sizeof(a1[0]);
    A.push_back(vector<fraction> (a1, a1+size));
    A.push_back(vector<fraction> (a2, a2+size));
    A.push_back(vector<fraction> (a3, a3+size));

    simplex(A);
}