#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <random>
#include <chrono>

using namespace std;

pair<double, double> Calculate_area(double inner_radius, double outer_radius, int num_samples){
    random_device rd;
    mt19937 gen(rd());
    int count = 0;
    
    auto start = chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(64) reduction(+:count)
    for(int i = 0; i<num_samples; i++ ){
        double x = uniform_real_distribution<double>(-2*outer_radius, 2*outer_radius)(gen);
        double y = uniform_real_distribution<double>(-2*outer_radius, 2*outer_radius)(gen);
        double z = uniform_real_distribution<double>(-inner_radius, inner_radius)(gen);
        double distance = pow(outer_radius - sqrt(x*x + y*y), 2) + z*z;

        if(distance < inner_radius*inner_radius ){
            count++;
        }
    }
    
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();
    
    double Area = (32*inner_radius)*(outer_radius*outer_radius)*(double(count)/num_samples);
    
    return make_pair(Area, duration);
}

int main(){
    double r = 2.0;
    double R = 5.0;
    double A_exact = M_PI*r*r*(2*M_PI*R);    
    int n[] = {1000000, 2000000, 4000000, 8000000, 16000000};
    int N;
    int n_max = sizeof(n)/sizeof(n[0]);

    cout << "-------------------------------------------------------------------" << endl;
    cout << left << setw(12) << "N" 
         << left << setw(20) << "Calculated Area"
         << left << setw(20) << "Error"
         << left << setw(20) << "Duration (s)" << endl;  
    cout << "-------------------------------------------------------------------" << endl;     
    
    for(int i = 0; i<n_max; i++){
        N = n[i];
        pair<double, double> result = Calculate_area(r, R, N);
        double Area = result.first;
        double duration = result.second;
        cout << left << setw(12) << N 
             << left << setw(20) << Area
             << left << setw(20) << abs(A_exact - Area)
             << left << setw(20) << duration << endl;        

    }
    
    cout << "-------------------------------------------------------------------" << endl;

    return 0;
}
