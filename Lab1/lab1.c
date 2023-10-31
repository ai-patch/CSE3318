/*********************************************  
Omega compile command:  gcc -o lab1 lab1.c -std=c99
**********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
  int numberoftrgts;
  int sizeA;
  int sizeB;
  scanf("%d %d %d", &sizeA, &sizeB, &numberoftrgts);
  
  int* a = (int *)malloc((sizeA+2) * sizeof(int));
  int* b = (int *)malloc((sizeB+2) * sizeof(int));
  int* trgts = (int *)malloc(numberoftrgts * sizeof(int));
  
  for(int i = 1; i < sizeA+1; i++)
  {
    scanf("%d", &a[i]);
  }
  a[0] = -9999999;
  a[sizeA+1] = 9999999;

  for(int i = 1; i < sizeB+1; i++)
  {
    scanf("%d", &b[i]);
  }
  b[0] = -9999999;
  b[sizeB+1] = 9999999;

  for(int i = 0; i < numberoftrgts; i++)
  {
    scanf("%d", &trgts[i]);
  }

  int largestSize;
  int lf,rt;
  int i, j;
  
  if(sizeA < sizeB)
  {
    largestSize = sizeB;
  }
  else
  {
    largestSize = sizeA;
  }

  int target;
  for(int ell = 0; ell < numberoftrgts; ell++)
  {
    target = trgts[ell];

    if(target > largestSize)
    {
      lf = target - largestSize;
    }
    else
    {
      lf = 0;
    }
    if(target > sizeA)
    {
      rt = sizeA;
    }
    else
    {
      rt = target;
    }

    while(lf <= rt)
    {
      i = (lf + rt)/2;
      if(i > sizeA+1)
      {
        i = sizeA;
      }

      printf("lf %d, rt %d ", lf, rt);
      j = target - i;
      if(a[i] <= b[j])
      {
        printf("i %d j %d a[%d] = %d b[%d] = %d ",i, j, i,a[i], j, b[j]);
        printf("a[%d]=%d\n", i+1,a[i+1]);
      }
      else
      {
        printf("i %d j %d b[%d] = %d a[%d] = %d ",i, j, j,b[j], i, a[i]);
        printf("b[%d]=%d\n", j+1,b[j+1]);
      }

        if(i + j == target && a[i] > b[j] && a[i] <= b[j+1])      //a
        {
          printf("a[%d]=%d has rank %d\n", i, a[i], target);
          break;
        }
        else if(i + j == target && a[i] <= b[j] && b[j] < a[i+1]) //b
        {
          printf("b[%d]=%d has rank %d\n", j, b[j], target);
          break;
        }
        else if(a[i + 1] <= b[j])                                 //small a
        {
          lf = i+1;
        }
        else                                                      //big a
        {
          rt = i-1;
        }

      }
  }
  free(a);
  free(b);
}
