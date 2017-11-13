/*Devise formulas for the functions that calculate my_first_i and my_last_i in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the casewhen n is evenly divisible by p.
*/

#include <stdio.h>

// range_calculate: 计算每个核的my_first_i & my_last_i
// int n: 数据量
// int p: 核数
// int rank: 核的号码（从0开始）
void range_calculate(int my_rank, int n, int p)
{
    int my_first_i, my_last_i;
    int my_n_count;
    int quotient = n/p; // 商
    int remainder = n%p; // 余（决定了前多少个核需要+1）
    if(my_rank < remainder){
        my_n_count = quotient + 1;
        my_first_i = my_rank * my_n_count;
    }else{
        my_n_count = quotient;
        my_first_i = my_rank * my_n_count + remainder;
    }
    my_last_i = my_first_i + my_n_count - 1;
    printf("my_first_i = %d, my_last_i = %d\n", my_first_i, my_last_i);
}

int main()
{
    int i;
    for(i=0; i<3; i++){
        range_calculate(i, 8, 3);
    }

    return 0;
}
