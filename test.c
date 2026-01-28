#include "fixedpoint_fast_inverse_square_root_lib.h"
#include <stdio.h>
#include <math.h>

int main(void){
    //float input[] = {0.00015625f, 0.000364f, 1.0f, 2.0f, 4.0f, 10.0f, 1.356025f, 25.0f};
    float input[] = {0.0005f,0.001f,0.005f,0.01f,0.05f,0.1f,0.5f,1.0f,2.0f,5.0f,10.0f,20.0f};
    printf("Fast Inverse Square Root Test\n");
    printf("input\t\tfast_inv_sqrt\tMath\t\tDiff\n");
    printf("------------------------------------------------------------\n");
    for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++){
        int input_fixed = FLOAT2FIX(input[i]);
        int result_fixed = fixedpoint_fast_inverse_square_root(input_fixed);
        float result = FIX2FLOAT(result_fixed);
        printf("%.10f\t%.10f\t%.10f\t%.10f\n", input[i], result, 1.0f / sqrtf(input[i]), fabsf(result - (1.0f / sqrtf(input[i]))));
    }
    return 0;
}
