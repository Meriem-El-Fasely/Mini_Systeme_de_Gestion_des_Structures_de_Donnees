#ifndef GRAPHE_H
#define GRAPHE_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "types.h"

#define MAX_NODES 20
#define INF 999999

typedef struct EdgeGraphe {
    int dest_index;
    float weight;
    struct EdgeGraphe *next;
} EdgeGraphe;

typedef struct {
    int id;
    double x, y;
    void *data; // Generic data
    EdgeGraphe *head; // Adjacency list
} NodeGraphe;

typedef struct {
    NodeGraphe *nodes[MAX_NODES];
    int node_count;
    DataType data_type;
    int is_directed;
    int is_weighted;

    // For path visualization
    int path_indices[MAX_NODES];
    int path_len;

    // UI State for drawing
    int selected_id; // -1 if none
} Graphe;

// --- Gestion ---
void init_graph(Graphe *g, DataType type, int directed);
void clear_graph(Graphe *g);
void copy_graph(Graphe *dest, const Graphe *src);
int add_node_graph(Graphe *g, double x, double y, const void *data);
void add_edge_graph(Graphe *g, int src, int dest, float weight);
NodeGraphe* get_node_at(Graphe *g, double x, double y);

// --- Algorithmes ---
// All return 1 on success, 0 on failure/negative cycles
int algo_dijkstra(Graphe *g, int start, int end);
int algo_bellman_ford(Graphe *g, int start, int end);
int algo_floyd_warshall(Graphe *g, int start, int end);

// --- Nouveaux Algorithmes et Utilitaires ---
void generate_random_graph(Graphe *g, int count, int directed, int weighted);
void find_all_paths_print(Graphe *g, int start, int end, char *buffer, int buffer_size);

// --- Dessin ---
gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data);

#endif
