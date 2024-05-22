#include <iostream>
#include <vector>
#include <cmath>
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

    for (int i = 2; i * i <= limit; ++i) {
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
 * @param is_prime - Pointer to an array indicating whether each number is prime.
 * @param limit    - Upper limit for verifying Goldbach's conjecture.
 * @return         - True if Goldbach's conjecture holds for all even numbers
 *                   up to the limit, false otherwise.
 * --------------------------------------------------------------------------------------
 */
bool verify_goldbach(bool* is_prime, int limit) {
    bool valid = true;

    for (int n = 4; n <= limit; n += 2) {
        bool found = false;
        for (int i = 2; i <= n / 2; ++i) {
            if (is_prime[i] && is_prime[n - i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Goldbach's conjecture is wrong for " << n << endl;
            valid = false;
        }
    }
    return valid;
}


int main() {
    
    const int limit = 10000000;
    bool* is_prime = new bool[limit + 1];

    auto start = chrono::high_resolution_clock::now();
    generate_primes(is_prime, limit);
    auto end = chrono::high_resolution_clock::now();
    cout << endl;

    chrono::duration<double> diff = end - start;
    cout << "Time to generate primes: " << diff.count() << " s" << endl;

    start = chrono::high_resolution_clock::now();
    bool result = verify_goldbach(is_prime, limit);
    end = chrono::high_resolution_clock::now();
    diff = end - start;

    cout << "Time to verify Goldbach's conjecture: " << diff.count() << " s" << endl;
    cout << "Goldbach Conjecture: " << (result ? "Holds" : "False") << endl;
    cout << endl;

    delete[] is_prime;
    return 0;
}
