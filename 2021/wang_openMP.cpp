#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;

#define  n 1000000000
unsigned char s[n];

int main(){

    int i;
    for(i = 0; i < n; i++){
        s[i] = (i*i) % 256;
    }

    int T[9] = {1, 2, 4, 6, 8, 10, 12, 14, 16};
    int thread;

    cout << "------------------------------------" << endl;
    cout << left << setw(10) << "Threads" << left << setw(15) << "Time taken" << endl;
    cout << "------------------------------------"<< endl;
    for(int j = 0; j < 9; j++){ 
        
        thread = T[j];
        int num[256] = {0};

        auto start = chrono::high_resolution_clock::now();
        
        #pragma omp parallel for num_threads(thread) reduction(+:num[:256])
        for(i = 0; i < n; i++){
            num[s[i]]++;
        }

        auto end = chrono::high_resolution_clock::now();
        double duration = chrono::duration<double>(end - start).count();

        ofstream myfile;
        myfile.open("primes.txt");

        for(i = 0; i < 256; i++ ){
            myfile << "The character " << i << " frequency is " << num[i] << endl;
        }

        cout << left << setw(10) << thread << left << setw(15) << duration << endl;
    }

    return 0;
}