#include "arbre.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NODE_RADIUS 30
#define LEVEL_HEIGHT 90
#define MIN_NODE_SPACING 70


// ==========================================
// UTILITAIRES
// ==========================================

size_t get_element_size(DataType type) {
    switch (type) {
        case TYPE_INT: return sizeof(int);
        case TYPE_FLOAT: return sizeof(float);
        case TYPE_CHAR: return sizeof(char);
        case TYPE_STRING: return MAX_STRING_LEN;
    }
    return 0;
}

int tree_generic_compare(const void *a, const void *b, DataType type) {
    switch (type) {
        case TYPE_INT: return (*(const int *)a - *(const int *)b);
        case TYPE_FLOAT: return (*(const float *)a > *(const float *)b) - (*(const float *)a < *(const float *)b);
        case TYPE_CHAR: return (*(const char *)a - *(const char *)b);
        case TYPE_STRING: return strcmp((const char *)a, (const char *)b);
    }
    return 0;
}

static void data_to_str(const void *data, DataType type, char *buffer) {
    switch (type) {
        case TYPE_INT: sprintf(buffer, "%d", *(const int*)data); break;
        case TYPE_FLOAT: sprintf(buffer, "%.1f", *(const float*)data); break;
        case TYPE_CHAR: sprintf(buffer, "%c", *(const char*)data); break;
        case TYPE_STRING: sprintf(buffer, "%s", (const char*)data); break;
    }
}

// ==========================================
// GESTION MÉMOIRE
// ==========================================

void init_tree(Arbre *tree, TreeType type, DataType data_type) {
    if(!tree) return;
    tree->root = NULL;
    tree->highlight_node = NULL;
    tree->type = type;
    tree->data_type = data_type;
}

static NodeArbre* create_node_tree(const void *data, DataType type) {
    NodeArbre *node = malloc(sizeof(NodeArbre));
    size_t sz = get_element_size(type);
    node->data = malloc(sz);
    memcpy(node->data, data, sz);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void clear_tree_nodes(NodeArbre *node, DataType type) {
    if (!node) return;
    clear_tree_nodes(node->left, type);
    clear_tree_nodes(node->right, type);
    if(node->data) free(node->data);
    free(node);
}

void destroy_tree(Arbre *tree) {
    if(!tree) return;
    clear_tree_nodes(tree->root, tree->data_type);
    tree->root = NULL;
}

// ==========================================
// INSERTION
// ==========================================

int tree_contains(NodeArbre *node, const void *data, DataType type) {
    if (!node) return 0;
    if (tree_generic_compare(data, node->data, type) == 0) return 1;

    if (tree_contains(node->left, data, type)) return 1;
    if (tree_contains(node->right, data, type)) return 1;

    return 0;
}

void insert_binary_ordered(NodeArbre **node, const void *data, DataType type) {
    if (*node == NULL) {
        *node = create_node_tree(data, type);
        return;
    }
    int cmp = tree_generic_compare(data, (*node)->data, type);
    if (cmp < 0) {
        insert_binary_ordered(&(*node)->left, data, type);
    } else if (cmp > 0) {
        insert_binary_ordered(&(*node)->right, data, type);
    }
}

void insert_nary_random(NodeArbre *root, const void *data, DataType type) {
    if (!root) return;

    NodeArbre *current = root;
    int attempts = 0;
    while(current && attempts < 100) {
        attempts++;
        if (rand() % 3 == 0) {
            NodeArbre *new_node = create_node_tree(data, type);
            new_node->right = current->left;
            current->left = new_node;
            return;
        }

        if (current->left && (rand()%2==0)) current = current->left;
        else if (current->right) current = current->right;
        else break;
    }

    if(current) {
         NodeArbre *new_node = create_node_tree(data, type);
         new_node->right = current->left;
         current->left = new_node;
    }
}

int insert_node_tree(Arbre *tree, const void *data) {
    if (tree->root == NULL) {
        tree->root = create_node_tree(data, tree->data_type);
        return 1;
    }

    if (tree_contains(tree->root, data, tree->data_type)) {
        return 0;
    }

    if (tree->type == ARBRE_BINAIRE) {
        insert_binary_ordered(&tree->root, data, tree->data_type);
    } else {
        insert_nary_random(tree->root, data, tree->data_type);
    }
    return 1;
}

// ==========================================
// TRI
// ==========================================

static void sort_sibling_list(NodeArbre *head, DataType type) {
    if(!head) return;
    int swapped;
    NodeArbre *ptr1;
    NodeArbre *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->right != lptr) {
            if (tree_generic_compare(ptr1->data, ptr1->right->data, type) > 0) {
                void *temp = ptr1->data;
                ptr1->data = ptr1->right->data;
                ptr1->right->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->right;
        }
        lptr = ptr1;
    } while (swapped);
}

