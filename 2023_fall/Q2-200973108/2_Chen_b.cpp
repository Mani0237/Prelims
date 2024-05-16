#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>

using namespace std;

// defining the function to be integrated
double f(double x, double y) { 
    return x*x + y*y;
}

// function for generating samples
void generate_samples(double x_min, double x_max, double y_min, double y_max, int samples, double* x, double* y) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis_x(x_min, x_max);
    uniform_real_distribution<> dis_y(y_min, y_max);

    for (int i = 0; i < samples; i++) {
        x[i] = dis_x(gen);
        y[i] = dis_y(gen);
    }
}

int main() {

    double x_min = -1.0;
    double x_max = 1.0;
    double y_min = -1.0;
    double y_max = 1.0;
    double exact_result = 2.666666666;
    double samples[] = {10, 100, 1000, 10000, 100000, 1000000};
    double num_samples = sizeof(samples)/sizeof(samples[0]);

    cout << "-----------------------------------------------------" << endl;
    cout << setw(10) << left << "N" 
         << setw(15) << "I"
         << setw(15) << "Error" << endl;
    cout << "-----------------------------------------------------" << endl;

    for (int N : samples) {
        double* x = new double[N];
        double* y = new double[N];
        double sum = 0.0;

        generate_samples(x_min, x_max, y_min, y_max, N, x, y);

        for (int i = 0; i < N; i++) {
            sum = sum + f(x[i],y[i]); 
        }
        double I = sum * (x_max - x_min) * (y_max - y_min)/ N;
        cout << setw(10) << left << N 
             << setw(15) << I
             << setw(15) << abs(I-exact_result) << endl;
        
        delete[] x;
        delete[] y;

    }

    cout << "-----------------------------------------------------" << endl;

    return 0;
}