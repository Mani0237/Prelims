#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "gauss_elimination.h"

using namespace std;

// defining the analytic solution
double T_exact(double x) {
    return 100.0 + pow(x,3);
}

// function to evaluate exact solution
double evaluate_exact(double x) {
    
}