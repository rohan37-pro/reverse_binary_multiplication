# reverse the binary multiplication to find the factor under under modulo 2^n
# this function find x if : a * x = b mod 2^n  [ where a, b and n are given ]
def reverse_binary_multiplication(multiplier, target_product, bit_size):
    # make a buffer to the size of the array to bit_size for calculating the product
    binary_buffer = 0
    missing_factor = 0

    # setting the pointer where the first 1 appeared in the binary_multiplier from Least Significant Bit
    binary_ptr = 0
    while ((multiplier >> binary_ptr) & 1) != 1:
        if ((target_product >> binary_ptr) & 1) == 1:
            return
        binary_ptr +=1

    
    factor_ptr = 0
    target_ptr = binary_ptr
    while target_ptr < bit_size:
        buffer_index = bit_size - target_ptr -1 
        if  binary_buffer>>target_ptr & 1 != (target_product>>target_ptr) & 1: 
            missing_factor += 1<<factor_ptr
            binary_buffer += multiplier << factor_ptr
            target_ptr += 1
            factor_ptr+=1
        else:
            target_ptr += 1
            factor_ptr+=1
    
    return missing_factor


if __name__=='__main__':
    import time
    print("Reverse Binary Multiplicaiton Method")
    print("solve 'x' if : a * x = b mod 2^n [for given a and b under modulo 2^n]")
    print()
    multiplier = int(input("enter multiplier(a) : "))
    target_product = int(input("enter target product(b) : "))
    bit_size = int(input("enter bit size (n) : "))
    time_start = time.time()
    missing_factor = reverse_binary_multiplication(multiplier, target_product, bit_size)
    if missing_factor == None:
        print("solution dosn't exist !!!")
        print("number-of-zeros-in-LSB-in-target_product > number-of-zeros-in-LSB-in-muliplier")
        quit()
    print(f" missing number is {missing_factor}")
    print(f" impossible equation solved: {missing_factor} * {multiplier} = {target_product}")
    print(f"found in {time.time()-time_start}sec..")