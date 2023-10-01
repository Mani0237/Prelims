/*===============================================================================
SUMMARY:
1. This code imolements Gauss Legendre Method of calculating Integrals.
2. In the following code you can easily change the function 'g' to be calculated, 
   the limits a and b. 
3. But if you want to change the number of quadrature points, you have to modify 
   the arrays nodes and weights.   
=================================================================================*/


#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

double g(double t){
    return 1.0/(sqrt(1 + pow(t, 2)));
}

double gauss_legendre(double a, double b){

    double nodes[5] = {-0.9061798459, -0.5384693101, 0.0, 0.5384693101, 0.9061798459};
    double weights[5] = {0.2369268851, 0.4786286705, 0.5688888888, 0.4786286705, 0.2369268851};

    double integral = 0.0;
    double x;

    for (int i = 0; i < 5; i++){
        x = (b - a) * nodes[i] / 2.0 + (a + b) / 2.0; 
        integral += 0.5 * (b - a) * weights[i] * g(x);
    }

    return integral;
}

int main(){

    double a = 0.0;
    double b = 5.0;
    double result_exact = 2.312438341272753;

    double result = gauss_legendre(a, b);
    cout << "The integral is = " << result << " with error = " << abs(result_exact - result) << endl;

    return 0;
}