static void sort_nary_recursive(NodeArbre *node, DataType type) {
    if (!node) return;
    if (node->left) {
        sort_sibling_list(node->left, type);

        NodeArbre *child = node->left;
        while(child) {
            sort_nary_recursive(child, type);
            child = child->right;
        }
    }
}


void sort_tree_nodes(Arbre *tree) {
    if(!tree || !tree->root) return;

    if (tree->type == ARBRE_NAIRE) {
        sort_nary_recursive(tree->root, tree->data_type);
    }
}


// ==========================================
// SUPPRESSION
// ==========================================

static NodeArbre* find_min(NodeArbre *node) {
    while(node->left) node = node->left;
    return node;
}

static NodeArbre* delete_binary_rec(NodeArbre *root, const void *data, DataType type) {
    if(!root) return NULL;
    int cmp = tree_generic_compare(data, root->data, type);
    if (cmp < 0) root->left = delete_binary_rec(root->left, data, type);
    else if (cmp > 0) root->right = delete_binary_rec(root->right, data, type);
    else {
        if (!root->left) {
            NodeArbre *temp = root->right;
            if(root->data) free(root->data);
            free(root);
            return temp;
        } else if (!root->right) {
            NodeArbre *temp = root->left;
            if(root->data) free(root->data);
            free(root);
            return temp;
        }
        NodeArbre *temp = find_min(root->right);
        size_t sz = get_element_size(type);
        memcpy(root->data, temp->data, sz);
        root->right = delete_binary_rec(root->right, temp->data, type);
    }
    return root;
}

int delete_node_tree(Arbre *tree, const void *data) {
    if (!tree->root) return -1;
    if (tree->type == ARBRE_BINAIRE) {
        tree->root = delete_binary_rec(tree->root, data, tree->data_type);
        return 0;
    }
    return -1;
}

// ==========================================
// PARCOURS
// ==========================================

static void append_buf(GtkTextBuffer *buffer, const void *data, DataType type) {
    char tmp[MAX_STRING_LEN];
    data_to_str(data, type, tmp);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, tmp, -1);
    gtk_text_buffer_insert(buffer, &end, " ", -1);
}

void parcours_prefixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue) {
    if (!node) return;
    if (queue) *queue = g_list_prepend(*queue, node);
    else append_buf(buffer, node->data, type);
    parcours_prefixe(node->left, type, buffer, queue);
    parcours_prefixe(node->right, type, buffer, queue);
}

void parcours_infixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue) {
    if (!node) return;
    parcours_infixe(node->left, type, buffer, queue);
    if (queue) *queue = g_list_prepend(*queue, node);
    else append_buf(buffer, node->data, type);
    parcours_infixe(node->right, type, buffer, queue);
}

void parcours_postfixe(NodeArbre *node, DataType type, GtkTextBuffer *buffer, GList **queue) {
    if (!node) return;
    parcours_postfixe(node->left, type, buffer, queue);
    parcours_postfixe(node->right, type, buffer, queue);
    if (queue) *queue = g_list_prepend(*queue, node);
    else append_buf(buffer, node->data, type);
}

void parcours_largeur(Arbre *tree, GtkTextBuffer *buffer, GList **queue) {
    if(!tree->root) return;
    GQueue *q = g_queue_new();
    g_queue_push_tail(q, tree->root);

    while(!g_queue_is_empty(q)) {
        NodeArbre *curr = g_queue_pop_head(q);
        if (queue) *queue = g_list_prepend(*queue, curr);
        else append_buf(buffer, curr->data, tree->data_type);

        if (tree->type == ARBRE_BINAIRE) {
            if(curr->left) g_queue_push_tail(q, curr->left);
            if(curr->right) g_queue_push_tail(q, curr->right);
        } else {
             NodeArbre *child = curr->left;
             while(child) {
                 g_queue_push_tail(q, child);
                 child = child->right;
             }
        }
    }
    g_queue_free(q);
}

