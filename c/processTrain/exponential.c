#include <stdio.h>
int main()
{
    float realtime[7] = {6, 4, 6, 4, 13, 13, 13};
    float predicttime[7];
    predicttime[0] = 10;
    float alpha = 0.5;
    int i;
    for(i=0; i<7; i++){
        predicttime[i+1] = alpha*realtime[i] + (1-alpha)*predicttime[i];
    }
    printf("CPU区间:"); 
    for(i=0; i<7; i++){
        printf("%f, ", realtime[i]);
    }
    printf("\n");
    printf("预测区间:");
    for(i=0; i<7; i++){
        printf("%f, ", predicttime[i]);
    }
    printf("\n");

    return 0;
}
