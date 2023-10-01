#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

pair<double, double> Calculate_integral(double radius, int num_sample) {
    double integral = 0;
    double duration = 0;

    random_device rd;
    mt19937 gen(rd());

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < num_sample; i++) {
        double x = uniform_real_distribution<double>(-radius, radius)(gen);
        double y = uniform_real_distribution<double>(-radius, radius)(gen);
        double z = uniform_real_distribution<double>(-radius, radius)(gen);
        double distance = sqrt(x * x + y * y + z * z);
        
        if (distance < 1) {
            double function_value = 1 + x * x + y * y + z * z;
            integral += function_value;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    duration = chrono::duration<double>(end - start).count();

    double average_integral = integral / static_cast<double>(num_sample);
    double calculated_integral = average_integral * pow(2 * radius, 3);
    
    return make_pair(calculated_integral, duration);
}

int main() {
    double r = 1.0;

    int n[] = {100, 1000, 10000, 100000, 1000000};
    int n_max = sizeof(n) / sizeof(n[0]);
    double exact_volume = 32 * M_PI / 15;

    cout << "-----------------------------------------------------------------------------\n";
    cout << left << setw(15) << "N" << setw(25) << "Calculated Volume" << setw(20) << "Error" << setw(20) << "Time (in sec)" << endl;
    cout << "-----------------------------------------------------------------------------\n";

    for (int i = 0; i < n_max; i++) {
        int N = n[i];
        pair<double, double> Results = Calculate_integral(r, N);
        double Volume = Results.first;
        double time = Results.second;
        double error = abs(Volume - exact_volume);

        cout << left << setw(15) << N << setw(25) << Volume << setw(20) << error << setw(20) << time << endl;
    }

    return 0;
}
