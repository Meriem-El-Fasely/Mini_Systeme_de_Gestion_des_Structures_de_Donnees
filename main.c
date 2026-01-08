#include "interface.h"

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    AppData app;
    memset(&app, 0, sizeof(AppData));

    // Création de la fenêtre principale
    app.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app.window), "Mini System - FIXED VERSION");
    gtk_window_set_default_size(GTK_WINDOW(app.window), 1024, 768);
    g_signal_connect(app.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Appliquer le CSS
    apply_css();

    // Layout principal (Horizontal Box: Sidebar | Contenu)
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(app.window), main_box);

    // Création de la Sidebar
    create_sidebar(&app, main_box);

    // Stack pour le contenu (pages)
    app.stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(app.stack), GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN);
    gtk_box_pack_start(GTK_BOX(main_box), app.stack, TRUE, TRUE, 0);

    // Création des pages
    create_page_accueil(&app);
    create_page_tri(&app);
    create_page_listes(&app);

    // Placeholder pages for Trees and Graphs
    create_page_pile_file(&app);
    create_page_arbres(&app);
    create_page_graphes(&app);

    // Afficher tout
    gtk_widget_show_all(app.window);

    // Initialiser les données (ex: liste vide)
    init_list(&app.current_list, LISTE_SIMPLE, TYPE_INT);

    gtk_main();

    // Nettoyage final
    destroy_list(&app.current_list);
    if (app.sort_array) free(app.sort_array);

    return 0;
}
