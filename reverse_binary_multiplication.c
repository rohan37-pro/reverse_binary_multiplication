#include <stdio.h>
#include <stdlib.h>
#include <time.h>


unsigned long long revere_binary_multiplication( unsigned long long multiplier, unsigned long long target_product, int bit_size){

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
            current_product_sum += (multiplier << (factor_bit_ptr+binary_ptr) );
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

    unsigned long long a[10000], b[10000], n[10000], m[10000];
    for (int i = 0; i<10000; i++){
        n[i] = rand() % (63 -2) + 3;
        m[i] = 1ULL << n[i];
        a[i] = rand() % (m[i] - 1) + 1;
        b[i] = rand() % (m[i] - 1) + 1;
    }

    unsigned long long missing_factor_RBM[10000]; 
    unsigned long long missing_factor_eea[10000];
    struct timespec start, end;
    double cpu_time_used_rbm, cpu_time_used_eea;

    // RBM 
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i=0; i< 10000; i++){
        missing_factor_RBM[i] = revere_binary_multiplication(a[i], b[i], n[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    cpu_time_used_rbm = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    printf("RBM runtime %f seconds\n", cpu_time_used_rbm);

    // EEA
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i=0; i< 10000; i++){
        missing_factor_eea[i] = solve_linear_congruence_eea(a[i], b[i], m[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    cpu_time_used_eea = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    printf("EEA runtime %f seconds\n", cpu_time_used_eea);

    int count = 0;
    for (int i=0; i<10000; i++){
        if (missing_factor_eea[i] != missing_factor_RBM[i]) {
            printf("[%llu] %llu * x mod %llu = %llu.. x= RBM(%llu) , EEA(%llu)\n",n[i],a[i], m[i], b[i], missing_factor_RBM[i], missing_factor_eea[i]);
            count ++;
        }
    }
    
    printf("total different answer = %d\n", count);

    return 0;
}