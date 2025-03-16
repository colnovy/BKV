#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int N;
    printf("enter N \n");
    scanf("%d", &N);

    if (N<=2) {
    printf("there are no prime numbers \n");
    return 0;
    }

    bool array[N];
    int i;
    for(i=0; i<N; i++) {
    array[i]=true;
    }
    array[0]=false; 
	array[1]=false;
	
    int s;
    for (s=2; s<N; s++) {
    if (array[s]==true) {
    int j;
    for (j=s*s; j<N; j+=s) {
    array[j]=false;
    }
	}
    }

    int N1 = 0;
    int h;
    for (h= N-1; h>=2; h--) {
    if (array[h]==true) {
    N1=h;
    break;
    }
    }

    if (N1!=0) {
    printf("%d",N1);
    }
	else {
    printf("there are no prime numbers");
    }
    return 0;
}
