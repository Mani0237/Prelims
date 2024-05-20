#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <chrono>

using namespace std;

pair<double, double> Calculate_volume(double a, double b, double c, int num_samples) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist_x(-a, a);
    uniform_real_distribution<double> dist_y(-b, b);
    uniform_real_distribution<double> dist_z(-c, c);
    
    int count = 0;
    
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < num_samples; i++) {
        double x = dist_x(gen);
        double y = dist_y(gen);
        double z = dist_z(gen);
        
        if ((x*x)/(a*a) + (y*y)/(b*b) + (z*z)/(c*c) < 1) {
            count++;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();
    
    double volume = 8 * a * b * c * static_cast<double>(count) / num_samples;
    
    return make_pair(volume, duration);
}

int main() {
    double a = 3.0;
    double b = 2.0;
    double c = 1.0;
    double V_exact = (4.0 / 3.0) * M_PI * a * b * c;
    int n[] = {1000, 10000, 100000, 1000000, 10000000};
    int N;
    int n_max = sizeof(n) / sizeof(n[0]);

    cout << "-------------------------------------------------------------------" << endl;
    cout << left << setw(12) << "N" 
         << left << setw(20) << "Calculated Volume"
         << left << setw(20) << "Error"
         << left << setw(20) << "Duration (s)" << endl;  
    cout << "-------------------------------------------------------------------" << endl;     
    
    for (int i = 0; i < n_max; i++) {
        N = n[i];
        pair<double, double> result = Calculate_volume(a, b, c, N);
        double volume = result.first;
        double duration = result.second;
        cout << left << setw(12) << N 
             << left << setw(20) << volume
             << left << setw(20) << abs(V_exact - volume)
             << left << setw(20) << duration << endl;        
    }
    
    cout << "-------------------------------------------------------------------" << endl;

    return 0;
}
