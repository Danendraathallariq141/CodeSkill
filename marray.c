//isti

#include <stdio.h>
#include "array.c"

//fungsi


//main program
int main (){
    int  inp, up, down, temp1, elemen;
    TabInt T;
    do
		{
		scanf("%d", &elemen);
		}
	while (!(IsIdxEff(T, elemen)));
    SetNeff(&T, elemen);
    for (int i = 1; i <= T.Neff; i++)
    {
        scanf("%d", &inp);
        SetEl(&T, i, inp);
    }
    printf("[");
    for (int i = 1; i < T.Neff; i++)
    {
        printf("%d, ", T.TI[i]);
    }
    printf("%d]\n", T.TI[T.Neff]);
//up
    up = 1;
    down = 1;
    for (int i = 2; i <= (T.Neff); i++){
        if (T.TI[i] >= T.TI[up])
        {
            temp1 = i;
            i = up;
            up = temp1;
        }
    }
//min
    for (int i = 2; i <= (T.Neff); i++){
        if (T.TI[i] <= T.TI[down])
        {
            temp1 = i;
            i = down;
            down = temp1;
        }
    }
    printf("%d\n", T.TI[down]);
    printf("%d\n", T.TI[up]);
    return 0;
}