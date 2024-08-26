#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <iomanip>

using namespace std;


/**
 * ---------------------------------------------------------------------------------
 * @brief function to generateprime numbers using the 
 *        Sieve of Eratosthenes algorithm.
 *        https://www.topcoder.com/thrive/articles/sieve-of-eratosthenes-algorithm# 
 *   
 * @param is_prime - array indicating whether each number is prime.
 * @param limit    - Upper limit for generating primes.
 * ----------------------------------------------------------------------------------
 */
void generate_primes(bool* is_prime, int limit) {
    fill(is_prime, is_prime + limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    
    int sqrt_limit = static_cast<int>(sqrt(limit));

    #pragma omp parallel for schedule(dynamic)
    for (int i = 2; i <= sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

/**
 * --------------------------------------------------------------------------------------
 * @brief Verifies Goldbach's conjecture for even numbers up to a given limit.
 *
 * Goldbach's conjecture states that every even integer greater than 2 can be
 * expressed as the sum of two prime numbers.
 *
 * @param is_prime      - Pointer to an array indicating whether each number is prime.
 * @param limit         - Upper limit for verifying Goldbach's conjecture.
 * @param schedule_type - schedule type for testing load balancing.
 * @param chunk_size    - chunk size 
 * 
 * @return - True if Goldbach's conjecture holds for all even numbers
 *           up to the limit, false otherwise.
 * --------------------------------------------------------------------------------------
 */
bool verify_goldbach(bool* is_prime, int limit, omp_sched_t schedule_type, int chunk_size) {
    bool valid = true;

    omp_set_schedule(schedule_type, chunk_size);

    #pragma omp parallel for schedule(runtime)
    for (int n = 4; n <= limit; n += 2) {
        bool found = false;
        for (int i = 2; i <= n / 2; ++i) {
            if (is_prime[i] && is_prime[n - i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            #pragma omp critical
            {
                std::cout << "Goldbach's conjecture is wrong for " << n << std::endl;
                valid = false;
            }
        }
    }
    return valid;
}

int main() {

    const int limit = 100000000;
    bool* is_prime = new bool[limit + 1];

    const int num_threads_list[] = {1, 2, 4, 8, 16, 28};
    const omp_sched_t schedules[] = {omp_sched_static, omp_sched_dynamic, omp_sched_guided};
    const char* schedule_names[] = {"Static", "Dynamic", "Guided"};

    std::cout << "==================================" << endl;
    std::cout << "     Prime number generation      " << endl;
    std::cout << "==================================" << endl;
    std::cout << "----------------------------------" << endl;
    std::cout << left << setw(10) << "Threads" << setw(20) << "Time (s)" << endl;
    std::cout << "----------------------------------" << endl;

    // Loop over different numbers of threads
    for (auto num_threads : num_threads_list) {
        omp_set_num_threads(num_threads);

        // Measure time
        auto start = chrono::high_resolution_clock::now();
        generate_primes(is_prime, limit);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> diff = end - start;

        // Output the result 
        std::cout << setw(10) << left << num_threads << setw(20) << diff.count() << endl;
    }
    std::cout << "----------------------------------" << endl;
    std::cout << endl;
    
    std::cout << "============================================================" << endl;
    std::cout << left << setw(10) << "Threads" 
         << left << setw(25) << "Time (s)" 
         << left << setw(25) << "Goldbach Conjecture" << endl;
    std::cout << "============================================================" << endl;
    
    for (int s = 0; s < 3; ++s) {
        auto schedule_type = schedules[s];
        auto schedule_name = schedule_names[s];

        std::cout << "----------------------------------------------------" << endl;
        std::cout << "                     " << schedule_name << endl;
        std::cout << "----------------------------------------------------" << endl;
        for (auto num_threads : num_threads_list) {
            omp_set_num_threads(num_threads);

            auto start = chrono::high_resolution_clock::now();
            bool result = verify_goldbach(is_prime, limit, schedule_type, 10000);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> diff = end - start;

            std::cout << left << setw(10) << num_threads
                 << left << setw(25) << diff.count()
                 << left << setw(25) << (result ? "Holds" : "False") << endl;
        }
    }
    std::cout << "============================================================" << endl;

    delete[] is_prime;
    return 0;
}
