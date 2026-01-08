#include "liste.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constantes de dessin
#define NODE_WIDTH 80
#define NODE_HEIGHT 40
#define SPACING_X 100
#define TEXT_OFFSET_Y 25
#define ARROW_LENGTH 10
#define PADDING 20

// ===============================================
// --- Fonctions utilitaires ---
// ===============================================

/**
 * @brief Récupère la taille d'un type de donnée.
 */
size_t get_list_element_size(DataType type) {
    switch (type) {
        case TYPE_INT: return sizeof(int);
        case TYPE_FLOAT: return sizeof(float);
        case TYPE_CHAR: return sizeof(char);
        case TYPE_STRING: return MAX_STRING_LEN;
    }
    return 0;
}

/**
 * @brief Fonction de comparaison générique pour les nœuds de liste.
 */
int list_generic_compare(const void *a, const void *b, DataType type) {
    switch (type) {
        case TYPE_INT: {
            int val_a = *(const int *)a;
            int val_b = *(const int *)b;
            return (val_a > val_b) - (val_a < val_b);
        }
        case TYPE_FLOAT: {
            float val_a = *(const float *)a;
            float val_b = *(const float *)b;
            if (val_a > val_b) return 1;
            if (val_a < val_b) return -1;
            return 0;
        }
        case TYPE_CHAR: {
            char val_a = *(const char *)a;
            char val_b = *(const char *)b;
            return (val_a > val_b) - (val_a < val_b);
        }
        case TYPE_STRING: {
            return strcmp((const char *)a, (const char *)b);
        }
    }
    return 0;
}

/**
 * @brief Initialise une nouvelle structure de Liste.
 */
void init_list(List *list, ListType type, DataType data_type) {
    if (list) {
        list->head = NULL;
        list->list_type = type;
        list->data_type = data_type;
        list->dimension = 0;
    }
}

/**
 * @brief Crée un nouveau nœud avec des données allouées.
 */
Node *create_node(const void *data, DataType data_type) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le nœud.\n");
        return NULL;
    }

    size_t data_size = get_list_element_size(data_type);
    new_node->data = malloc(data_size);
    if (!new_node->data) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les données.\n");
        free(new_node);
        return NULL;
    }

    memcpy(new_node->data, data, data_size);

    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

/**
 * @brief Libère la mémoire d'un nœud, y compris ses données.
 */
void free_node(Node *node, DataType data_type) {
    if (node) {
        if (node->data) {
            free(node->data);
        }
        free(node);
    }
}

/**
 * @brief Libère toute la mémoire de la liste.
 */
void destroy_list(List *list) {
    if (!list) return;

    Node *current = list->head;
    Node *next;

    while (current) {
        next = current->next;
        free_node(current, list->data_type);
        current = next;
    }
    list->head = NULL;
    list->dimension = 0;
}

/**
 * @brief Génère des données aléatoires pour remplir la liste.
 */
void generate_random_list_data(List *list, int count) {
    if (!list) return;

    destroy_list(list); // Ensure clean start

    srand(time(NULL));

    if (list->data_type == TYPE_INT) {
        // Unique Integers (Fisher-Yates)
        int max_val = 1000;
        if (count > max_val) max_val = count * 2; // Expand pool if needed
        int *pool = malloc(max_val * sizeof(int));
        for (int i = 0; i < max_val; i++) pool[i] = i;

        for (int i = 0; i < count; i++) {
            int r = i + rand() % (max_val - i);
            int temp = pool[i];
            pool[i] = pool[r];
            pool[r] = temp;

            insert_at_end(list, &pool[i]);
        }
        free(pool);
    }
    else {
        // Other types
        for (int i = 0; i < count; i++) {
            switch (list->data_type) {
                case TYPE_FLOAT: {
                    float data = (float)rand() / (float)(RAND_MAX / 100.0);
                    insert_at_end(list, &data);
                    break;
                }
                case TYPE_CHAR: {
                    char data = (char)(65 + rand() % 26);
                    insert_at_end(list, &data);
                    break;
                }
                case TYPE_STRING: {
                    char data[MAX_STRING_LEN];
                    const char *alpha = "abcdefghijklmnopqrstuvwxyz";
                    int len = 3 + rand() % 5;
                    for (int j = 0; j < len; j++) {
                        data[j] = alpha[rand() % 26];
                    }
                    data[len] = '\0';
                    insert_at_end(list, data);
                    break;
                }
                default: break;
            }
        }
    }
}


// ===============================================
// --- Opérations de Liste ---
// ===============================================

/**
 * @brief Insère un nœud au début de la liste.
 */
