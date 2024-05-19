#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

const double L = 5.0;

// defining function f(x, y)
double f(double x) {
    return 6*x;
}

// defining exact solution
double T_exact(double x) {
    return 100.0 + pow(x,3);
}

// defining function g(x, y) for boundary conditions
double g(double x) {
    if(x == 0.0) {
        return T_exact(x);
    }
    else if(x == L ) {
        return T_exact(x);
    }
    else {
        return 0.0;
    }
}



int main() {

    // defining calculation parameters to be used 
    double h_values[] = {0.25, 0.125, 0.0625};   // grid spacing for Jacobi solver
    double tol = 1.0e-5;                         // tolerance for termination   
    int h_max = sizeof(h_values)/sizeof(double); // total number of grid spacing to be tested 

    cout << endl;
    cout << "       ********** Calculations Started **********        " << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << setw(10) << "h" << setw(20) << "Error" << setw(15) << "Iterations" << endl;
    cout << "---------------------------------------------------------" << endl;
    
    for (int i = 0; i < h_max; i++) {

        // defining some local variables
        double h = h_values[i];  
        int iter = 0;
        double res = 1.0;

        // grid variables
        int n = L/h + 1;   // grid points in x-direction     
        double dx = h;         // grid spacing in x-direction        

        // initializing the solution and boundary conditions
        double u[n];
        double u_new[n]; 
        for (int i = 0; i < n; i++){
            double x = i * h;
            u[i] = g(x);
            u_new[i] = g(x);
        }

        // opening file to store residual
        ofstream residual_file;
        string filename = "Q1_residual_h" + to_string(h) + ".txt";
        residual_file.open(filename);

        // implementing the Jacobi solver
        while (res > tol) {
            for (int i = 1; i < n - 1; i++) {
                    u_new[i]= 0.5 * (u[i-1]+ u[i+1] - dx*dx*f(i*dx));
            }

            // Calculation of L2 Norm residual
            res = 0.0;
            for(int i = 1; i < n - 1; i++){
                    res = res + pow(u_new[i] - u[i], 2);
                    u[i] = u_new[i];
            }

            res = sqrt(res / ((n-2)));
            iter++;

            // writing data to file
            residual_file << setw(5) << iter 
                          << setw(15) << res 
                          << endl;  
        }

        // closing residula file
        residual_file.close();

        // error calculations
        double error = 0.0;
        for(int i = 0; i < n; i++){
                double diff = u[i];
                error = error + diff*diff;
        }

        error = sqrt(h*h*error);
        cout << setw(10) << h << setw(20) << error << setw(15) << iter << endl;

    }

    return 0;
}
