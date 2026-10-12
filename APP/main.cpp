#include "Set.h"
#include <iostream>

Set sieve(int n) {
    Set primes(n);
    Set composite(n);
    for (int i = 2; i * i <= n; ++i)
        if (!composite.is_member(i))
            for (int j = i * i; j <= n; j += i)
                composite += j;
    for (int i = 2; i <= n; ++i)
        if (!composite.is_member(i))
            primes += i;
    return primes;
}

int main() {
    int n;
    std::cout << "n = ";
    if (!(std::cin >> n) || n < 2 || n > 32) {
        std::cerr << "2 <= n <= 32 required\n";
        return 1;
    }
    std::cout << sieve(n) << std::endl;
    return 0;
}