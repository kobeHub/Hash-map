#include <math.h>

#include "prime.h"


/**
 * Return whether n is prime or not
 *
 * returns:
 *  1: prime
 *  0: not prime
 *  -1: undefined (ie: n < 2)
 *
* ***/
int is_prime(int n) {
  if (n < 2) {return -1;}
  if (n < 4) {return 1;}
  if ((n % 2) == 0) {return 0;} 
  for (int i = 3; i <= floor(sqrt((double) n)); i += 2) {
    if (n % i == 0) {
      return 0;
    }
  }
  return 1;
}


/**
 * Return the next prime
** */
int next_prime(int n) {
  while (is_prime(n) == 1) {
    n++;
  }
  return n;
}
