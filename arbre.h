#ifndef ARBRE_H
#define ARBRE_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "types.h"

// Types d'arbres
typedef enum {
    ARBRE_BINAIRE,
    ARBRE_NAIRE
} TreeType;

// Nœud d'arbre (Utilisé pour Binaire et N-Aire via Représentation Fils-Gauche / Frère-Droit)
typedef struct NodeArbre {
    void *data;
    struct NodeArbre *left;  // Binaire: Fils Gauche | N-Aire: Premier Fils
    struct NodeArbre *right; // Binaire: Fils Droit  | N-Aire: Frère Droit
} NodeArbre;

// Structure de contrôle de l'Arbre
typedef struct {
    NodeArbre *root;
    NodeArbre *highlight_node; // For animation
    TreeType type;
    DataType data_type;
} Arbre;

// --- Gestion de la mémoire ---
void init_tree(Arbre *tree, TreeType type, DataType data_type);
void destroy_tree(Arbre *tree);
void clear_tree_nodes(NodeArbre *node, DataType type);

// --- Opérations ---
int insert_node_tree(Arbre *tree, const void *data); // Insertion générique
void insert_binary_ordered(NodeArbre **node, const void *data, DataType type); // BST
void insert_nary_random(NodeArbre *root, const void *data, DataType type); // Insertion aléatoire N-Aire

int delete_node_tree(Arbre *tree, const void *data); // Suppression par valeur

// --- Parcours & Métriques ---
int tree_size(NodeArbre *node);
int tree_height(NodeArbre *node, TreeType type);

// Parcours Profondeur (Modified to support Animation Queue)
void parcours_prefixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue);
void parcours_infixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue);
void parcours_postfixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue);

// Parcours Largeur
void parcours_largeur(Arbre *tree, GtkTextBuffer *buffer, GList **queue);

// Transformation
void transform_to_binary(Arbre *tree);
void sort_tree_nodes(Arbre *tree);
void get_tree_visualization_size(Arbre *tree, int *w, int *h);

// --- Dessin ---
gboolean draw_tree(GtkWidget *widget, cairo_t *cr, gpointer data);

// Utilitaires
int tree_generic_compare(const void *a, const void *b, DataType type);

#endif
