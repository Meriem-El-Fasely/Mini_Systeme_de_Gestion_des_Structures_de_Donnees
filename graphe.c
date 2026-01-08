#include "graphe.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define NODE_RADIUS 20

// ==========================================
// GESTION
// ==========================================

void init_graph(Graphe *g, DataType type, int directed) {
    if(!g) return;
    g->node_count = 0;
    g->data_type = type;
    g->is_directed = directed;
    g->is_weighted = 1;
    g->path_len = 0;
    g->selected_id = -1;
    for(int i=0; i<MAX_NODES; i++) g->nodes[i] = NULL;
}

void clear_graph(Graphe *g) {
    if(!g) return;
    for(int i=0; i<g->node_count; i++) {
        if(g->nodes[i]) {
            EdgeGraphe *e = g->nodes[i]->head;
            while(e) {
                EdgeGraphe *tmp = e;
                e = e->next;
                free(tmp);
            }
            if(g->nodes[i]->data) free(g->nodes[i]->data);
            free(g->nodes[i]);
            g->nodes[i] = NULL;
        }
    }
    g->node_count = 0;
    g->path_len = 0;
}

void copy_graph(Graphe *dest, const Graphe *src) {
    if (!dest || !src) return;
    clear_graph(dest);
    init_graph(dest, src->data_type, src->is_directed);
    dest->is_weighted = src->is_weighted;
    dest->selected_id = src->selected_id;

    // Nodes
    for(int i=0; i<src->node_count; i++) {
        NodeGraphe *n = src->nodes[i];
        add_node_graph(dest, n->x, n->y, n->data);

    }

    // Edges
    for(int i=0; i<src->node_count; i++) {
        EdgeGraphe *e = src->nodes[i]->head;
        while(e) {


            if (src->is_directed) {
                add_edge_graph(dest, i, e->dest_index, e->weight);
            } else {

                 if (i < e->dest_index) {
                     add_edge_graph(dest, i, e->dest_index, e->weight);
                 }
            }

            e = e->next;
        }
    }
}

int add_node_graph(Graphe *g, double x, double y, const void *data) {
    if (g->node_count >= MAX_NODES) return -1;

    NodeGraphe *n = malloc(sizeof(NodeGraphe));
    n->id = g->node_count;
    n->x = x;
    n->y = y;
    n->head = NULL;

    // Copy data
    size_t sz = 0;
    if(g->data_type == TYPE_INT) sz=sizeof(int);
    else if(g->data_type == TYPE_FLOAT) sz=sizeof(float);
    else if(g->data_type == TYPE_CHAR) sz=sizeof(char);
    else if(g->data_type == TYPE_STRING) sz=MAX_STRING_LEN;

    n->data = malloc(sz);
    if (g->data_type == TYPE_STRING) strncpy((char*)n->data, (char*)data, MAX_STRING_LEN);
    else memcpy(n->data, data, sz);

    g->nodes[g->node_count] = n;
    g->node_count++;
    return n->id;
}

void add_edge_graph(Graphe *g, int src, int dest, float weight) {
    if (src < 0 || src >= g->node_count || dest < 0 || dest >= g->node_count) return;


    EdgeGraphe *curr = g->nodes[src]->head;
    while(curr) {
        if (curr->dest_index == dest) {
            curr->weight = weight;
            return;
        }
        curr = curr->next;
    }


    EdgeGraphe *e = malloc(sizeof(EdgeGraphe));
    e->dest_index = dest;
    e->weight = weight;
    e->next = g->nodes[src]->head;
    g->nodes[src]->head = e;


    if (!g->is_directed) {
        EdgeGraphe *e2 = malloc(sizeof(EdgeGraphe));
        e2->dest_index = src;
        e2->weight = weight;
        e2->next = g->nodes[dest]->head;
        g->nodes[dest]->head = e2;
    }
}

NodeGraphe* get_node_at(Graphe *g, double x, double y) {
    for(int i=0; i<g->node_count; i++) {
        double dx = g->nodes[i]->x - x;
        double dy = g->nodes[i]->y - y;
        if (sqrt(dx*dx + dy*dy) <= NODE_RADIUS) {
            return g->nodes[i];
        }
    }
    return NULL;
}

