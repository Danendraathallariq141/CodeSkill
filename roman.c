#include "roman.h"
#include <stdio.h>

/* **** Roman Digit Operator *** */
boolean IsRomanDigit(infotype X){
    return((X=='I')|| (X=='V') || (X=='X') || (X=='L') || (X=='C') || (X== 'D') || (X=='M'));
}
/* Mengirim true jika X adalah digit romawi I, V, X, L, C, D, atau M (case sensitive) */
int RomanToDec(infotype X){
    if (IsRomanDigit(X)){
        if (X=='I'){
            return(1);
        }
        else if (X=='V'){
            return(5);
        }
        else if (X=='X'){
            return(10);
        }
        else if (X=='L'){
            return(50);
        }
        else if (X=='C'){
            return(100);
        }
        else if (X=='D'){
            return(500);
        }
        else if (X=='M'){
            return(1000);
        }
    }
}
/* I.S. X adalah digit romawi yang dikenali */
/* F.S. Mengembalikan nilai desimal dari X */
int Decimal(Stack S){
    int jumlah,indeks;
    infotype value, next;
    indeks = S.TOP;
    value = S.T[S.TOP];
    jumlah = RomanToDec(value);
    while(IsRomanDigit(value)){
        next = S.T[indeks-1];
        if (RomanToDec(next) < RomanToDec(value)){
            jumlah -= RomanToDec(next);
        }
        else{
            jumlah += RomanToDec(next);
        }
        value = next;
        if (indeks == 1){
            break;
        }
        else {
            indeks-=1;
        }
    }
    //printf("%d\n", jumlah);
    //jumlah +=value;
    return (jumlah);
}
/* Mengembalikan nilai desimal dari bilangan romawi R */
/* I.S. R berisi digit-digit romawi yang dikenali dengan susunan yang benar,
        digit paling atas adalah digit terakhir */
/* F.S. Mengembalikan nilai desimal dari bilangan romawi R */
