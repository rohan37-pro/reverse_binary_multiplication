#include <stdio.h>
#include <stdlib.h>
#include <time.h>


unsigned long long reverse_binary_multiplication( unsigned long long multiplier, unsigned long long target_product, int bit_size){

    unsigned long long current_product_sum = 0;
    unsigned long long missing_factor = 0;
    
    int binary_ptr = 0;
    while (!((multiplier >> binary_ptr) & 1ULL) && binary_ptr < bit_size) {
        if (((target_product >> binary_ptr) & 1ULL) == 1ULL) {
            return 0;
        }
        binary_ptr++;
    }
    if (binary_ptr == bit_size && target_product != 0) {
        return 0;
    }
    for (int factor_bit_ptr = 0; (factor_bit_ptr + binary_ptr) < bit_size; ++factor_bit_ptr) {
        
        int target_bit = (target_product >> (factor_bit_ptr + binary_ptr)) & 1ULL;
        int product_bit = (current_product_sum >> (factor_bit_ptr + binary_ptr)) & 1ULL;
        if (target_bit != product_bit) {
            missing_factor |= (1ULL << factor_bit_ptr);
            current_product_sum += (multiplier << factor_bit_ptr );
        }
    }
    return missing_factor;
}

unsigned long long gcdExtended(unsigned long long a, unsigned long long b, unsigned long long *x, unsigned long long *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    unsigned long long x1, y1;
    unsigned long long g = gcdExtended(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return g;
}

unsigned long long solve_linear_congruence_eea(unsigned long long a, unsigned long long b, unsigned long long m) {
    unsigned long long x_inv, y_dummy;
    unsigned long long common_divisor = gcdExtended(a, m, &x_inv, &y_dummy);

    if (b % common_divisor != 0) {
        return 0; // No solution
    }

    unsigned long long a_prime = a / common_divisor;
    unsigned long long b_prime = b / common_divisor;
    unsigned long long m_prime = m / common_divisor;

    x_inv = (x_inv % m_prime + m_prime) % m_prime;

    unsigned long long solution = (x_inv * b_prime) % m_prime;

    return solution; // This gives one solution modulo m/gcd(a,m)
}

int main(){
    int NUM_TEST_CASES = 1000;
    int repeate_tests = 100000;
    unsigned long long *a = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long));
    unsigned long long *b = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long));
    unsigned long long *n = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long));
    unsigned long long *m = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long));
    unsigned long long *missing_factor_RBM = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long)); 
    unsigned long long *missing_factor_eea = (unsigned long long *)malloc(NUM_TEST_CASES * sizeof(unsigned long long));

    // Basic error checking for malloc
    if (!a || !b || !n || !m || !missing_factor_RBM || !missing_factor_eea) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }
    
    for (int i = 0; i<NUM_TEST_CASES; i++){
        n[i] = rand() % 61 + 3;
        m[i] = 1ULL << n[i];
        a[i] = rand() % (m[i] - 2) + 1;
        b[i] = rand() % (m[i] - 2) + 1;
    }
    struct timespec start, end;
    double cpu_time_used_rbm=0, cpu_time_used_eea=0;
    int count_difference = 0;

    // repeate the test cases to find an avarage time.
    for (int i=0; i<repeate_tests; i++){
        // pass test cases to RBM 
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i=0; i< NUM_TEST_CASES; i++){
            missing_factor_RBM[i] = reverse_binary_multiplication(a[i], b[i], n[i]);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        cpu_time_used_rbm += (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
        

        // pass test cases to EEA
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i=0; i< NUM_TEST_CASES; i++){
            missing_factor_eea[i] = solve_linear_congruence_eea(a[i], b[i], m[i]);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        cpu_time_used_eea += (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
        

        // counting the number of differente output of EEA and RBM
        for (int i=0; i<NUM_TEST_CASES; i++){
            if (missing_factor_eea[i] != missing_factor_RBM[i]) {
                // printf("[%llu] %llu * x mod %llu = %llu.. x= RBM(%llu) , EEA(%llu)\n",n[i],a[i], m[i], b[i], missing_factor_RBM[i], missing_factor_eea[i]);
                count_difference ++;
            }
        }
    }
    printf("after reapeating %d test cases %d times\n",NUM_TEST_CASES, repeate_tests);
    printf("total runtime of RBM %f seconds\n", cpu_time_used_rbm);
    printf("total runtime of EEA %f seconds\n", cpu_time_used_eea);
    printf("runtime_of_EEA / runtime_of_RBM = %f\n",cpu_time_used_eea/cpu_time_used_rbm);
    printf("number of different answer for all (test_case*repeat)= %d\n", count_difference);

    return 0;
}