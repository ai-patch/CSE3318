#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Node structure for a stack
struct Node {
    int data;
    int length; // Additional field for message length
    struct Node* next;
};

// Stack structure
struct Stack {
    struct Node* top;
    int minLen; // Minimum message length
    int maxLen; // Maximum message length
};

// Queue structure
struct Queue {
    struct Stack* inStack;
    struct Stack* outStack;
    int totalLength; // Total sum of all message lengths
    int numMessages; // Number of messages
};

// Function to initialize a stack
void initStack(struct Stack* stack) {
    stack->top = NULL;
    stack->minLen = INT_MAX;
    stack->maxLen = INT_MIN;
}

// Function to initialize a queue
void initQueue(struct Queue* queue) {
    queue->inStack = (struct Stack*)malloc(sizeof(struct Stack));
    queue->outStack = (struct Stack*)malloc(sizeof(struct Stack));
    queue->totalLength = 0;
    queue->numMessages = 0;

    initStack(queue->inStack);
    initStack(queue->outStack);
}

// Function to push a value onto the stack
void push(struct Stack* stack, int value, int length) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->length = length;
    newNode->next = stack->top;

    stack->top = newNode;
    if (length < stack->minLen) {
        stack->minLen = length;
    }
    if (length > stack->maxLen) {
        stack->maxLen = length;
    }
}

// Function to pop a value from the stack
struct Node* pop(struct Stack* stack) {
    if (stack->top == NULL) {
        return NULL; // Stack is empty
    }

    struct Node* temp = stack->top;
    stack->top = stack->top->next;
    return temp;
}

// Function to enqueue a message
void enqueue(struct Queue* queue, int length) {
    int value; // You need to decide how to get the value to enqueue
    push(queue->inStack, value, length);
    queue->totalLength += length;
    queue->numMessages++;
}

// Function to dequeue a message
struct Node* dequeue(struct Queue* queue) {
    if (queue->outStack->top == NULL) {
        // If outStack is empty, transfer messages from inStack
        while (queue->inStack->top != NULL) {
            struct Node* temp = pop(queue->inStack);
            push(queue->outStack, temp->data, temp->length);
            free(temp);
        }
    }

    if (queue->outStack->top == NULL) {
        return NULL; // Queue is empty
    }

    struct Node* temp = pop(queue->outStack);
    queue->totalLength -= temp->length;
    queue->numMessages--;

    return temp;
}

// Function to compute and print the average length of a message
void computeAverage(struct Queue* queue) {
    if (queue->numMessages > 0) {
        double average = (double)queue->totalLength / queue->numMessages;
        printf("Average message length: %.2f\n", average);
    } else {
        printf("Error: Queue is empty\n");
    }
}

// Function to determine and print the minimum message length
void printMinLength(struct Queue* queue) {
    if (queue->numMessages > 0) {
        printf("Minimum message length: %d\n", queue->outStack->minLen);
    } else {
        printf("Error: Queue is empty\n");
    }
}

// Function to determine and print the maximum message length
void printMaxLength(struct Queue* queue) {
    if (queue->numMessages > 0) {
        printf("Maximum message length: %d\n", queue->outStack->maxLen);
    } else {
        printf("Error: Queue is empty\n");
    }
}
int main() {
    struct Queue myQueue;
    initQueue(&myQueue);

    FILE *file = fopen("a.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int command, value;

    while (fscanf(file, "%d", &command) == 1) {
        switch (command) {
            case 0:
                printf("Exiting program\n");
                fclose(file);
                return 0;

            case 1:
                if (fscanf(file, "%d", &value) == 1) {
                    enqueue(&myQueue, value);
                    printf("%d has been enqueued\n", value);
                } else {
                    printf("Invalid command format\n");
                }
                break;

            case 2:
                // Dequeue and print the length
                {
                    struct Node* node = dequeue(&myQueue);
                    if (node != NULL) {
                        printf("Dequeued length: %d\n", node->length);
                        free(node);
                    } else {
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
