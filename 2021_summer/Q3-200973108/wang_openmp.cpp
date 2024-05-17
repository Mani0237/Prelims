#include <iostream>
#include <chrono>
#include <iomanip>
#include <omp.h>
#define n 1000000000

using namespace std;
unsigned char s[n];

int main() {
    int i;
    for(i=0; i<n; i++) {
        s[i] = i*i % 256;
    }

    int num[256] = {0};
    #pragma omp parallel for
    for (int i=0; i<n; i++) {
        #pragma omp atomic
        num[s[i]]++;
    }

    for (int i=0; i<256; i++) { 
        printf("The char %d freq is %d\n", i, num[i]);
    }
}
