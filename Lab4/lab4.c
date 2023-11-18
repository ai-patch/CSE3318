#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Node 
{
    int data;
    int length;
    struct Node* next;
};

struct Stack
{
    struct Node* top;
    int minLen;
    int maxLen;
};

struct Queue 
{
    struct Stack* inStack;
    struct Stack* outStack;
    int totalLength;
    int numMessages;
};

void initStack(struct Stack* stack) 
{
    stack->top = NULL;
    stack->minLen = INT_MAX;
    stack->maxLen = INT_MIN;
}

void initQueue(struct Queue* queue)
{
    queue->inStack = (struct Stack*)malloc(sizeof(struct Stack));
    queue->outStack = (struct Stack*)malloc(sizeof(struct Stack));
    queue->totalLength = 0;
    queue->numMessages = 0;

    initStack(queue->inStack);
    initStack(queue->outStack);
}

void push(struct Stack* stack, int value, int length) 
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->length = length;
    newNode->next = stack->top;

    stack->top = newNode;
    if (length < stack->minLen)
    {
        stack->minLen = length;
    }
    if (length > stack->maxLen)
    {
        stack->maxLen = length;
    }
}

struct Node* pop(struct Stack* stack)
{
    if (stack->top == NULL)
    {
        return NULL;
    }

    struct Node* temp = stack->top;
    stack->top = stack->top->next;
    return temp;
}

void enqueue(struct Queue* queue, int length)
{
    int value;
    push(queue->inStack, value, length);
    queue->totalLength += length;
    queue->numMessages++;
}

struct Node* dequeue(struct Queue* queue)
{
    if (queue->outStack->top == NULL) 
    {
        while (queue->inStack->top != NULL)
        {
            struct Node* temp = pop(queue->inStack);
            push(queue->outStack, temp->data, temp->length);
            free(temp);
        }
    }

    if (queue->outStack->top == NULL)
    {
        return NULL;
    }

    struct Node* temp = pop(queue->outStack);
    queue->totalLength -= temp->length;
    queue->numMessages--;

    return temp;
}

void computeAverage(struct Queue* queue)
{
    if (queue->numMessages > 0)
    {
        double average = (double)queue->totalLength / queue->numMessages;
        printf("Average message length: %.2f\n", average);
    }
    else
    {
        printf("Error: Cannot Compute. Queue is empty\n");
    }
}

void printMinLength(struct Queue* queue)
{
    int minLen = INT_MAX;

    struct Node* current = queue->inStack->top;
    while (current != NULL)
    {
        if (current->length < minLen)
        {
            minLen = current->length;
        }
        current = current->next;
    }

    current = queue->outStack->top;
    while (current != NULL)
    {
        if (current->length < minLen)
        {
            minLen = current->length;
        }
        current = current->next;
    }

    if (queue->numMessages > 0)
    {
        printf("Minimum length: %d\n", minLen);
    }
    else
    {
        printf("Error: Cannot Compute. Queue is empty\n");
    }
}

void printMaxLength(struct Queue* queue)
{
    int maxLen = INT_MIN;

    struct Node* current = queue->inStack->top;
    while (current != NULL)
    {
        if (current->length > maxLen)
        {
            maxLen = current->length;
        }
        current = current->next;
    }

    current = queue->outStack->top;
    while (current != NULL)
    {
        if (current->length > maxLen)
        {
            maxLen = current->length;
        }
        current = current->next;
    }

    if (queue->numMessages > 0)
    {
        printf("Maximum length: %d\n", maxLen);
    }
    else
    {
        printf("Error: Cannot Compute. Queue is empty\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    struct Queue myQueue;
    initQueue(&myQueue);

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int command, value;

    while (fscanf(file, "%d", &command) == 1)
    {
        switch (command)
        {
        case 0:
            printf("Exiting program\n");
            fclose(file);
            return 0;

        case 1:
            if (fscanf(file, "%d", &value) == 1)
            {
                enqueue(&myQueue, value);
                printf("%d has been enqueued\n", value);
            }
            else
            {
                printf("Invalid command format\n");
            }
            break;

        case 2:
        {
            struct Node* node = dequeue(&myQueue);
            if (node != NULL)
            {
                printf("Dequeued length: %d\n", node->length);
                free(node);
            }
            else
            {
                printf("Error: Queue is empty\n");
            }
        }
        break;

        case 3:
            computeAverage(&myQueue);
            break;

        case 4:
            printMinLength(&myQueue);
            break;

        case 5:
            printMaxLength(&myQueue);
            break;

        default:
                printf("Invalid command\n");
        }
    }

    fclose(file);
    return 0;
}
