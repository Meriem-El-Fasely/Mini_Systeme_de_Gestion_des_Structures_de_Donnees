#include "pile_file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// --- Helpers ---

static PFNode* create_pf_node(const void *data, DataType type) {
    PFNode *node = malloc(sizeof(PFNode));
    if (!node) return NULL;

    size_t size = 0;
    if (type == TYPE_INT) size = sizeof(int);
    else if (type == TYPE_FLOAT) size = sizeof(float);
    else if (type == TYPE_CHAR) size = sizeof(char);
    else if (type == TYPE_STRING) size = MAX_STRING_LEN; // Fixed buffer for simplicity or strdup

    if (type == TYPE_STRING) {
        node->data = malloc(MAX_STRING_LEN);
        if (data) strncpy((char*)node->data, (char*)data, MAX_STRING_LEN);
        else memset(node->data, 0, MAX_STRING_LEN);
    } else {
        node->data = malloc(size);
        if (data) memcpy(node->data, data, size);
    }
    node->next = NULL;
    return node;
}

static void free_pf_node(PFNode *node) {
    if (node) {
        if (node->data) free(node->data);
        free(node);
    }
}

void pf_get_value_string(const void *data, DataType type, char *buffer, size_t buf_size) {
    if (!data) {
        snprintf(buffer, buf_size, "?");
        return;
    }
    if (type == TYPE_INT) snprintf(buffer, buf_size, "%d", *(int*)data);
    else if (type == TYPE_FLOAT) snprintf(buffer, buf_size, "%.2f", *(float*)data);
    else if (type == TYPE_CHAR) snprintf(buffer, buf_size, "%c", *(char*)data);
    else if (type == TYPE_STRING) snprintf(buffer, buf_size, "%s", (char*)data);
}

// --- Stack Implementation ---

void init_stack(Stack *s, DataType dtype, int max_cap) {
    s->top = NULL;
    s->data_type = dtype;
    s->size = 0;
    s->max_capacity = max_cap;
}

void reset_stack(Stack *s) {
    while (s->top) {
        PFNode *temp = s->top;
        s->top = s->top->next;
        free_pf_node(temp);
    }
    s->size = 0;
}

void destroy_stack(Stack *s) {
    reset_stack(s);
}

int stack_push(Stack *s, const void *data) {
    // If max_capacity == 0, unlimited (or manual limit not set)
    if (s->max_capacity > 0 && s->size >= s->max_capacity) return 0;

    PFNode *node = create_pf_node(data, s->data_type);
    if (!node) return 0;

    node->next = s->top;
    s->top = node;
    s->size++;
    return 1;
}

int stack_pop(Stack *s, void *out_data) {
    if (!s->top) return 0;

    PFNode *temp = s->top;
    if (out_data) {
        size_t size = 0;
        if (s->data_type == TYPE_INT) size = sizeof(int);
        else if (s->data_type == TYPE_FLOAT) size = sizeof(float);
        else if (s->data_type == TYPE_CHAR) size = sizeof(char);
        else if (s->data_type == TYPE_STRING) size = MAX_STRING_LEN;

        memcpy(out_data, temp->data, size);
    }

    s->top = temp->next;
    free_pf_node(temp);
    s->size--;
    return 1;
}

int stack_peek(const Stack *s, void *out_data) {
    if (!s->top) return 0;
    if (out_data) {
        size_t size = 0;
        if (s->data_type == TYPE_INT) size = sizeof(int);
        else if (s->data_type == TYPE_FLOAT) size = sizeof(float);
        else if (s->data_type == TYPE_CHAR) size = sizeof(char);
        else if (s->data_type == TYPE_STRING) size = MAX_STRING_LEN;
        memcpy(out_data, s->top->data, size);
    }
    return 1;
}

int stack_is_empty(const Stack *s) {
    return s->size == 0;
}

// --- Queue Implementation ---

void init_queue(Queue *q, DataType dtype, int max_cap) {
    q->front = NULL;
    q->rear = NULL;
    q->data_type = dtype;
    q->size = 0;
    q->max_capacity = max_cap;
}

void reset_queue(Queue *q) {
    while (q->front) {
        PFNode *temp = q->front;
        q->front = q->front->next;
        free_pf_node(temp);
    }
    q->rear = NULL;
    q->size = 0;
}

void destroy_queue(Queue *q) {
    reset_queue(q);
}

int queue_enqueue(Queue *q, const void *data) {
    if (q->max_capacity > 0 && q->size >= q->max_capacity) return 0;

    PFNode *node = create_pf_node(data, q->data_type);
    if (!node) return 0;

    if (q->rear) {
        q->rear->next = node;
    }
    q->rear = node;
    if (!q->front) {
        q->front = node;
    }
    q->size++;
    return 1;
}

int queue_dequeue(Queue *q, void *out_data) {
    if (!q->front) return 0;

    PFNode *temp = q->front;
    if (out_data) {
        size_t size = 0;
        if (q->data_type == TYPE_INT) size = sizeof(int);
        else if (q->data_type == TYPE_FLOAT) size = sizeof(float);
        else if (q->data_type == TYPE_CHAR) size = sizeof(char);
        else if (q->data_type == TYPE_STRING) size = MAX_STRING_LEN;
        memcpy(out_data, temp->data, size);
    }

    q->front = q->front->next;
    if (!q->front) {
        q->rear = NULL;
    }
    free_pf_node(temp);
    q->size--;
    return 1;
}

