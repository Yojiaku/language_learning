#include <stdio.h>
typedef enum{false, true}bool;
bool isPrime(int n)
{
    if(n < 2) return false;

    for(int i=2; i<=n/2; i++)
    {
        if(n%i == 0) return false;
    }
    return true;
}

int main()
{
    int start = 1;
    int end = 10;
    for(int i=start; i<=end; i++)
    {
        if(isPrime(i)) printf("%d, ", i);
    }
    printf("\n");
    return 0;
}
