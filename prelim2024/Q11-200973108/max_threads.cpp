#include <iostream>
#include <omp.h>

int main() {
    int max_threads = omp_get_max_threads();
    std::cout << "Maximum number of OpenMP threads: " << max_threads << std::endl;
    return 0;
}
