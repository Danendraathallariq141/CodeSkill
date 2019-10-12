#include "stack.h"
#include <stdio.h>

int main(){
    int n,value,next;
    Stack S;
    scanf("%d\n", &n);
/*    if (n==1){
        Push(&S,0);
    }
    else if (n==2){
        Push(&S,0);
        Push(&S,1);
    }
    else{ */
    S.T[0] = 0;
    S.T[1] = 0;
    S.T[2] = 1;
    if (n >2){
        value = S.T[1];
        for(int i=2;i<n;i++){
            next = S.T[i];
            Push(&S,(value+next));
            value=next;    
        }
    }
    printf("%d\n", S.T[0]);
    printf("%d\n", S.T[1]);
    printf("%d\n", S.T[2]);
    printf("%d\n", S.T[n]);
    return (0);
}