void insert_at_beginning(List *list, const void *data) {
    Node *new_node = create_node(data, list->data_type);
    if (!new_node) return;

    new_node->next = list->head;

    if (list->head != NULL) {
        if (list->list_type == LISTE_DOUBLE) {
            list->head->prev = new_node;
        }
    }

    list->head = new_node;
    list->dimension++;
}

/**
 * @brief Insère un nœud à la fin de la liste.
 */
void insert_at_end(List *list, const void *data) {
    Node *new_node = create_node(data, list->data_type);
    if (!new_node) return;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        if (list->list_type == LISTE_DOUBLE) {
            new_node->prev = current;
        }
    }
    list->dimension++;
}

/**
 * @brief Insère un nœud à une position donnée.
 */
int insert_at_position(List *list, const void *data, int position) {
    if (position < 0 || position > list->dimension) return -1;

    if (position == 0) {
        insert_at_beginning(list, data);
        return 0;
    }

    if (position == list->dimension) {
        insert_at_end(list, data);
        return 0;
    }

    Node *new_node = create_node(data, list->data_type);
    if (!new_node) return -1;

    Node *current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }

    new_node->next = current->next;

    if (list->list_type == LISTE_DOUBLE) {
        new_node->prev = current;
        if (current->next) {
            current->next->prev = new_node;
        }
    }

    current->next = new_node;
    list->dimension++;
    return 0;
}

/**
 * @brief Insère un nœud à la position triée (suppose l'ordre croissant).
 */
void insert_sorted(List *list, const void *data) {
    if (!list) return;
    if (list->head == NULL) {
        insert_at_beginning(list, data);
        return;
    }

    // Trouver la position d'insertion
    Node *current = list->head;
    int pos = 0;
    while(current != NULL) {
        if(list_generic_compare(data, current->data, list->data_type) < 0) {
            break;
        }
        current = current->next;
        pos++;
    }
    insert_at_position(list, data, pos);
}

/**
 * @brief Supprime un nœud à une position donnée.
 */
int delete_at_position(List *list, int position) {
    if (position < 0 || position >= list->dimension) return -1;

    Node *to_delete = NULL;

    if (position == 0) {
        to_delete = list->head;
        list->head = list->head->next;
        if (list->head && list->list_type == LISTE_DOUBLE) {
            list->head->prev = NULL;
        }
    } else {
        Node *current = list->head;
        for (int i = 0; i < position - 1; i++) {
            current = current->next;
        }

        to_delete = current->next;
        current->next = to_delete->next;

        if (list->list_type == LISTE_DOUBLE && to_delete->next) {
            to_delete->next->prev = current;
        }
    }

    free_node(to_delete, list->data_type);
    list->dimension--;
    return 0;
}

/**
 * @brief Modifie les données d'un nœud à une position donnée.
 */
int modify_at_position(List *list, const void *new_data, int position) {
    if (position < 0 || position >= list->dimension) return -1;

    Node *current = list->head;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }

    size_t data_size = get_list_element_size(list->data_type);
    memcpy(current->data, new_data, data_size);

    return 0;
}

/**
 * @brief Supprime la première occurrence d'un nœud contenant la valeur donnée.
 */
int delete_by_value(List *list, const void *val) {
    if (!list || !list->head) return -1;

    Node *current = list->head;
    int pos = 0;
    while(current != NULL) {
        if(list_generic_compare(current->data, val, list->data_type) == 0) {
            // Found
            return delete_at_position(list, pos);
        }
        current = current->next;
        pos++;
    }
    return -1; // Not found
}

/**
 * @brief Modifie la première occurrence d'un nœud contenant old_val par new_val.
 */
int modify_by_value(List *list, const void *old_val, const void *new_val) {
    if (!list || !list->head) return -1;

    Node *current = list->head;
    size_t dsize = get_list_element_size(list->data_type);

    while(current != NULL) {
        if(list_generic_compare(current->data, old_val, list->data_type) == 0) {
             // Found, replace data
             memcpy(current->data, new_val, dsize);
             return 0;
        }
        current = current->next;
    }
    return -1; // Not found
}

/**
 * @brief Trie la liste chaînée (Tri par Insertion).
 */
void sort_linked_list(List *list) {
    if (!list || list->dimension < 2) return;

    Node *sorted = NULL;
    Node *current = list->head;

    while (current != NULL) {
        Node *next = current->next;

        // Isoler le nœud courant
        current->next = NULL;
        if (list->list_type == LISTE_DOUBLE) current->prev = NULL;

        // Insérer dans la liste triée
        if (sorted == NULL || list_generic_compare(current->data, sorted->data, list->data_type) < 0) {
            current->next = sorted;
            if (sorted && list->list_type == LISTE_DOUBLE) sorted->prev = current;
            sorted = current;
        } else {
            Node *temp = sorted;
            while (temp->next != NULL && list_generic_compare(current->data, temp->next->data, list->data_type) >= 0) {
                temp = temp->next;
            }

            current->next = temp->next;
            if (current->next && list->list_type == LISTE_DOUBLE) current->next->prev = current;
            if (list->list_type == LISTE_DOUBLE) current->prev = temp;
            temp->next = current;
        }

        current = next;
    }

}

