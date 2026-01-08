#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include "types.h"
#include "tri.h"
#include "liste.h"
#include "arbre.h"
#include "graphe.h"
#include "pile_file.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *stack;
    GtkWidget *page_accueil;

    // Page Tri Elements
    GtkWidget *entry_manual;
    // Page Tri Elements - New Design
    GtkComboBoxText *combo_type;
    GtkComboBoxText *combo_algorithm;
    GtkRadioButton *radio_random;
    GtkRadioButton *radio_manual;
    GtkSpinButton *spin_size;

    // Data & Results Display
    GtkTextView *text_view_data;
    GtkTextBuffer *text_buffer_data;

    // Graph
    GtkWidget *drawing_area_curve;

    // Controls
    GtkWidget *btn_reset;
    GtkWidget *btn_launch;
    GtkWidget *btn_compare;
    GtkWidget *btn_curves;

    // Page Liste Elements
    GtkWidget *drawing_area_liste;
    GtkComboBoxText *combo_liste_type;
    GtkComboBoxText *combo_liste_data_type;
    GtkComboBoxText *combo_liste_algo;
    GtkSpinButton *spin_liste_size;
    GtkRadioButton *radio_liste_random;
    GtkRadioButton *radio_liste_manual;
    GtkEntry *entry_liste_value;
    GtkSpinButton *spin_liste_pos;
    GtkCheckButton *check_op_by_value;

    // State
    List current_list;
    // For sorting, we can manage pointers in implementation
    void *sort_array;
    int sort_array_size;
    DataType sort_data_type;
    TriStats tri_stats[4];

    // Curve Data
    double *curve_times[4]; // Arrays to store times for curve
    int *curve_sizes;       // Array of sizes tested
    int curve_points;       // Number of points in the curve

    // Page Arbres Elements
    Arbre current_tree;
    GtkWidget *drawing_area_tree;
    GtkComboBoxText *combo_tree_type;
    GtkComboBoxText *combo_tree_dtype;
    GtkSpinButton *spin_tree_size;
    GtkEntry *entry_tree_val;
    GtkTextView *text_view_tree_log;

    // Animation State
    GList *anim_queue;      // Queue of pointers to NodeArbre
    guint anim_timer_id;    // Timer ID

    // Page Graphes Elements
    Graphe current_graph;
    GtkWidget *drawing_area_graph;
    GtkComboBoxText *combo_graph_dtype;
    int graph_mode; // 0=Node, 1=Edge
    int selected_node_id; // For edge creation

    // NEW Graph UI Elements
    GtkComboBoxText *combo_graph_type;    // Directed/Undirected
    GtkComboBoxText *combo_graph_nature;  // Weighted/Unweighted
    // combo_graph_dtype already exists

    // Graph Inputs
    GtkEntry *entry_graph_node_val;
    GtkEntry *entry_graph_edge_src;
    GtkEntry *entry_graph_edge_dest;

    GtkEntry *entry_graph_edge_weight;
    GtkSpinButton *spin_graph_count;

    // Graph Algos
    GtkComboBoxText *combo_graph_algo;
    GtkEntry *entry_graph_algo_start;
    GtkEntry *entry_graph_algo_end;

    // Graph Results
    GtkTextView *text_view_graph_log;

    #define GRAPH_HISTORY_SIZE 10
    // Undo History
    Graphe graph_history[GRAPH_HISTORY_SIZE];
    int graph_history_top; // Points to the next empty slot (or current count)
    GtkWidget *btn_graph_undo;

    // Page Piles & Files Elements
    Stack current_stack;
    Queue current_queue;
    int pile_file_mode; // 0 = Stack, 1 = Queue

    GtkWidget *pf_stack_main; // Layout stack to switch views
    GtkWidget *page_pile_view;
    GtkWidget *page_file_view;

    GtkWidget *drawing_area_pf; // Unified or separate? Unified is easier if we switch logic in draw callback

    // Config
    GtkComboBoxText *combo_pf_choice; // "Pile" or "File"
    GtkComboBoxText *combo_pf_dtype;
    GtkSpinButton *spin_pf_size;
    GtkRadioButton *radio_pf_manual;
    GtkRadioButton *radio_pf_random;

    // Operations
    GtkEntry *entry_pf_val;
    GtkLabel *lbl_pf_message;
    GtkLabel *lbl_pf_status;

    // Buttons (Dynamic based on mode or all created and hidden/shown)
    GtkWidget *btn_pf_action1; // Push / Enqueue
    GtkWidget *btn_pf_action2; // Pop / Dequeue
    GtkWidget *btn_pf_action3; // Peek / Front
    GtkWidget *btn_pf_clear;   // Clear

} AppData;

void apply_css();
void create_sidebar(AppData *app, GtkWidget *box);
void create_page_accueil(AppData *app);
void create_page_tri(AppData *app);
void create_page_listes(AppData *app);
void create_page_arbres(AppData *app);
void create_page_graphes(AppData *app);
void create_page_pile_file(AppData *app);

// Callback for menu navigation
void on_menu_clicked(GtkWidget *widget, gpointer data);

#endif
