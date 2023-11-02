/******************************************************************
 *  Omega compile command:  gcc -o lab3 lab3.c                  *
 *  ---------------------> ./lab3 < a.dat                       *
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define max(a, b) (a > b ? a : b)
const int MAX = 51;

typedef struct Work
{
    int startTime;
    int endTime;
    int wt;
} 
Work;

int binSearch(Work *runs, int n, int key)
{
    int high = n - 1;
    int low = 0;
    while (low <= high)
    {
        int mid = (low + high) / 2;

        if (runs[mid].endTime <= key)
        {
            low = mid + 1; 
        }
        else
        {
            high = mid - 1;
        }
    }
    return high;
}

int MaxWeightedSched(Work *arr, size_t n)
{
    int P[MAX];
    int table[MAX][MAX];

    Work *room1 = (Work *)malloc((n + 1) * sizeof(Work));
    Work *room2 = (Work *)malloc((n + 1) * sizeof(Work));
    int room1CT = 0;
    int room2CT = 0;
    P[0] = -1;

    for (int i = 1; i <= n; i++)
    {
        P[i] = binSearch(arr, n + 1, arr[i].startTime);
    }

    printf("%-5s %-5s %-5s %-5s %-5s\n", "i", "s", "f", "w", "p");
    
    for (int i = 1; i <= n; i++)
    {
        printf("%-5d %-5d %-5d %-5d %-5d\n", i, arr[i].startTime, arr[i].endTime, arr[i].wt, P[i]);
    }

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == j)
            {
                if (i == 0)
                {
                    table[i][j] = 0;
                }
                
                else
                {
                    table[i][j] = table[i][i - 1];
                }
            }
            
            else
            {
                if (i > j)
                {
                    table[i][j] = max(table[P[i]][j] + arr[i].wt, table[i - 1][j]);
                }
                
                else
                {
                    table[i][j] = max(table[i][P[j]] + arr[j].wt, table[i][j - 1]);
                }
            }
        }
    }

    printf("\n");
    int s = 0;

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            printf("%-5d ", table[i][j]);
        }
        printf("\n");
    }

    int j;
    int i = j = n;
    while (1)
    {
        if (i == 0 && j == 0)
        {
            break;
        }

        if (i == j)
        {
            j--;
        }
        
        else if (i > j)
        {
            if (table[i][j] == table[P[i]][j] + arr[i].wt)
            {
                room1[room1CT++] = arr[i];
                s += arr[i].wt;
                i = P[i];
            }
            
            else
            {
                i--;
            }
        }
        
        else
        {
            if (table[i][j] == table[i][P[j]] + arr[j].wt)
            {
                room2[room2CT++] = arr[j];
                s += arr[j].wt;
                j = P[j];
            }
            
            else
            {
                j--;
            }
        }
    }

    printf("%d\n", room1CT);
    for (int i = room1CT - 1; i >= 0; i--)
    {

        printf("%d %d %d\n", room1[i].startTime, room1[i].endTime, room1[i].wt);
    }

    printf("%d\n", room2CT);

    for (int i = room2CT - 1; i >= 0; i--)
    {

        printf("%d %d %d\n", room2[i].startTime, room2[i].endTime, room2[i].wt);
    }

    return table[n][n];
}

int main()
{
    int n = 0;
    scanf("%d", &n);

    Work *runs = (Work *)malloc((n + 1) * sizeof(Work));
    
    runs[0].startTime = 0;
    runs[0].endTime = -999999999;
    runs[0].wt = 0;

    for (int i = 1; i <= n; i++)
    {
        scanf("%d %d %d", &runs[i].startTime, &runs[i].endTime, &runs[i].wt);
    }

    int maxWeight = MaxWeightedSched(runs, n);
    printf("%d\n", maxWeight);

    free(runs);
    return 0;
}