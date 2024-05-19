#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

const double L = 5.0;

// defining function f(x) which act as source term for MES
double f(double x) {
    //return 6*(x+1)-2.0*tanh(x-5)/(cosh(x-5)*cosh(x-5));
    return 6.0*x;
}

// Function to evaluate exact solution
double T_exact(double x) {
    //return 1.0 + pow(x+1, 3) + tanh(x-5);
    return 100.0 + pow(x,3);
}

// Function to evaluate boundary conditions
double evaluate_boundary(double x) {
    return T_exact(x); 
}

// calculate residual and write in file
double calculate_residual(double* u, double* u_new, int N, double h) {
    double res = 0.0;
    for (int i = 1; i < N-1; i++){
        res += pow(u_new[i] - u[i], 2);
    }
    res = sqrt(res/(N-2));
    return res;
}

// Function to calculate norms
void calculate_norms(double* T, int N, double h, double& L1, double& L2, double& Linf) {
    L1 = 0.0;
    L2 = 0.0;
    Linf = 0.0;
    for (int i = 0; i < N; ++i) {
        double x = (i*h);
        double error = abs(T[i]);
        L1 = L1 + error;
        L2 = L2 + error * error;
        if (error > Linf) {
            Linf = error;
        }
    }
    L1 = h * L1;
    L2 = h * sqrt(L2);
}

int main() {
    // defining calculation parameters to be used 
    int N_zones[] = {8, 16, 32, 64, 128, 256};   // grid spacing for Jacobi solver
    double tol = 1.0e-5;                         // tolerance for termination   
    int N_max = sizeof(N_zones) / sizeof(int);   // total number of grid spacing to be tested 

    cout << endl;
    cout << "       ********** Calculations Started **********        " << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    cout << setw(10) << "N_zones" << setw(20) << "L1 norm" << setw(20) << "L2 Norm" << setw(20) << "L_inf" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    
    for (int k = 0; k < N_max; k++) {
        // defining some local variables
        int N = N_zones[k] + 1;
        double h = L / (N-1);
        double dx = h;
        double res = 1.0;
        double w = 0.5;

        // initialize the solution and boundary conditions
        double T[N] = {0};
        double T_new[N] = {0};
        T[0] = T_exact(0.0);
        T[N-1] = T_exact(5.0);
        T_new[0] =  T[0];
        T_new[N-1] =  T[N-1];

        // opening file to store residual
        ofstream res_file;
        string filename = "res_N" + to_string(N_zones[k]) + ".txt";
        res_file.open(filename);
        
        // implementing the weighted Jacobi solver
        while (res > tol) {
            for (int i = 1; i < N-1; i++) {
                T_new[i] = 0.5 * (T[i+1] + T[i-1] - h*h*f(i * dx));
            }

            // calculating residual and writing to file
            res = calculate_residual(T, T_new, N, h);
            res_file << fixed << setprecision(4);
            res_file << res << endl;

            // update T for next iteration
            for (int i = 1; i < N - 1; ++i) {
                T[i] = T_new[i] ;
            }
        }

        // closing residual file
        res_file.close();

        // calculate norms
        double L1 = 0.0;
        double L2 = 0.0;
        double Linf = 0.0;
        calculate_norms(T, N, h, L1, L2, Linf);

        // Output the results for the current mesh resolution
        cout << fixed << setprecision(4);
        cout << setw(10) << N_zones[k] << setw(20) << L1/N << setw(20) << L2/N << setw(20) << Linf/N << endl;

        // Save solution and exact values to a file
        ofstream sol_file;
        string sol_filename = "solution_N" + to_string(N_zones[k]) + ".txt";
        sol_file.open(sol_filename);
        sol_file << fixed << setprecision(4);
        for (int i = 0; i < N; ++i) {
            double x_coord = i * h;
            sol_file<< setw(15) << x_coord << setw(20) << T[i] << setw(20) << T_exact(x_coord) << endl;
        }
        sol_file.close();

    }
    cout << "--------------------------------------------------------------------------" << endl;

    return 0;
}