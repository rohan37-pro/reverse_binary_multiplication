#include <stdio.h>
#include <stdlib.h>

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
            current_product_sum += (multiplier << factor_bit_ptr );
        }
    }
    return missing_factor;
}

int main(){
    unsigned long long a, b, n, x;
    printf("Reverse Binary Multiplication\n");
    printf("to solve: a*x = b (mod 2^n)\n");

    printf("Enter a: ");
    scanf("%llu",&a);
    printf("Enter b: ");
    scanf("%llu",&b);
    printf("Enter n: ");
    scanf("%llu",&n);

    x = revere_binary_multiplication(a, b, n);
    if (x==0)
        printf("Solution Doesn't Exist\n");
    else
        printf("solution x = %llu\n",x);

    return 0;
}