// ==========================================
// MÉTRIQUES
// ==========================================

int tree_size(NodeArbre *node) {
    if (!node) return 0;
    return 1 + tree_size(node->left) + tree_size(node->right);
}

int tree_height(NodeArbre *node, TreeType type) {
    if (!node) return 0;
    if (type == ARBRE_BINAIRE) {
        int l = tree_height(node->left, type);
        int r = tree_height(node->right, type);
        return 1 + (l > r ? l : r);
    } else {
        int max_h = 0;
        NodeArbre *child = node->left;
        while(child) {
            int h = tree_height(child, type);
            if (h > max_h) max_h = h;
            child = child->right;
        }
        return 1 + max_h;
    }
}

// ==========================================
// TRANSFORMATION
// ==========================================

void transform_to_binary(Arbre *tree) {
    if (tree->type == ARBRE_NAIRE) {
        tree->type = ARBRE_BINAIRE;
    }
}

// ==========================================
// DESSIN (SMART LAYOUT)
// ==========================================

static double get_subtree_width(NodeArbre *node, TreeType ttype) {
    if (!node) return 0;

    double my_width = MIN_NODE_SPACING;

    if (ttype == ARBRE_BINAIRE) {
        double w_left = get_subtree_width(node->left, ttype);
        double w_right = get_subtree_width(node->right, ttype);
        double children_w = w_left + w_right;

        return (children_w > my_width) ? children_w : my_width;
    } else {
        double children_w = 0;
        NodeArbre *child = node->left;
        while(child) {
            children_w += get_subtree_width(child, ttype);
            child = child->right;
        }
        return (children_w > my_width) ? children_w : my_width;
    }
}

