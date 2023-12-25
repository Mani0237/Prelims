#include <iostream>
#include <omp.h>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

pair<double,double> caluclate_area(double inner_radius, double outer_radius, int num_rand_samples){
   random_device rd;
   mt19937 gen(rd());
   int count = 0;
   

   double start = omp_get_wtime();
   for (int i = 0; i < num_rand_samples; i++){
      double x = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);
      double y = uniform_real_distribution<double>(-outer_radius, outer_radius)(gen);

      double radius = sqrt(x*x + y*y);
      if (radius < outer_radius && radius > inner_radius) {
         count++;
      }
   }
   double calc_area = (count / (double)num_rand_samples) * (4 * outer_radius * outer_radius);
   double end = omp_get_wtime();    
   return make_pair(calc_area, end - start);
}	

int main() {
   
   double r = 10.0;
   double R = 20.0;
   double area_exact = 22 * 300 / 7.0;
   int samples[] = {1, 2, 4, 8, 16};
   int n_max = sizeof(samples)/sizeof(int);

   cout << endl;
   cout << "          ***** Results for different samples *****" << endl;
   cout << "--------------------------------------------------------------------" << endl;
   cout << setw(10) << "N" 
        << setw(20) << "Calculated Area"
        << setw(20) << "error" 
        << setw(15) << "Time Taken" << endl;
   cout << "--------------------------------------------------------------------" << endl;

   for (int i = 0; i < n_max; i++){
      int N = 100 * samples[i];
      pair<double, double> res = caluclate_area(r, R, N);
      double error = abs(res.first - area_exact); 
      cout << setw(10) << N 
           << setw(20) << res.first
           << setw(20) << error 
           << setw(15) << res.second << endl; 
   }   
   cout << "--------------------------------------------------------------------" << endl;

   return 0;
}

	
