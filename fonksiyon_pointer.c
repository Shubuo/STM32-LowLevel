/*
	Fonksiyon pointerı aldığı değerleri arraye atayarak 
	adreslerini print eder.
*/

#include <stdio.h>
    
    int *sum(int x, int y)
    {
        static int arr[2];
        arr[0] = x; arr[1] = y;
        return arr;
    }
    
    int main( )
    {
        int (*fp)(int, int);
        fp = sum(10,15);
        //int s = fp(10, 15);
     for ( int i = 0; i < 2; i++ ) {
      printf("*(p + [%d]) : %d\n", i, *(fp + i) );
        }
       return 0;  
    }