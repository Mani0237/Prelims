#ifndef GELIMINATION_H
#define GELIMINATION_H

#include <cmath> 
#include <iostream>
using namespace std; 

const int MAX_SIZE = 200 ;

void gauss_elimination(double A[][MAX_SIZE], double B[], double x[], int n ){
     //STEP 1: Taking the inputs (done in main file)
     //STEP 2: Pivoting the elements
     for(int k=0; k < n-1; k++){
        int max = k ;
        for(int i = k + 1; i<n; i++){
            if(abs(A[i][k])>abs(A[max][k]))
              max = i; 
        } 
        
        // Swap rows if required
        if(max!=k){
            for(int j = k; j < n; j++){
                swap(A[k][j], A[max][j]);
            }
            swap(B[k], B[max]);
        }

        // Eliminate 
        for(int i = k + 1; i < n; i++){
            double factor = A[i][k] / A[k][k];
            for(int j = k; j < n; j++){
                A[i][j] = A[i][j] -factor * A[k][j];
            }
            B[i] = B[i] - factor * B[k];
        }        
     }

    // Step 3: Back substitution
    for(int i = n - 1; i >= 0; i--){
        double sum = 0;
        for(int j = i + 1; j < n; j++){
            sum += A[i][j] * x[j];
        }
        x[i] = (B[i] - sum) / A[i][i];
    }
     
}

#endif