static void draw_node_smart_rec(cairo_t *cr, Arbre *tree, NodeArbre *node, double x, double y, double available_width, DataType type, TreeType ttype) {
    if (!node) return;

    char buf[64];
    data_to_str(node->data, type, buf);

    cairo_set_line_width(cr, 3);

    if (ttype == ARBRE_BINAIRE) {
        double w_left = get_subtree_width(node->left, ttype);
        double w_right = get_subtree_width(node->right, ttype);
        double total_w = w_left + w_right;
        if(total_w == 0) total_w = 1;

        double start_x = x - total_w / 2.0;

        if (node->left) {
            double child_center = start_x + w_left / 2.0;

            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, child_center, y + LEVEL_HEIGHT);
            cairo_stroke(cr);

            draw_node_smart_rec(cr, tree, node->left, child_center, y + LEVEL_HEIGHT, w_left, type, ttype);
        }

        if (node->right) {
            double child_center = start_x + w_left + w_right / 2.0;

            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_move_to(cr, x, y);
            cairo_line_to(cr, child_center, y + LEVEL_HEIGHT);
            cairo_stroke(cr);

            draw_node_smart_rec(cr, tree, node->right, child_center, y + LEVEL_HEIGHT, w_right, type, ttype);
        }

    } else {
        double total_children_w = 0;
        NodeArbre *child = node->left;
        while(child) {
            total_children_w += get_subtree_width(child, ttype);
            child = child->right;
        }

        if (total_children_w > 0) {
            double start_x = x - total_children_w / 2.0;
            double current_x = start_x;

            child = node->left;
            while(child) {
                double w = get_subtree_width(child, ttype);
                double child_center = current_x + w / 2.0;

                cairo_set_source_rgb(cr, 0, 0, 0);
                cairo_move_to(cr, x, y);
                cairo_line_to(cr, child_center, y + LEVEL_HEIGHT);
                cairo_stroke(cr);

                draw_node_smart_rec(cr, tree, child, child_center, y + LEVEL_HEIGHT, w, type, ttype);

                current_x += w;
                child = child->right;
            }
        }
    }


    cairo_pattern_t *pat;
    if (node == tree->highlight_node) {
        pat = cairo_pattern_create_radial(x, y, NODE_RADIUS-5, x, y, NODE_RADIUS+12);
        cairo_pattern_add_color_stop_rgba(pat, 0, 1, 0.4, 0.2, 0.8);
        cairo_pattern_add_color_stop_rgba(pat, 1, 1, 0.2, 0.2, 0);
    } else {
        pat = cairo_pattern_create_radial(x+3, y+3, NODE_RADIUS-5, x+3, y+3, NODE_RADIUS+5);
        cairo_pattern_add_color_stop_rgba(pat, 0, 0, 0, 0, 0.3);
        cairo_pattern_add_color_stop_rgba(pat, 1, 0, 0, 0, 0);
    }
    cairo_set_source(cr, pat);
    cairo_arc(cr, x, y, NODE_RADIUS+10, 0, 2*M_PI);
    cairo_fill(cr);
    cairo_pattern_destroy(pat);

    pat = cairo_pattern_create_linear(x - NODE_RADIUS, y - NODE_RADIUS, x + NODE_RADIUS, y + NODE_RADIUS);
    if (node == tree->highlight_node) {
        cairo_pattern_add_color_stop_rgb(pat, 0, 1, 0.9, 0.6);
        cairo_pattern_add_color_stop_rgb(pat, 1, 1, 0.6, 0.2);
    } else {
        cairo_pattern_add_color_stop_rgb(pat, 0, 0.9, 0.95, 1.0);
        cairo_pattern_add_color_stop_rgb(pat, 1, 0.6, 0.8, 1.0);
    }
    cairo_set_source(cr, pat);
    cairo_arc(cr, x, y, NODE_RADIUS, 0, 2 * M_PI);
    cairo_fill(cr);
    cairo_pattern_destroy(pat);

    cairo_set_source_rgba(cr, 1, 1, 1, 0.6);
    cairo_arc(cr, x-8, y-8, NODE_RADIUS/2.5, 0, 2*M_PI);
    cairo_fill(cr);

    // Border
    cairo_set_line_width(cr, 2);
    if (node == tree->highlight_node) cairo_set_source_rgb(cr, 1, 0.2, 0.2); // Red
    else cairo_set_source_rgb(cr, 0.2, 0.4, 0.8); // Dark Blue Border
    cairo_arc(cr, x, y, NODE_RADIUS, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Text (BLACK)
    cairo_set_source_rgb(cr, 0, 0, 0); // Black Text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);
    cairo_text_extents_t te;
    cairo_text_extents(cr, buf, &te);
    cairo_move_to(cr, x - te.width/2, y + te.height/2);
    cairo_show_text(cr, buf);
}

gboolean draw_tree(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Arbre *tree = (Arbre *)data;
    if (!tree) return FALSE;

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    cairo_pattern_t *bg = cairo_pattern_create_linear(0, 0, 0, height);
    cairo_pattern_add_color_stop_rgb(bg, 0, 0.98, 0.98, 1.0); // Very light blue top
    cairo_pattern_add_color_stop_rgb(bg, 1, 0.90, 0.95, 1.0); // Slightly darker bottom
    cairo_set_source(cr, bg);
    cairo_paint(cr);
    cairo_pattern_destroy(bg);

    if (!tree->root) {
        cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_ITALIC, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 24);
        cairo_text_extents_t te;
        cairo_text_extents(cr, "Arbre Vide (Cliquez Générer)", &te);
        cairo_move_to(cr, (width-te.width)/2, height/2);
        cairo_show_text(cr, "Arbre Vide (Cliquez Générer)");
        return FALSE;
    }

    double total_tree_w = get_subtree_width(tree->root, tree->type);



    draw_node_smart_rec(cr, tree, tree->root, width / 2.0, 40, total_tree_w, tree->data_type, tree->type);

    return TRUE;
}

void get_tree_visualization_size(Arbre *tree, int *w, int *h) {
    if (!tree || !tree->root) { *w=400; *h=400; return; }

    double total_w = get_subtree_width(tree->root, tree->type);
    int start_y = 40;
    int tree_h = tree_height(tree->root, tree->type);

    *w = (int)total_w + 100; // Padding
    *h = start_y + tree_h * LEVEL_HEIGHT + 100; // Padding
}
