#ifndef PILE_FILE_H
#define PILE_FILE_H

#include "types.h"
#include <gtk/gtk.h>
#include <cairo.h>

typedef enum {
    STRUCT_STACK,
    STRUCT_QUEUE
} StructureType;



typedef struct PFNode {
    void *data;
    struct PFNode *next;
} PFNode;

typedef struct {
    PFNode *top;
    DataType data_type;
    int size;
    int max_capacity;
} Stack;

typedef struct {
    PFNode *front;
    PFNode *rear;
    DataType data_type;
    int size;
    int max_capacity;
} Queue;

// --- Init & Destroy ---
void init_stack(Stack *s, DataType dtype, int max_cap);
void destroy_stack(Stack *s);
void reset_stack(Stack *s); // Clears content but keeps config

void init_queue(Queue *q, DataType dtype, int max_cap);
void destroy_queue(Queue *q);
void reset_queue(Queue *q);

// --- Operations Stack ---
// Returns 1 if success, 0 if failure (e.g., full)
int stack_push(Stack *s, const void *data);
// Returns 1 if success, 0 if failure (e.g., empty). Data is copied to *out_data if not NULL.
int stack_pop(Stack *s, void *out_data);
int stack_peek(const Stack *s, void *out_data);
int stack_is_empty(const Stack *s);

// --- Operations Queue ---
int queue_enqueue(Queue *q, const void *data);
int queue_dequeue(Queue *q, void *out_data);
int queue_front(const Queue *q, void *out_data);
int queue_is_empty(const Queue *q);

// --- Random Generation ---
void stack_generate_random(Stack *s, int count);
void queue_generate_random(Queue *q, int count);

// --- Sorting ---
void sort_stack(Stack *s);
void sort_queue(Queue *q);

// --- Visuals ---
// Helper to get formatted string for display
void pf_get_value_string(const void *data, DataType type, char *buffer, size_t buf_size);

#endif
