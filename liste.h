#ifndef LISTE_H
#define LISTE_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "types.h" // Inclut DataType, MAX_STRING_LEN

// Énumérations spécifiques à liste
typedef enum {
    LISTE_SIMPLE,
    LISTE_DOUBLE
} ListType;

// --- Structure de nœud générique ---

typedef struct Node {
    void *data;         // Pointeur vers les données
    struct Node *next;  // Pointeur vers le nœud suivant
    struct Node *prev;  // Pointeur vers le nœud précédent (pour liste double)
} Node;

// --- Structure de la Liste ---

typedef struct {
    Node *head;         // Tête de la liste
    ListType list_type; // Type de liste (simple ou double)
    DataType data_type; // Type de données
    int dimension;      // Nombre d'éléments
} List;

// --- Prototypes des fonctions utilitaires ---

void init_list(List *list, ListType type, DataType data_type);
Node *create_node(const void *data, DataType data_type);
void free_node(Node *node, DataType data_type);
void destroy_list(List *list);
void generate_random_list_data(List *list, int count);
size_t get_list_element_size(DataType type);
int list_generic_compare(const void *a, const void *b, DataType type);


// --- Prototypes des opérations de Liste ---

void insert_at_beginning(List *list, const void *data);
void insert_at_end(List *list, const void *data);
int insert_at_position(List *list, const void *data, int position);
void insert_sorted(List *list, const void *data); // New
int delete_at_position(List *list, int position);
int modify_at_position(List *list, const void *new_data, int position);
void sort_linked_list(List *list); // Insertion Sort by default
void sort_list_bubble(List *list);
void sort_list_shell(List *list); // New
void sort_list_quick(List *list); // New // New

// Opérations par valeur
int delete_by_value(List *list, const void *val);
int modify_by_value(List *list, const void *old_val, const void *new_val);

// --- Prototype de la fonction de dessin (Cairo) ---

gboolean draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer data);
int get_list_drawing_height(List *list, int width);

#endif // LISTE_H
