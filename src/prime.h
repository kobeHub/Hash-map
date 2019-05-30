// The header to generate prime number for hash table resize.
// Given the basic size, if need to resize up, double the basic, 
// and find the minimum prime number but larger than the basic.
//
// if need to size down, half the basic, do simlar ops.

int is_prime(const int n);
int next_prime(int n);
