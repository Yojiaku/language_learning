/*We've implicitly assumed that each call to Compute_next_value requires roughly the same amount of work as the other calls. How would you change your answer to the preceding question if call i = k requires k+1 times as mush work as the call with i=0? So if the first call (i=0) requires 2 milliseconds, the second call(i=1) requires 4, the third(i=2) requires 6, and so on.
*/
#include <stdio.h>
/* assign[c][j] is the jth value of i assigned to core c */
/* work[c] is the total amount of work assigned to core c */
// int n: 数据量
// int p: 核个数
void range_calculate(int n, int p)
{
    int c = 0;
    int j = 0;
    int i;
    int work[4] = {0, 0, 0, 0};
    int assign_[4][12];
    int m, k;
    for(m=0; m<4; m++)
        for(k=0; k<12; k++)
            assign_[m][k] = -1;

    for(i=0; i<n; i++){
        work[c] += 2*(i+1);
        assign_[c][j] = i;
        c = (c+1)%p;  // 重点！！
        if(c==0) j++;
    }   

    printf("| Core | Value of i | Total Work |\n");
    for(m=0;m<p;m++){
        printf("| %d |", m);
        for(k=0;k<n/p;k++){
            printf(" %d ", assign_[m][k]);
        }
        printf("| %d |\n", work[m]);
    }
}

int main()
{
    range_calculate(12, 4);
    return 0;
}
