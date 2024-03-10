#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Allan Jean-Jacques
Movie line Assignment
COP 3502
3/5/24
*/

// Struct to store information about a customer
typedef struct 
{
    char name[51];
    int num_tickets;
    int line_number;
    int arrival_time;
    int checkout_time;
} Customer;

// Struct to represent a node in a linked list
typedef struct Node 
{
    Customer *customer;
    struct Node *next;
} Node;

// Struct to represent a queue
typedef struct 
{
    Node *front;
    Node *rear;
    int size;
} Queue;

// Function to create a customer
Customer *create_customer(char *name, int num_tickets, int line_number, int arrival_time) 
{
    Customer *customer = (Customer *)malloc(sizeof(Customer));
    if (customer == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(customer->name, name);
    customer->num_tickets = num_tickets;
    customer->line_number = line_number;
    customer->arrival_time = arrival_time;
    customer->checkout_time = -1; // Initialize to -1, meaning not yet checked out
    return customer;
}

// Function to create a new node
Node *create_node(Customer *customer) 
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->customer = customer;
    node->next = NULL;
    return node;
}

// Function to create a new empty queue
Queue *create_queue() 
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Function to enqueue a customer into the queue
void enqueue(Queue *queue, Customer *customer) 
{
    Node *node = create_node(customer);
    if (queue->rear == NULL) 
    {
        queue->front = node;
        queue->rear = node;
    } 
    else 
    {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->size++;
}

// Function to dequeue a customer from the queue
Customer *dequeue(Queue *queue) 
{
    if (queue->front == NULL) 
    {
        return NULL;
    }
    Node *temp = queue->front;
    Customer *customer = temp->customer;
    queue->front = queue->front->next;
    if (queue->front == NULL) 
    {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return customer;
}

// Function to check if the queue is empty
int is_empty(Queue *queue) 
{
    return queue->front == NULL;
}

// Function to return the size of the queue
int get_size(Queue *queue) 
{
    return queue->size;
}

// Main function
int main() 
{
    int n, b;
    scanf("%d %d", &n, &b);

    // Create an array of queues, one for each booth
    Queue *booths[b];
    for (int i = 0; i < b; i++) 
    {
        booths[i] = create_queue();
    }

    // Read customer information and enqueue them into the appropriate queue
    for (int i = 0; i < n; i++) 
    {
        char name[51];
        int num_tickets, arrival_time;
        scanf("%s %d %d", name, &num_tickets, &arrival_time);

        // Determine the queue number based on the given rules
        int line_number = (name[0] - 'A') % 13;
        if (line_number == 0) 
        {
            // Find the queue with the smallest size
            int smallest_size = get_size(booths[0]);
            int smallest_index = 0;
            for (int j = 1; j < b; j++) 
            {
                if (get_size(booths[j]) < smallest_size) 
                {
                    smallest_size = get_size(booths[j]);
                    smallest_index = j;
                }
            }
            line_number = smallest_index + 1; // Queue numbers are 1-based
        }

        // Create a new customer and enqueue them into the appropriate queue
        Customer *customer = create_customer(name, num_tickets, line_number, arrival_time);
        enqueue(booths[line_number - 1], customer);
    }

    // Simulate the process of serving customers at each booth
    for (int i = 0; i < b; i++) 
    {
        printf("Booth %d\n", i + 1);

        while (!is_empty(booths[i])) 
        {
            Customer *customer = dequeue(booths[i]);
            // Calculate the checkout time
            int processing_time = 30 + customer->num_tickets * 5;
            if (customer->arrival_time > customer->checkout_time) 
            {
                customer->checkout_time = customer->arrival_time + processing_time;
            } 
            else 
            {
                customer->checkout_time += processing_time;
            }
            // Print the checkout time
            printf("%s from line %d checks out at time %d.\n", customer->name, customer->line_number,
                   customer->checkout_time);
            free(customer); 
        }
        printf("\n");
    }

    // Free memory for queues
    for (int i = 0; i < b; i++) 
    {
        free(booths[i]);
    }

    return 0;
}