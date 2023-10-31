/***************************************************
Omega compile command:  gcc -o lab2 lab2.c -std=c99
***************************************************/

#include <stdio.h>
#include <stdlib.h>

int find_subsqnce(int *sqnce, int* subsqnce, int sqnce_len, int subsqnce_len, int interleave_fac)
{
    for (int i = 0, j = 0; i < subsqnce_len; i++)
    {
        int count = 0;
        while (1)
        {
            if (sqnce[j++] == subsqnce[i])
            {
                if (++count == interleave_fac)
                {
                break;
                }
            }
            else if (j >= sqnce_len)
            {
            return 0;
            }
        }
    }
    return 1;
}

int bin_search_subsqnce(int *A, int *X, int Alen, int Xlen, int max_subsqnce_len)
{
    int high = max_subsqnce_len;
    int low = 0;
    int max_repeats = 0;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        int subsqnce_found = find_subsqnce(A, X, Alen, Xlen, mid);
        if (subsqnce_found == 0)
        {
        printf("low %d mid %d high %d failed\n", low, mid, high);
        high = mid - 1;
        }
        else
        {
         printf("low %d mid %d high %d passed\n", low, mid, high);
         max_repeats = mid > max_repeats ? mid : max_repeats;
         low = mid + 1;
        }
    }
    return max_repeats;
}

int main()
{
    int *A = NULL;
    int *X = NULL;
    int Alen = 0;
    int Xlen = 0;
    int i;
    int max_subsqnce_len; 
    int max_interleave_fac;
    scanf("%d",&Alen);
    scanf("%d",&Xlen);
    A = malloc(Alen * sizeof(int));
    X = malloc(Xlen * sizeof(int));
    max_subsqnce_len = Alen / Xlen;

    for (i = 0; i < Alen; i++)
    {
        scanf("%d", &A[i]);
    }
    scanf("%d", &i);
   
    if (i != -999999999)
    {
        printf("Error in reading A.\n");
        exit(1);
    }

    for (i = 0; i < Xlen; i++)
    {
        scanf("%d", &X[i]);
    }

    scanf("%d", &i);

    if (i != -999999999)
    {
        printf("Error is reading X.\n");
        exit(1);
    }

    max_interleave_fac = bin_search_subsqnce(A, X, Alen, Xlen, max_subsqnce_len);
    printf("Maximum repeats is %d\n", max_interleave_fac);
    free(A);
    free(X);
    return 0;
}