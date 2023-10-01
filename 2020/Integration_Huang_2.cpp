#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

double f(double x){
    return 1.0/sqrt(1.0 + pow(x, 2));
}

pair<double, double> Calculate_integral(double a, double b, int num_sample) {
    double integral = 0;
    double duration = 0;

    random_device rd;
    mt19937 gen(rd());

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < num_sample; i++) {

        double x = uniform_real_distribution<double>(a, b)(gen);
        integral += f(x);
    }

    auto end = chrono::high_resolution_clock::now();
    duration = chrono::duration<double>(end - start).count();

    double calculated_integral = (b - a) * integral / static_cast<double>(num_sample);
    
    return make_pair(calculated_integral, duration);
}

int main() {
    double a = -1.0;
    double b = 10.0;

    int n[] = {100, 1000, 10000, 100000, 1000000, 10000000};
    int n_max = sizeof(n) / sizeof(n[0]);
    double exact_result = 3.8795965;

    cout << "-----------------------------------------------------------------------------\n";
    cout << left << setw(15) << "N" << setw(25) << "Calculated Integral" << setw(20) << "Error" << setw(20) << "Time (in sec)" << endl;
    cout << "-----------------------------------------------------------------------------\n";

    for (int i = 0; i < n_max; i++) {
        int N = n[i];
        pair<double, double> Results = Calculate_integral(a, b, N);
        double Result = Results.first;
        double time = Results.second;
        double error = abs(Result - exact_result);

        cout << left << setw(15) << N << setw(25) << Result << setw(20) << error << setw(20) << time << endl;
    }
    cout << "-----------------------------------------------------------------------------\n";
    
    return 0;
}