/**
 * @brief Trie la liste chaînée (Tri à Bulle).
 */
void sort_list_bubble(List *list) {
    if (!list || list->head == NULL || list->dimension < 2) return;

    int swapped;
    Node *ptr1;
    Node *lptr = NULL;

    size_t data_size = get_list_element_size(list->data_type);
    void *temp = malloc(data_size);
    if (!temp) return;

    do {
        swapped = 0;
        ptr1 = list->head;

        while (ptr1->next != lptr) {
            if (list_generic_compare(ptr1->data, ptr1->next->data, list->data_type) > 0) {
                // Swap data
                memcpy(temp, ptr1->data, data_size);
                memcpy(ptr1->data, ptr1->next->data, data_size);
                memcpy(ptr1->next->data, temp, data_size);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    free(temp);
}

// Helper for swapping data
static void swap_data(Node *a, Node *b, size_t size) {
    if(a==b) return;
    void *temp = malloc(size);
    memcpy(temp, a->data, size);
    memcpy(a->data, b->data, size);
    memcpy(b->data, temp, size);
    free(temp);
}

/**
 * @brief Trie la liste chaîne (Shell Sort - Data Swap).
 */
void sort_list_shell(List *list) {
    if (!list || list->dimension < 2) return;
    int n = list->dimension;
    size_t dsize = get_list_element_size(list->data_type);

    Node **nodes = malloc(n * sizeof(Node*));
    Node *curr = list->head;
    for(int i=0; i<n; i++) { nodes[i] = curr; curr = curr->next; }

    for (int gap = n/2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
             void *temp = malloc(dsize);
             memcpy(temp, nodes[i]->data, dsize);

             int j;
             for (j = i; j >= gap && list_generic_compare(nodes[j - gap]->data, temp, list->data_type) > 0; j -= gap) {
                 memcpy(nodes[j]->data, nodes[j - gap]->data, dsize);
             }
             memcpy(nodes[j]->data, temp, dsize);
             free(temp);
        }
    }
    free(nodes);
}

// Partition helper for QuickSort
static int partition(Node **nodes, int low, int high, DataType type, size_t size) {
    void *pivot = nodes[high]->data; // Pivot data
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (list_generic_compare(nodes[j]->data, pivot, type) < 0) {
            i++;
            swap_data(nodes[i], nodes[j], size);
        }
    }
    swap_data(nodes[i + 1], nodes[high], size);
    return (i + 1);
}

static void quickSort(Node **nodes, int low, int high, DataType type, size_t size) {
    if (low < high) {
        int pi = partition(nodes, low, high, type, size);
        quickSort(nodes, low, pi - 1, type, size);
        quickSort(nodes, pi + 1, high, type, size);
    }
}

/**
 * @brief Trie la liste chaîne (Quick Sort - Data Swap).
 */
void sort_list_quick(List *list) {
    if (!list || list->dimension < 2) return;
    int n = list->dimension;
    size_t dsize = get_list_element_size(list->data_type);

    Node **nodes = malloc(n * sizeof(Node*));
    Node *curr = list->head;
    for(int i=0; i<n; i++) { nodes[i] = curr; curr = curr->next; }

    quickSort(nodes, 0, n-1, list->data_type, dsize);

    free(nodes);
}


// ===============================================
// --- Fonction de Dessin (Cairo) ---
// ===============================================

/**
 * @brief Convertit les données d'un nœud en chaîne de caractères pour l'affichage.
 */
static void data_to_string(const void *data, DataType type, char *buffer, size_t buffer_size) {
    switch (type) {
        case TYPE_INT:
            snprintf(buffer, buffer_size, "%d", *(const int *)data);
            break;
        case TYPE_FLOAT:
            snprintf(buffer, buffer_size, "%.2f", *(const float *)data);
            break;
        case TYPE_CHAR:
            snprintf(buffer, buffer_size, "%c", *(const char *)data);
            break;
        case TYPE_STRING:
            snprintf(buffer, buffer_size, "%s", (const char *)data);
            break;
    }
}

/**
 * @brief Dessine la liste chaînée (simple ou double).
 *
 *
 */
gboolean draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer data) {
    List *list = (List *)data;
    if (!list || list->head == NULL) {
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, PADDING, PADDING + 10);
        cairo_show_text(cr, "La liste est vide.");
        return FALSE;
    }

    // guint height = gtk_widget_get_allocated_height(widget);
    guint width = gtk_widget_get_allocated_width(widget);

    // Position initiale
    double current_x = 20; // PADDING
    double current_y = 60; // Start lower to allow indices
    double row_height = NODE_HEIGHT + 60; // Space for next row

    char buffer[MAX_STRING_LEN];
    Node *current = list->head;
    int index = 0;

    while (current != NULL) {
        // --- Wrap if needed ---
        if (current_x + NODE_WIDTH + 40 > width) { // Increased margin
             current_x = 20;
             current_y += row_height;
        }

        double center_y = current_y + NODE_HEIGHT / 2.0;


        index++;

        // --- 1. Dessiner le nœud (boîte) ---
        cairo_rectangle(cr, current_x, current_y, NODE_WIDTH, NODE_HEIGHT);
        cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
        cairo_fill_preserve(cr);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_stroke(cr);

        // --- 2. Dessiner les données du nœud ---
        data_to_string(current->data, list->data_type, buffer, MAX_STRING_LEN);
        cairo_set_font_size(cr, 12);
        cairo_text_extents_t te;
        cairo_text_extents(cr, buffer, &te);
        cairo_move_to(cr, current_x + NODE_WIDTH/2 - te.width/2, current_y + 25);
        cairo_show_text(cr, buffer);

        // --- 3. Dessiner les pointeurs (Next & Prev) ---
        if (current->next != NULL) {
            double start_x = current_x + NODE_WIDTH;
            double end_x = current_x + SPACING_X; // Ideal next pos

            // Check if next node wraps
            gboolean next_wraps = (end_x + NODE_WIDTH + 40 > width);

            if(next_wraps) {
                // Draw curve to next line
                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_move_to(cr, start_x, center_y - 5);
                cairo_curve_to(cr, start_x + 30, center_y, width - 10, center_y, width - 10, center_y + 20);
                cairo_line_to(cr, width - 10, current_y + row_height - 20);
                cairo_curve_to(cr, width - 10, current_y + row_height, 20, current_y + row_height - 30, 20, current_y + row_height - NODE_HEIGHT/2 - 5);
                cairo_stroke(cr);

                // If Double, draw Prev arrow (return path)
                if (list->list_type == LISTE_DOUBLE) {
                    cairo_set_source_rgb(cr, 0.8, 0.0, 0.0); // Red for Prev
                    // Simplified return path (dashed?)
                    double next_line_y = current_y + row_height;
                    cairo_move_to(cr, 25, next_line_y - NODE_HEIGHT/2 + 5);
                    cairo_curve_to(cr, 25, next_line_y - 40, width - 15, current_y + 40, start_x, center_y + 5);
                    cairo_stroke(cr);
                }
            } else {
                 // Straight line
                 cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                 cairo_move_to(cr, start_x, center_y - 3);
                 cairo_line_to(cr, end_x, center_y - 3);
                 cairo_stroke(cr);
                 // Arrow head Next
                 cairo_move_to(cr, end_x, center_y - 3);
                 cairo_line_to(cr, end_x - 5, center_y - 8);
                 cairo_line_to(cr, end_x - 5, center_y + 2);
                 cairo_fill(cr);

                 // If Double, draw Prev arrow
                 if (list->list_type == LISTE_DOUBLE) {
                     cairo_set_source_rgb(cr, 0.8, 0.0, 0.0);
                     cairo_move_to(cr, end_x, center_y + 3);
                     cairo_line_to(cr, start_x, center_y + 3);
                     cairo_stroke(cr);
                     // Arrow head Prev
                     cairo_move_to(cr, start_x, center_y + 3);
                     cairo_line_to(cr, start_x + 5, center_y + 8);
                     cairo_line_to(cr, start_x + 5, center_y - 2);
                     cairo_fill(cr);
                 }
            }
        } else {
            // NULL
             cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
             cairo_move_to(cr, current_x + NODE_WIDTH + 2, current_y + 25);
             cairo_show_text(cr, "X");
        }

        current_x += SPACING_X;
        current = current->next;
    }

    return TRUE;
}

/**
 * @brief Calcule la hauteur nécessaire pour dessiner la liste.
 */
int get_list_drawing_height(List *list, int width) {
    if (!list || !list->head) return 400; // Min height

    double current_x = 20;
    double current_y = 60;
    double row_height = NODE_HEIGHT + 60;

    Node *current = list->head;
    while (current != NULL) {
        if (current_x + NODE_WIDTH + 40 > width) {
             current_x = 20;
             current_y += row_height;
        }
        current_x += SPACING_X;
        current = current->next;
    }

    return (int)(current_y + row_height + 20);
}
