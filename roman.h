#include "stack.h"

#ifndef ROMAN_H
#define ROMAN_H

/* MODUL ROMAWI*/

/* **** Roman Digit Operator *** */
boolean IsRomanDigit(infotype X);
/* Mengirim true jika X adalah digit romawi I, V, X, L, C, D, atau M (case sensitive) */
int RomanToDec(infotype X);
/* I.S. X adalah digit romawi yang dikenali */
/* F.S. Mengembalikan nilai desimal dari X */
int Decimal(Stack S);
/* Mengembalikan nilai desimal dari bilangan romawi R */
/* I.S. R berisi digit-digit romawi yang dikenali dengan susunan yang benar,
        digit paling atas adalah digit terakhir */
/* F.S. Mengembalikan nilai desimal dari bilangan romawi R */

#endif