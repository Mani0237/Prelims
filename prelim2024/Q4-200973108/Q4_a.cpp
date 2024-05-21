#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>

using namespace std;

/**
 * -----------------------------------------------------------------------------------------------
 * @brief Function to calculate the volume of an ellipsoid using the Monte Carlo method.
 * 
 * @param a Semi-axis length along the x-axis.
 * @param b Semi-axis length along the y-axis.
 * @param c Semi-axis length along the z-axis.
 * @param num_samples Number of random samples to use for the Monte Carlo simulation.
 * 
 * @return A pair containing the estimated volume of the ellipsoid and the duration 
 *         of the calculation in seconds.
 * -----------------------------------------------------------------------------------------------
 */
pair<double, double> Calculate_volume(double a, double b, double c, int num_samples) {
    
    // generating random numbers 
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist_x(-a, a);
    uniform_real_distribution<double> dist_y(-b, b);
    uniform_real_distribution<double> dist_z(-c, c);
    
    // counter for points lying inside the ellipsoid
    int count = 0;
    
    // starting clock
    auto start = chrono::high_resolution_clock::now();

    // Monte Carlo simulation
    for (int i = 0; i < num_samples; i++) {
        double x = dist_x(gen);
        double y = dist_y(gen);
        double z = dist_z(gen);
        
        // checking if point (x, y, z) lies inside the ellipsoid
        if ((x*x)/(a*a) + (y*y)/(b*b) + (z*z)/(c*c) < 1) {
            count++;
        }
    }

    // stopping clock
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();
    
    // volume calculation: length of domain in each direction is 2a, 2b, 2c respectively
    // So, volume of ellipsoid = Volume domain * ratio of points inside domain
    double volume = 8 * a * b * c * static_cast<double>(count) / num_samples;
    
    return make_pair(volume, duration);
}



int main() {

    // defining parameters
    double a = 3.0;   // semi-axis along x-axis
    double b = 2.0;   // semi-axis along y-axis
    double c = 1.0;   // semi-axis along z-axis

    // formula for volume of ellipsoid
    double V_exact = (4.0 / 3.0) * M_PI * a * b * c;   

    // array for different sample sizes for the MC simulation
    int n[] = {1000, 10000, 100000, 1000000, 10000000};
    int N;
    int n_max = sizeof(n) / sizeof(n[0]);

    cout << "-------------------------------------------------------------------" << endl;
    cout << left << setw(12) << "N" 
         << left << setw(20) << "Calculated Volume"
         << left << setw(20) << "Error"
         << left << setw(20) << "Duration (s)" << endl;  
    cout << "-------------------------------------------------------------------" << endl;     
    
    // loop over each sample size
    for (int i = 0; i < n_max; i++) {

        N = n[i];

        // calculate volume using monte carlo simulation with N samples.
        pair<double, double> result = Calculate_volume(a, b, c, N);
        double volume = result.first;
        double duration = result.second;

        // Printing results for the current sample size
        cout << left << setw(12) << N 
             << left << setw(20) << volume
             << left << setw(20) << abs(V_exact - volume)
             << left << setw(20) << duration << endl;        
    }
    
    cout << "-------------------------------------------------------------------" << endl;

    return 0;
}