// ==========================================
// ALGORITHMES
// ==========================================

static void reconstruct_path(Graphe *g, int start, int end, int *pred) {
    g->path_len = 0;
    if (pred[end] == -1 && start != end) return;

    int curr = end;
    int temp_path[MAX_NODES];
    int count = 0;

    while (curr != -1) {
        temp_path[count++] = curr;
        curr = pred[curr];
        if (curr == -1 && temp_path[count-1] != start) {

             break;
        }
    }


    for(int i=0; i<count; i++) {
        g->path_indices[i] = temp_path[count-1-i];
    }
    g->path_len = count;
}

int algo_dijkstra(Graphe *g, int start, int end) {
    if (start < 0 || end < 0 || start >= g->node_count || end >= g->node_count) return 0;

    float dist[MAX_NODES];
    int pred[MAX_NODES];
    int visited[MAX_NODES];

    for(int i=0; i<g->node_count; i++) {
        dist[i] = FLT_MAX;
        pred[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for(int i=0; i<g->node_count; i++) {

        int u = -1;
        float min_d = FLT_MAX;
        for(int j=0; j<g->node_count; j++) {
            if (!visited[j] && dist[j] < min_d) {
                min_d = dist[j];
                u = j;
            }
        }

        if (u == -1 || dist[u] == FLT_MAX) break;
        visited[u] = 1;
        if (u == end) break;


        EdgeGraphe *e = g->nodes[u]->head;
        while(e) {
            int v = e->dest_index;
            if (!visited[v] && dist[u] + e->weight < dist[v]) {
                dist[v] = dist[u] + e->weight;
                pred[v] = u;
            }
            e = e->next;
        }
    }

    reconstruct_path(g, start, end, pred);
    return 1;
}

int algo_bellman_ford(Graphe *g, int start, int end) {
    if (start < 0 || end < 0) return 0;
    float dist[MAX_NODES];
    int pred[MAX_NODES];

    for(int i=0; i<g->node_count; i++) { dist[i] = FLT_MAX; pred[i] = -1; }
    dist[start] = 0;


    for(int i=0; i<g->node_count - 1; i++) {
        for(int u=0; u<g->node_count; u++) {
            EdgeGraphe *e = g->nodes[u]->head;
            while(e) {
                int v = e->dest_index;
                if (dist[u] != FLT_MAX && dist[u] + e->weight < dist[v]) {
                    dist[v] = dist[u] + e->weight;
                    pred[v] = u;
                }
                e = e->next;
            }
        }
    }


    for(int u=0; u<g->node_count; u++) {
        EdgeGraphe *e = g->nodes[u]->head;
        while(e) {
            if (dist[u] != FLT_MAX && dist[u] + e->weight < dist[e->dest_index]) {
                return 0;
            }
            e = e->next;
        }
    }

    reconstruct_path(g, start, end, pred);
    return 1;
}

int algo_floyd_warshall(Graphe *g, int start, int end) {
    if (start < 0 || end < 0) return 0;

    float dist[MAX_NODES][MAX_NODES];
    int next[MAX_NODES][MAX_NODES];


    for(int i=0; i<g->node_count; i++) {
        for(int j=0; j<g->node_count; j++) {
            if (i == j) dist[i][j] = 0;
            else dist[i][j] = FLT_MAX;
            next[i][j] = -1;
        }
    }


    for(int i=0; i<g->node_count; i++) {
        EdgeGraphe *e = g->nodes[i]->head;
        while(e) {
            dist[i][e->dest_index] = e->weight;
            next[i][e->dest_index] = e->dest_index;
            e = e->next;
        }
    }


    for(int k=0; k<g->node_count; k++) {
        for(int i=0; i<g->node_count; i++) {
            for(int j=0; j<g->node_count; j++) {
                if (dist[i][k] != FLT_MAX && dist[k][j] != FLT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }


    for(int i=0; i<g->node_count; i++) {
        if (dist[i][i] < 0) return 0;
    }


    if (dist[start][end] == FLT_MAX) {
        g->path_len = 0;
        return 1;
    }

    g->path_len = 0;
    int curr = start;
    while(curr != end) {
        g->path_indices[g->path_len++] = curr;
        curr = next[curr][end];
        if (curr == -1) break;
    }
    g->path_indices[g->path_len++] = end;

    return 1;
}



static int is_too_close(Graphe *g, double x, double y, double min_dist) {
    for(int i=0; i<g->node_count; i++) {
        double dx = g->nodes[i]->x - x;
        double dy = g->nodes[i]->y - y;
        if (sqrt(dx*dx + dy*dy) < min_dist) return 1;
    }
    return 0;
}

void generate_random_graph(Graphe *g, int count, int directed, int weighted) {
    clear_graph(g);
    init_graph(g, g->data_type, directed);
    g->is_weighted = weighted;

    if (count > MAX_NODES) count = MAX_NODES;

    int margin = 60;
    int max_x = 700;
    int max_y = 500;

    for(int i=0; i<count; i++) {
        double x, y;
        int attempts = 0;
        int placed = 0;

        while(attempts < 100) {
            x = margin + rand() % (max_x - margin);
            y = margin + rand() % (max_y - margin);

            if (!is_too_close(g, x, y, 60.0)) {
                placed = 1;
                break;
            }
            attempts++;
        }

        if (!placed) {
            x = margin + rand() % (max_x - margin);
            y = margin + rand() % (max_y - margin);
        }

        char buf[MAX_STRING_LEN];
        if (g->data_type == TYPE_INT) { int v = i; add_node_graph(g, x, y, &v); }
        else if (g->data_type == TYPE_FLOAT) { float v = i + 0.5; add_node_graph(g, x, y, &v); }
        else if (g->data_type == TYPE_CHAR) { char v = 'A' + i; add_node_graph(g, x, y, &v); }
        else if (g->data_type == TYPE_STRING) { sprintf(buf, "N%d", i); add_node_graph(g, x, y, buf); }
    }


    int max_edges_per_node = (count > 5) ? 2 : 3;

    for(int i=0; i<count; i++) {
        int k_edges = 1 + rand() % max_edges_per_node;

        for(int k=0; k<k_edges; k++) {
            int dest = rand() % count;
            if (dest != i) {

                float w = weighted ? (1 + rand() % 20) : 1;
                add_edge_graph(g, i, dest, w);
            }
        }
    }
}

static void dfs_all_paths(Graphe *g, int u, int d, int visited[], int path[], int path_idx, char *buffer, int *buf_len, int buf_cap) {
    visited[u] = 1;
    path[path_idx] = u;
    path_idx++;

    if (u == d) {
        char line[256];
        line[0] = '\0';
        strcat(line, "[ ");
        float total_cost = 0;
        for(int i=0; i<path_idx; i++) {
            char tmp[16];
            sprintf(tmp, "%d ", path[i]);
            strcat(line, tmp);
            if(i < path_idx - 1) strcat(line, "-> ");


            if (i < path_idx - 1) {
                EdgeGraphe *e = g->nodes[path[i]]->head;
                while(e) {
                    if (e->dest_index == path[i+1]) { total_cost += e->weight; break; }
                    e = e->next;
                }
            }
        }
        char cost_str[32];
        sprintf(cost_str, "] (Cout: %.1f)\n", total_cost);
        strcat(line, cost_str);

        if (*buf_len + strlen(line) < buf_cap) {
            strcat(buffer, line);
            *buf_len += strlen(line);
        }
    } else {
        EdgeGraphe *e = g->nodes[u]->head;
        while(e) {
            if (!visited[e->dest_index]) {
                dfs_all_paths(g, e->dest_index, d, visited, path, path_idx, buffer, buf_len, buf_cap);
            }
            e = e->next;
        }
    }

    path_idx--;
    visited[u] = 0;
}

void find_all_paths_print(Graphe *g, int start, int end, char *buffer, int buffer_size) {
    int visited[MAX_NODES];
    int path[MAX_NODES];
    for(int i=0; i<MAX_NODES; i++) visited[i] = 0;

    buffer[0] = '\0';
    int len = 0;

    char header[64];
    sprintf(header, "--- Tous les chemins de %d vers %d ---\n", start, end);
    strcat(buffer, header);
    len += strlen(header);

    dfs_all_paths(g, start, end, visited, path, 0, buffer, &len, buffer_size);

    if (len == strlen(header)) {
        strcat(buffer, "Aucun chemin trouvÃ©.\n");
    }
    strcat(buffer, "-----------------------------\n");
}


// ==========================================
// DESSIN
// ==========================================

static void draw_glow(cairo_t *cr, double x, double y, double radius, double r, double g, double b, double alpha) {
    cairo_pattern_t *pat = cairo_pattern_create_radial(x, y, 0, x, y, radius*1.5);
    cairo_pattern_add_color_stop_rgba(pat, 0, r, g, b, alpha);
    cairo_pattern_add_color_stop_rgba(pat, 1, r, g, b, 0);
    cairo_set_source(cr, pat);
    cairo_arc(cr, x, y, radius*1.5, 0, 2*M_PI);
    cairo_fill(cr);
    cairo_pattern_destroy(pat);
}

static void draw_arrow(cairo_t *cr, double x1, double y1, double x2, double y2, int is_highlighted, double offset_factor, int is_directed) {
    double angle = atan2(y2 - y1, x2 - x1);
    double r = NODE_RADIUS;

    double mx = (x1 + x2) / 2.0;
    double my = (y1 + y2) / 2.0;
    double perp_x = -(y2 - y1);
    double perp_y = (x2 - x1);
    double len = sqrt(perp_x*perp_x + perp_y*perp_y);
    if (len > 0) { perp_x /= len; perp_y /= len; }

    double cx = mx + perp_x * (offset_factor * 40.0);
    double cy = my + perp_y * (offset_factor * 40.0);

    if (is_highlighted) {
        cairo_set_source_rgba(cr, 1.0, 0.2, 0.4, 0.8);
        cairo_set_line_width(cr, 4);
    } else {
        cairo_set_source_rgba(cr, 0.2, 0.2, 0.2, 0.8);
        cairo_set_line_width(cr, 2.0);
    }

    cairo_move_to(cr, x1, y1);

    if (fabs(offset_factor) < 0.1) {
        cairo_line_to(cr, x2, y2);
    } else {
        cairo_curve_to(cr, x1, y1, cx, cy, x2, y2);
    }
    cairo_stroke(cr);

    if (is_directed) {
        double arr_angle;
        if (fabs(offset_factor) < 0.1) {
            arr_angle = angle;
        } else {
            arr_angle = atan2(y2 - cy, x2 - cx);
        }

        double end_x = x2 - r * cos(arr_angle);
        double end_y = y2 - r * sin(arr_angle);

        double arrow_len = 14;
        double angle_offset = M_PI / 7;

        cairo_move_to(cr, end_x, end_y);
        cairo_line_to(cr, end_x - arrow_len * cos(arr_angle - angle_offset), end_y - arrow_len * sin(arr_angle - angle_offset));
        cairo_move_to(cr, end_x, end_y);
        cairo_line_to(cr, end_x - arrow_len * cos(arr_angle + angle_offset), end_y - arrow_len * sin(arr_angle + angle_offset));
        cairo_stroke(cr);
    }
}

static void draw_weight_label(cairo_t *cr, double x1, double y1, double x2, double y2, double offset_factor, float weight) {
    double mx = (x1 + x2) / 2.0;
    double my = (y1 + y2) / 2.0;

    double perp_x = -(y2 - y1);
    double perp_y = (x2 - x1);
    double len = sqrt(perp_x*perp_x + perp_y*perp_y);
    if (len > 0) { perp_x /= len; perp_y /= len; }
    double cx = mx + perp_x * (offset_factor * 40.0);
    double cy = my + perp_y * (offset_factor * 40.0);
    double label_x = 0.25*x1 + 0.5*cx + 0.25*x2;
    double label_y = 0.25*y1 + 0.5*cy + 0.25*y2;

    char w_str[16];
    sprintf(w_str, "%.0f", weight);
    cairo_set_font_size(cr, 11);
    cairo_text_extents_t te;
    cairo_text_extents(cr, w_str, &te);
    double pad = 4;
    double w = te.width + 2*pad;
    double h = te.height + 2*pad;

    cairo_set_source_rgba(cr, 1, 1, 1, 0.9);
    cairo_rectangle(cr, label_x - w/2, label_y - h/2, w, h);
    cairo_fill(cr);
    cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 1.0);
    cairo_set_line_width(cr, 1);
    cairo_rectangle(cr, label_x - w/2, label_y - h/2, w, h);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, label_x - te.width/2 - te.x_bearing, label_y - te.height/2 - te.y_bearing);
    cairo_show_text(cr, w_str);
}


gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Graphe *g = (Graphe *)data;
    if (!g) return FALSE;

    if (g->node_count == 0) {
        cairo_set_source_rgba(cr, 0.4, 0.4, 0.4, 0.5);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 20);
        cairo_move_to(cr, 50, 50);
        cairo_show_text(cr, "Double-cliquez pour ajouter un noeud.");
        return TRUE;
    }

    // --- 1. Edges Layer ---
    for(int i=0; i<g->node_count; i++) {
        NodeGraphe *u = g->nodes[i];
        EdgeGraphe *e = u->head;
        while(e) {
            NodeGraphe *v = g->nodes[e->dest_index];


            if (!g->is_directed && u->id > v->id) {
                e = e->next;
                continue;
            }



            int is_path = 0;
            for(int k=0; k<g->path_len - 1; k++) {
                int p1 = g->path_indices[k];
                int p2 = g->path_indices[k+1];

                if ( (p1 == u->id && p2 == v->id) || (!g->is_directed && p1 == v->id && p2 == u->id)) {
                    is_path = 1;
                    break;
                }
            }


            double offset = 0.0;
            if (g->is_directed) {

                EdgeGraphe *rev = v->head;
                int rev_exists = 0;
                while(rev) { if(rev->dest_index == u->id) { rev_exists=1; break; } rev=rev->next; }
                if (rev_exists) offset = 0.5;
            }

            draw_arrow(cr, u->x, u->y, v->x, v->y, is_path, offset, g->is_directed);

            if (g->is_weighted) {
                draw_weight_label(cr, u->x, u->y, v->x, v->y, offset, e->weight);
            }

            e = e->next;
        }
    }

    // --- 2. Nodes Layer ---
    for(int i=0; i<g->node_count; i++) {
        NodeGraphe *n = g->nodes[i];
        int is_path_node = 0;
        for(int k=0; k<g->path_len; k++) if (g->path_indices[k] == n->id) is_path_node = 1;

        // Shadow
        cairo_set_source_rgba(cr, 0, 0, 0, 0.2);
        cairo_arc(cr, n->x + 3, n->y + 3, NODE_RADIUS, 0, 2*M_PI);
        cairo_fill(cr);



        if (is_path_node) cairo_set_source_rgb(cr, 1.0, 0.42, 0.42);
        else if (n->id == g->selected_id) cairo_set_source_rgb(cr, 0.4, 0.9, 0.4);
        else cairo_set_source_rgb(cr, 0.3, 0.67, 0.96);

        cairo_arc(cr, n->x, n->y, NODE_RADIUS, 0, 2*M_PI);
        cairo_fill(cr);

        cairo_set_source_rgba(cr, 1, 1, 1, 0.9);
        cairo_set_line_width(cr, 2);
        cairo_stroke(cr);

        if (n->id == g->selected_id) {
            cairo_set_source_rgba(cr, 0.1, 0.8, 0.1, 0.6);
            cairo_set_line_width(cr, 4);
            cairo_arc(cr, n->x, n->y, NODE_RADIUS + 2, 0, 2*M_PI);
            cairo_stroke(cr);
        }

        // Text
        cairo_set_source_rgb(cr, 1, 1, 1);

        cairo_set_source_rgb(cr, 0, 0, 0);

        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 13); // Slightly smaller

        char buf[32];
        if (g->data_type == TYPE_INT) sprintf(buf, "%d", *(int*)n->data);
        else if (g->data_type == TYPE_FLOAT) sprintf(buf, "%.1f", *(float*)n->data);
        else if (g->data_type == TYPE_CHAR) sprintf(buf, "%c", *(char*)n->data);
        else if (g->data_type == TYPE_STRING) sprintf(buf, "%s", (char*)n->data);

        cairo_text_extents_t te;
        cairo_text_extents(cr, buf, &te);
        cairo_move_to(cr, n->x - te.width/2 - te.x_bearing, n->y - te.height/2 - te.y_bearing); // Center text better
        cairo_show_text(cr, buf);
    }

    return TRUE;
}

