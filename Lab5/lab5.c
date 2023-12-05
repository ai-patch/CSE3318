#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define TABLE_SIZE_FACTOR .4615

struct edge
{
    int tail, head, type;
};
typedef struct edge edgeType;

struct vertex
{
    char name[26];
    int number;
    int firstEdgeIndex;
};
typedef struct vertex vertexType;

edgeType *edgeTab;
vertexType *vertexTab;

int n;
int e;

int *vertexStatus, *secondDFSrestarts;
int finishIndex;

int tailThenHead(const void *xin, const void *yin)
{
    int result;
    edgeType *x, *y;

    x = (edgeType *)xin;
    y = (edgeType *)yin;
    result = x->tail - y->tail;
    if (result != 0)
        return result;
    else
        return x->head - y->head;
}

bool isPrime(int n)
{
    if (n <= 1)
    {
        return false;
    }
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int getNextPrime(int n)
{
    while (!isPrime(n))
    {
        n++;
    }
    return n;
}

unsigned int str2int(unsigned char *str)
{
    int str_len = strlen(str);
    unsigned int str_int = 0;
    int counter = 0;

    while (counter < str_len)
    {
        str_int += str[counter];
        counter++;
    }
    return str_int;
}

void read_input_file()
{
    int i, j;
    char a[26], b[26];

    scanf("%d %d", &n, &e);
    edgeTab = (edgeType *)malloc(e * sizeof(edgeType));
    vertexTab = (vertexType *)malloc(n * sizeof(vertexType));
    if (!edgeTab || !vertexTab)
    {
        printf("malloc failed %d\n", __LINE__);
        exit(0);
    }

    for (i = 0; i < n; i++)
    {
        vertexTab[i].number = -1;
        vertexTab[i].firstEdgeIndex = -1;
    }

    for (i = 0; i < e; i++)
    {
        scanf("%s %s", a, b);

        int tailIndex = -1;
        for (j = 0; j < n; j++)
        {
            if (vertexTab[j].number == -1 || strcmp(vertexTab[j].name, a) == 0)
            {
                if (vertexTab[j].number == -1)
                {
                    vertexTab[j].number = j;
                    strcpy(vertexTab[j].name, a);
                }
                tailIndex = j;
                break;
            }
        }

        int headIndex = -1;
        for (j = 0; j < n; j++)
        {
            if (vertexTab[j].number == -1 || strcmp(vertexTab[j].name, b) == 0)
            {
                if (vertexTab[j].number == -1)
                {
                    vertexTab[j].number = j;
                    strcpy(vertexTab[j].name, b);
                }
                headIndex = j;
                break;
            }
        }

        edgeTab[i].tail = tailIndex;
        edgeTab[i].head = headIndex;
        edgeTab[i].type = 0;

        if (vertexTab[tailIndex].firstEdgeIndex == -1)
        {
            vertexTab[tailIndex].firstEdgeIndex = i;
        }
    }
}

void DFSvisit(int u)
{
    int i, v;

    vertexStatus[u] = GRAY;

    for (i = vertexTab[u].firstEdgeIndex; i < e && edgeTab[i].tail == u; i++)
    {
        v = edgeTab[i].head;
        if (vertexStatus[v] == WHITE)
            DFSvisit(v);
    }
    vertexStatus[u] = BLACK;
    secondDFSrestarts[--finishIndex] = u;
}

void reverseEdges()
{
    int i, a, b;

    for (i = 0; i < e; i++)
    {
        a = edgeTab[i].tail;
        b = edgeTab[i].head;
        edgeTab[i].tail = b;
        edgeTab[i].head = a;
    }

    qsort(edgeTab, e, sizeof(edgeType), tailThenHead);

    for (i = 0; i < n; i++)
    {
        vertexTab[i].firstEdgeIndex = -1;
    }

    int j = 0;
    for (i = 0; i < n; i++)
    {
        vertexTab[i].firstEdgeIndex = j;
        for (; j < e && edgeTab[j].tail == i; j++)
            ;
    }
}

void DFSvisit2(int u)
{
    int i, v;

    printf("%s\n", vertexTab[u].name);
    vertexStatus[u] = GRAY;

    for (i = vertexTab[u].firstEdgeIndex; i < e && edgeTab[i].tail == u; i++)
    {
        v = edgeTab[i].head;
        if (vertexStatus[v] == WHITE)
            DFSvisit2(v);
    }
    vertexStatus[u] = BLACK;
}

int main()
{
    int i, j, k, nextDFS;
    int SCCcount = 0;
    int h1, h2;
    int probes = 0;

    read_input_file();

    vertexStatus = (int *)malloc(n * sizeof(int));
    secondDFSrestarts = (int *)malloc(n * sizeof(int));
    if (!vertexStatus || !secondDFSrestarts)
    {
        printf("malloc failed\n");
        exit(0);
    }

    for (i = 0; i < n; i++)
        vertexStatus[i] = WHITE;
    finishIndex = n;
    for (i = 0; i < n; i++)
        if (vertexStatus[i] == WHITE)
            DFSvisit(i);

    reverseEdges();

    int initialSize = n / TABLE_SIZE_FACTOR;
    int hashTableSize = getNextPrime(initialSize);
    printf("Double hash table size is: %d\n", hashTableSize);
    printf("Names in order of first appearance:\n");
    for (i = 0; i < n; i++)
    {
        printf("%d %s\n", i, vertexTab[i].name);
    }
    printf("Double hash table used while processing input: \n");

    int *hashTableOccupied = (int *)calloc(hashTableSize, sizeof(int));

    for (int i = 0; i < n; i++)
    {
        unsigned int asciiSum = str2int(vertexTab[i].name);

        h1 = asciiSum % hashTableSize;
        h2 = asciiSum % (hashTableSize - 1) + 1;

        while (hashTableOccupied[h1] != 0)
        {
            h1 = (h1 + h2) % hashTableSize;
            probes++;
        }

        printf("%d %d (%s)\n", h1, i, vertexTab[i].name);
        hashTableOccupied[h1] = 1;
    }

    free(hashTableOccupied);

    printf("Total probes: %d\n", probes);
    for (i = 0; i < n; i++)
        vertexStatus[i] = WHITE;
    for (i = 0; i < n; i++)
        if (vertexStatus[secondDFSrestarts[i]] == WHITE)
        {
            SCCcount++;
            printf("SCC %d\n", SCCcount);
            DFSvisit2(secondDFSrestarts[i]);
        }

    free(edgeTab);
    free(vertexTab);
    free(vertexStatus);
    free(secondDFSrestarts);
    return 0;
}
