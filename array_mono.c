#include "array.c"
#include <stdio.h>

int main(){
   int inp,statik,membesar,mengecil;
   membesar =0;
   mengecil =0;
   statik =0;
    do
    {
        scanf("%d", &inp);
    } while (!(IsIdxEff(T , inp)));
    
    SetNeff(&T, inp);
    for (int i = 1; i <= T.Neff; i++)
    {
        scanf("%d", &inp);
        SetEl(&T, i, inp);
    }
    
    for(int i = 1 ; i<=T.Neff-1;i++){
        if (T.TI[i] > T.TI[i+1]){
            mengecil += 1;
        }
        else if (T.TI[i] < T.TI[i+1])
        {
            membesar+=1;
        }
        else if (T.TI[i] == T.TI[i+1]){
            statik+=1;
            membesar+=1;
            mengecil += 1;
        }
    }
    
    if (statik == (T.Neff-1)){
        printf("Array monoton statik");
    }
    else if (membesar == (T.Neff-1)){
        printf("Array monoton tidak mengecil");
    }
    else if (mengecil == (T.Neff-1)){
        printf("Array monoton tidak membesar");
    }
    else 
    {
        printf("Array tidak monoton");
    }
    
    return 0;
}