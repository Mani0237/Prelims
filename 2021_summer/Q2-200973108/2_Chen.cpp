#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;

struct Point {
    double x, y;
};

bool is_inside_circle(double sample_x, double sample_y, double center_x, double center_y, double radius) {
    return pow(sample_x - center_x, 2) + pow(sample_y - center_y, 2) <= pow(radius, 2);
}

void generate_samples(double x_low, double y_low, double x_up, double y_up, int num_samples, double* x, double* y) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis_x(x_low, x_up);
    uniform_real_distribution<> dis_y(y_low, y_up);

    for (int i = 0; i < num_samples; ++i) {
        x[i] = dis_x(gen);
        y[i] = dis_y(gen);
    }
}

double accept_reject_monte_carlo(const Point* centers, const double* radius, int num_circles, int num_samples) {
    double x_low = -5.5, y_low = -3.5, x_up = 5.5, y_up = 5.5;
    double area_sampling_region = (x_up - x_low) * (y_up - y_low);
    double* x = new double[num_samples];
    double* y = new double[num_samples];

    generate_samples(x_low, y_low, x_up, y_up, num_samples, x, y);

    int inside_count = 0;
    for (int i = 0; i < num_samples; ++i) {
        bool inside_all_circles = true;
        for (int j = 0; j < num_circles; ++j) {
            if (!is_inside_circle(x[i], y[i], centers[j].x, centers[j].y, radius[j])) {
                inside_all_circles = false;
                break;
            }
        }
        if (inside_all_circles)
            inside_count++;
    }

    delete[] x;
    delete[] y;

    return (double)inside_count / num_samples * area_sampling_region;
}

int main() {
    
    Point centers[] = { { -2, 0 }, { 2, 0 }, { 0, 2 } };
    double radius[] = { 3, 3, 3 };
    int num_samples_list[] = { 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
    int num_circles = sizeof(centers) / sizeof(centers[0]);

    cout << "----------------------------------------------------" << endl;
    cout << setw(15) << left << "N" 
         << setw(15) << "Area" 
         << setw(15) << "Time(in ms)" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int num_samples : num_samples_list) {
        auto start_time = chrono::steady_clock::now();
        double area_estimate = accept_reject_monte_carlo(centers, radius, num_circles, num_samples);
        auto end_time = chrono::steady_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        
        cout << setw(15) << left << num_samples 
         << setw(15) << area_estimate
         << setw(15) << elapsed_time.count() << endl;
    }

    cout << "----------------------------------------------------" << endl;

    return 0;
}