int queue_front(const Queue *q, void *out_data) {
    if (!q->front) return 0;
    if (out_data) {
        size_t size = 0;
        if (q->data_type == TYPE_INT) size = sizeof(int);
        else if (q->data_type == TYPE_FLOAT) size = sizeof(float);
        else if (q->data_type == TYPE_CHAR) size = sizeof(char);
        else if (q->data_type == TYPE_STRING) size = MAX_STRING_LEN;
        memcpy(out_data, q->front->data, size);
    }
    return 1;
}

int queue_is_empty(const Queue *q) {
    return q->size == 0;
}

// --- Random Gen ---

void stack_generate_random(Stack *s, int count) {
    reset_stack(s);
    for(int i=0; i<count; i++) {
        if (s->data_type == TYPE_INT) {
            int v = rand() % 100;
            stack_push(s, &v);
        } else if (s->data_type == TYPE_FLOAT) {
            float v = (float)(rand() % 1000) / 10.0f;
            stack_push(s, &v);
        } else if (s->data_type == TYPE_CHAR) {
            char v = 'A' + (rand() % 26);
            stack_push(s, &v);
        } else if (s->data_type == TYPE_STRING) {
            char buf[10];
            // Generate random string with only letters
            int len = 3 + rand() % 5;
            for(int k=0; k<len; k++) buf[k] = (rand()%2 == 0) ? 'A'+(rand()%26) : 'a'+(rand()%26);
            buf[len] = '\0';
            stack_push(s, buf);
        }
    }
}

void queue_generate_random(Queue *q, int count) {
    reset_queue(q);
    for(int i=0; i<count; i++) {
        if (q->data_type == TYPE_INT) {
            int v = rand() % 100;
            queue_enqueue(q, &v);
        } else if (q->data_type == TYPE_FLOAT) {
            float v = (float)(rand() % 1000) / 10.0f;
            queue_enqueue(q, &v);
        } else if (q->data_type == TYPE_CHAR) {
            char v = 'A' + (rand() % 26);
            queue_enqueue(q, &v);
        } else if (q->data_type == TYPE_STRING) {
            char buf[10];
             // Generate random string with only letters
            int len = 3 + rand() % 5;
            for(int k=0; k<len; k++) buf[k] = (rand()%2 == 0) ? 'A'+(rand()%26) : 'a'+(rand()%26);
            buf[len] = '\0';
            queue_enqueue(q, buf);
        }
    }
}

// --- Sorting ---

// Helper compare function for bubble sort
static int compare_pf_nodes(const void *a, const void *b, DataType type) {
    if (type == TYPE_INT) return (*(int*)a - *(int*)b);
    else if (type == TYPE_FLOAT) return (*(float*)a > *(float*)b) - (*(float*)a < *(float*)b);
    else if (type == TYPE_CHAR) return (*(char*)a - *(char*)b);
    else if (type == TYPE_STRING) return strcmp((char*)a, (char*)b);
    return 0;
}

void sort_stack(Stack *s) {
    if (!s || s->size < 2) return;

    int n = s->size;
    size_t sz = 0;
    if(s->data_type == TYPE_INT) sz=sizeof(int);
    else if(s->data_type == TYPE_FLOAT) sz=sizeof(float);
    else if(s->data_type == TYPE_CHAR) sz=sizeof(char);
    else if(s->data_type == TYPE_STRING) sz=MAX_STRING_LEN;

    void *arr = malloc(n * sz);
    if(!arr) return;

    for(int i=0; i<n; i++) stack_pop(s, (char*)arr + i*sz);

    // Bubble Sort
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            void *p1 = (char*)arr + j*sz;
            void *p2 = (char*)arr + (j+1)*sz;
            if (compare_pf_nodes(p1, p2, s->data_type) > 0) {
                 char temp[MAX_STRING_LEN];
                 memcpy(temp, p1, sz);
                 memcpy(p1, p2, sz);
                 memcpy(p2, temp, sz);
            }
        }
    }

    // Push back (Descending loop for correct stack order)
    for(int i=n-1; i>=0; i--) stack_push(s, (char*)arr + i*sz);
    free(arr);
}

void sort_queue(Queue *q) {
    if (!q || q->size < 2) return;

    int n = q->size;
    size_t sz = 0;
    if(q->data_type == TYPE_INT) sz=sizeof(int);
    else if(q->data_type == TYPE_FLOAT) sz=sizeof(float);
    else if(q->data_type == TYPE_CHAR) sz=sizeof(char);
    else if(q->data_type == TYPE_STRING) sz=MAX_STRING_LEN;

    void *arr = malloc(n * sz);
    for(int i=0; i<n; i++) queue_dequeue(q, (char*)arr + i*sz);

    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            void *p1 = (char*)arr + j*sz;
            void *p2 = (char*)arr + (j+1)*sz;
             if (compare_pf_nodes(p1, p2, q->data_type) > 0) {
                 char temp[MAX_STRING_LEN];
                 memcpy(temp, p1, sz);
                 memcpy(p1, p2, sz);
                 memcpy(p2, temp, sz);
            }
        }
    }

    for(int i=0; i<n; i++) queue_enqueue(q, (char*)arr + i*sz);
    free(arr);
}
