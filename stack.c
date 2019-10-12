#include "stack.h"
#include <stdio.h>
/* ********* Prototype ********* */
Stack S;
#define TOP(S) (S).TOP 
#define InfoTOP(S) (S).T[TOP(S)]

/* *** Konstruktor/Kreator *** */
void CreateEmpty(Stack *S){
    TOP(*S) = Nil;
}
/* I.S. Sembarang */
/* F.S. Membuat sebuah stack S yang kosong berkapasitas MaxEl */
/* Ciri stack kosong : TOP bernilai Nil */


/* ********* Predikat Untuk test keadaan KOLEKSI ********* */
boolean IsEmpty(Stack S){
    return (TOP(S) == 0);
}
/* Mengirim true jika Stack kosong*/
/* Ciri stack kosong : TOP bernilai Nil */
boolean IsFull(Stack S){
    return (TOP(S) == MaxEl);
}
/* Mengirim true jika stack S penuh */
/* Ciri stack penuh : TOP bernilai MaxEl */

/* ********** Operator Dasar Stack ********* */
void Push(Stack *S, infotype X){
    TOP(*S) += 1;
    InfoTOP(*S) = X;
}
/* Menambahkan X sebagai elemen Stack S. */
/* I.S. S mungkin kosong, S tidak penuh */
/*F.S. X menjadi element TOP yang baru, TOP bertambah 1 */
void Pop (Stack *S, infotype *X){
    *X = InfoTOP(*S);
    TOP(*S) -= 1;

}
/* Menghapus X dari Stack S. */
/* I.S. S tidak kosong */
/* F.S. X adalah nilai elemen TOP yang lama, TOP berkurang 1 */

