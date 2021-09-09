#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int oppg1(){
    long xy_size = 1000*1000*1000; // 8 GB (sizeof(long) = 8 bytes)
    long x_dim = 1000;
    long y_dim = xy_size/x_dim;
    long** matrix = malloc(y_dim*sizeof(long*));
    for(long y = 0; y < y_dim; y++){
        matrix[y] = malloc(x_dim*sizeof(long));
        memset(matrix[y], 0, x_dim*sizeof(long));
    }
    printf("Allocation complete (press any key to continue...)\n");
    getchar();
    return 0;
}

/*
Q1
Memory get filled up. When the PC becomes responsive again, we see that some of the filled up memory is transferred to swap.
Swap is the part of the virtual memory that actually is on the HDD. This is probably used for paging?
100 var 8gb
1000 var 6.8gb
10000 var 3.2gb

The program pre-allocates with y-dimension and then allocates of size x*size(long).
Seems like the bigger the pre-allocation, the worse?

Writing to un-allocated memory gives segfault. The program isn't allowed to write to memory it has not allocated
Malloc uses lazy evaluation and is therefore allowed to allocate memory unless it's killed by the kernel.
It also allocates from virtual memory which is why it freezes the computer when it allocates too small chunks at a time (x=100).
This causes constant page faulting and thrashing because of frequent virtual memory allocation.


*/


void oppg2(){
    Array kort;
    kort = array_new(3);
    
    array_insertBack_opg2(&kort, 1);
    array_insertBack_opg2(&kort, 2);
    array_insertBack_opg2(&kort, 3);  
    //array_insertBack(&kort, 4); 
    // Hvis vi skriver kort = array_new(2), så kan vi fylle den med 3 ting uten at det segfaulter. Dersom vi har kort  = array_new(3) og fyller den med fire ting, får vi segfault hver gang. Hvorfor er det slik? =fsanitize=address klager uansett på at vi gjør noe galt.
    //AddressSanitizer makes this consistent!
    for (int i = 0; i < 5; i++){
        printf("%li \n", kort.data[i]);
    }
}

int main(){
    Array kort;
    kort = array_new(3);
    array_insertBack_opg3(&kort, 1);
    array_insertBack_opg3(&kort, 2);
    array_insertBack_opg3(&kort, 3);  
    array_insertBack_opg3(&kort, 4);
    array_insertBack_opg3(&kort, 5);  
    for (int i = 0; i < 5; i++){
        printf("%li \n", kort.data[i]);    
    }
    free(kort.data);
    return 0;
}