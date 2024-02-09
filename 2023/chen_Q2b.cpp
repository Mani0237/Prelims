#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <random>
#include <chrono>

using namespace std;

pair<double, double> Calculate_area(double inner_radius, double outer_radius, int num_samples, int num_cores){
    int count = 0;
    
    auto start = chrono::high_resolution_clock::now();
    #pragma omp parallel num_threads(num_cores) reduction(+:count)
    {
        // Create a separate random number generator for each thread
        random_device rd;
        mt19937 gen(rd() + omp_get_thread_num());
        
        #pragma omp for
        for(int i = 0; i<num_samples; i++ ){
            double x = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
            double y = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
            double distance = sqrt(x*x + y*y);
            if(distance <= outer_radius && distance >= inner_radius){
                count++;
            }
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
    double A_exact = 300.0 * M_PI;    
    int threads[] = {1, 2, 4, 8, 16};
    int t;
    int N = 10000000;
    int t_max = sizeof(threads)/sizeof(threads[0]);

    cout << "-------------------------------------------------------------------" << endl;
    cout << left << setw(12) << "Threads" 
         << left << setw(20) << "Calculated Area"
         << left << setw(20) << "Error"
         << left << setw(20) << "Duration (s)" << endl;  
    cout << "-------------------------------------------------------------------" << endl;     
    
    for(int i = 0; i<t_max; i++){
        t = threads[i];
        pair<double, double> result = Calculate_area(r, R, N, t);
        double Area = result.first;
        double duration = result.second;
        cout << left << setw(12) << t 
             << left << setw(20) << Area
             << left << setw(20) << abs(A_exact - Area)
             << left << setw(20) << duration << endl;        

    }
    
    cout << "-------------------------------------------------------------------" << endl;

    return 0;
}
