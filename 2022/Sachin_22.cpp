#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <string>
#include <iomanip>

using namespace std;

double dv_dt(double t, double x, double v){
    double P = 0.1;
    double gamma = 0.5;
    double zeta = 0.2;
    double omega = 1;

    return P*cos(omega*t) - zeta*v - x - gamma*pow(x, 3);
}

int main(){

    double H[] = {1, 0.5, 0.1, 0.05};
    int H_size = sizeof(H)/sizeof(H[0]); 

    cout << "---------------------------------\n";
    cout << left << setw(10) << "h" << setw(20) << "Time Taken" << endl;\
    cout << "---------------------------------\n";    

    for(int i = 0; i < H_size; i++){
        
        double x = 0;
        double v = 0;
        double t = 0;
        double T = 100;
        double duration = 0;
        double h = H[i];
        
        ofstream myfile;
        string filename = "Ode_sachin_" + to_string(i) + ".txt";
        myfile.open(filename);

        auto start = chrono::high_resolution_clock::now();
        while(t <= T){
        
            myfile << t << "\t" << x << "\t" << v << endl;

            double kx1 = v;
            double kv1 = dv_dt(t, x, v);

            double kx2 = v + h*kv1/2;
            double kv2 = dv_dt(t + h/2, x + h*kx1/2, v + h*kv1/2);
        
            double kx3 = v + h*kv2/2;
            double kv3 = dv_dt(t + h/2, x + h*kx2/2, v + h*kv2/2);
        
            double kx4 = v + h*kv2;
            double kv4 = dv_dt(t + h, x + h*kx2, v + h*kv2);

            x = x + (h/6)*(kx1 + 2*kx2 + 2*kx3 + kx4);
            v = v + (h/6)*(kv1 + 2*kv2 + 2*kv3 + kv4);
            t = t + h;

        }

        auto end = chrono::high_resolution_clock::now();
        duration = chrono::duration<double>(end - start).count();       

        cout << left << setw(10) << h << setw(20) << duration << endl;       
    
    } 

    return 0;
}