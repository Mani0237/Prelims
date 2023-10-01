/*===========================================================================================
SUMMARY: 1. The code calculates the volume of a sphere of unit radius using Monte Carlo method.
         2. This includes two functions: Calculate_volume and main function.
         3. The main function calls Calculate_volume for a given radius(equal to 1 in our case)
            and differnet number of samples.
         4. The Results obtained are then printed on screen.   
=============================================================================================*/
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

// Defining the function to calculate volume
/*----------------------------------------------------------------------------
Parameters: 

        INPUTS:    
            radius - the radius of sphere whose volume is to be calculated
            num_sample - the number of samples used in MC algorithm

        OUTPUTS:
            volume - the volume calculated using MC algorithm
            time - time taken to calculate volume for a given number of samples
-----------------------------------------------------------------------------*/
pair<double, double> Calculate_volume(double radius, int num_sample){
    
    double volume = 0;
    double duration = 0;
    double count = 0;

    random_device rd;
    mt19937 gen(rd());

    auto start = chrono::high_resolution_clock::now();
    
    // Implementation of Monte Carlo Scheme
    for(int i = 0; i < num_sample; i++){

        double x = uniform_real_distribution<double>(-radius, radius)(gen);
        double y = uniform_real_distribution<double>(-radius, radius)(gen);
        double z = uniform_real_distribution<double>(-radius, radius)(gen);
        double distance = sqrt(x*x + y*y + z*z);

        if(distance <= radius){
            count++; 
        }

    }
    auto end = chrono::high_resolution_clock::now();
    duration = chrono::duration<double>(end - start).count();

    volume = pow(2*radius, 3)*count/(double)(num_sample); 

    return make_pair(volume, duration);
}


// Defining the main function
int main(){

    double r = 1.0;
    double V_exact = 4*M_PI*pow(r, 3)/3.0;

    int n[] = {100, 1000, 10000, 100000, 1000000};
    int n_max = sizeof(n)/sizeof(n[0]);

    cout << "-----------------------------------------------------------------------------\n";
    cout << left << setw(15) << "N" << setw(25) << "Calculated Volume" << setw(20) << "Error" << setw(20) << "Time(in sec)" << endl;
    cout << "-----------------------------------------------------------------------------\n";  

    // Calculating volume for different number of samples
    for(int i = 0; i < n_max; i++){
        
        int N = n[i];
        pair<double, double> Results = Calculate_volume(r, N);
        double Volume = Results.first;
        double time = Results.second;

        cout << left << setw(15) << N << setw(25) << Volume << setw(20) << abs(Volume - V_exact) << setw(20) << time << endl;

    }

    return 0;
}