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
        double x = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
        double y = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
        double distance = sqrt(x*x + y*y);
        if(distance <= outer_radius && distance >= inner_radius){
            count++;
        }
    }
    
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();
    
    double Area = 4*(outer_radius*outer_radius)*(double(count)/num_samples);
    
    return make_pair(Area, duration);
}

int main(){
    double r = 10.0;
    double R = 20.0;
    double A_exact = 300.0*M_PI;    
    int n[] = {1000, 2000, 4000, 8000, 16000};
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
