#include <iostream>
#include <omp.h>

using namespace std;

int main() {
    int max_threads = omp_get_max_threads();
    cout << "Maximum number of OpenMP threads: " << max_threads << endl;
    return 0;
}
