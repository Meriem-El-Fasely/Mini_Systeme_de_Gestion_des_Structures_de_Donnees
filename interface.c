#include "interface.h"
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ===========================================
// FONCTIONS UTILITAIRES POUR L'INTERFACE
// ===========================================

void on_menu_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int page_index = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "page"));

    const char *page_names[] = {"accueil", "tri", "listes", "piles_files", "arbres", "graphes"};
    if (page_index >= 0 && page_index < 6) {
        gtk_stack_set_visible_child_name(GTK_STACK(app->stack), page_names[page_index]);
    }
}

// ===========================================
// STYLE ET SIDEBAR
// ===========================================

void apply_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    const char *css =
        "window { background-color: #F5F5F5; }"

        /* Sidebar */
        ".sidebar { background-color: #5B4FDB; color: white; }"
        ".sidebar-title { font-size: 32px; font-weight: bold; padding: 0px; }"
        ".sidebar-button { background: transparent; border: none; color: white; "
        "padding: 20px; text-align: left; font-size: 20px; }"
        ".sidebar-button:hover { background-color: rgba(255,255,255,0.1); }"
        ".sidebar-footer { color: white; padding: 20px; font-size: 20px; }"
        ".sidebar-footer-name { color: white; font-size: 23px; font-weight: bold;padding: 2px; }"

        /* Titres */
        ".welcome-title { font-size: 80px; font-weight: bold; color: #5B4FDB; }"
        ".welcome-subtitle { font-size: 30px; color: #333; margin-top: 10px; }"
        ".section-title { font-size: 18px; font-weight: bold; margin: 10px 0; color: #333; }"

        /* Boutons colorés */
        "button.btn-red { "
        "  background-color: #DC2626; color: white; "
        "  padding: 12px 24px; border-radius: 6px; "
        "  font-weight: bold; font-size: 14px; "
        "  border: none; background-image: none; "
        "  box-shadow: 0 2px 4px rgba(0,0,0,0.1); "
        "  transition: all 0.3s; "
        "}"
        "button.btn-red:hover { "
        "  background-color: #B91C1C; "
        "  box-shadow: 0 4px 6px rgba(0,0,0,0.15); "
        "}"

        "button.btn-green { "
        "  background-color: #10B981; color: white; "
        "  padding: 12px 24px; border-radius: 6px; "
        "  font-weight: bold; font-size: 14px; "
        "  border: none; background-image: none; "
        "  box-shadow: 0 2px 4px rgba(0,0,0,0.1); "
        "  transition: all 0.3s; "
        "}"
        "button.btn-green:hover { "
        "  background-color: #059669; "
        "  box-shadow: 0 4px 6px rgba(0,0,0,0.15); "
        "}"

        "button.btn-blue { "
        "  background-color: #3B82F6; color: white; "
        "  padding: 12px 24px; border-radius: 6px; "
        "  font-weight: bold; font-size: 14px; "
        "  border: none; background-image: none; "
        "  box-shadow: 0 2px 4px rgba(0,0,0,0.1); "
        "  transition: all 0.3s; "
        "}"
        "button.btn-blue:hover { "
        "  background-color: #2563EB; "
        "  box-shadow: 0 4px 6px rgba(0,0,0,0.15); "
        "}"

        "button.btn-orange { "
        "  background-color: #F97316; color: white; "
        "  padding: 12px 24px; border-radius: 6px; "
        "  font-weight: bold; font-size: 14px; "
        "  border: none; background-image: none; "
        "  box-shadow: 0 2px 4px rgba(0,0,0,0.1); "
        "  transition: all 0.3s; "
        "}"
        "button.btn-orange:hover { "
        "  background-color: #EA580C; "
        "  box-shadow: 0 4px 6px rgba(0,0,0,0.15); "
        "}"

        ".btn-generer { "
        "  background-color: #9CA3AF; color: white; "
        "  padding: 10px 20px; border-radius: 6px; "
        "  font-weight: bold; font-size: 13px; "
        "}";

    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}

// Création de la sidebar
void create_sidebar(AppData *app, GtkWidget *box) {
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(sidebar, 270, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "sidebar");

    // Titre
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(title_box, 20);
    gtk_widget_set_margin_end(title_box, 20);
    gtk_widget_set_margin_top(title_box, 30);
    gtk_widget_set_margin_bottom(title_box, 30);

    GtkWidget *title1 = gtk_label_new("STRUCTURE");
    GtkWidget *title2 = gtk_label_new("DE DONNÉES");
    gtk_style_context_add_class(gtk_widget_get_style_context(title1), "sidebar-title");
    gtk_style_context_add_class(gtk_widget_get_style_context(title2), "sidebar-title");
    gtk_box_pack_start(GTK_BOX(title_box), title1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(title_box), title2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar), title_box, FALSE, FALSE, 0);

    // Séparateur
    gtk_box_pack_start(GTK_BOX(sidebar), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 20);

    // Boutons menu
    const char *menu_items[] = {"Accueil", "Algorithmes de Tri", "Listes Chaînées", "Piles & Files", "Arbres", "Graphes"};
    for (int i = 0; i < 6; i++) {
        GtkWidget *btn = gtk_button_new_with_label(menu_items[i]);
        gtk_style_context_add_class(gtk_widget_get_style_context(btn), "sidebar-button");
        gtk_widget_set_halign(btn, GTK_ALIGN_FILL);
        g_object_set_data(G_OBJECT(btn), "page", GINT_TO_POINTER(i));
        g_signal_connect(btn, "clicked", G_CALLBACK(on_menu_clicked), app);
        gtk_box_pack_start(GTK_BOX(sidebar), btn, FALSE, FALSE, 0);
    }

    // Spacer
    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(sidebar), spacer, TRUE, TRUE, 0);

    // Footer
    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(footer, 20);
    gtk_widget_set_margin_bottom(footer, 20);

    GtkWidget *prep = gtk_label_new("Préparé par :");
    GtkWidget *name = gtk_label_new("Meriem El Fasely");
    GtkWidget *enc = gtk_label_new("Encadré par :");
    GtkWidget *prof = gtk_label_new("Pr F.Khoukhi");

    gtk_style_context_add_class(gtk_widget_get_style_context(footer), "sidebar-footer");

    gtk_style_context_add_class(gtk_widget_get_style_context(name), "sidebar-footer-name");
    gtk_style_context_add_class(gtk_widget_get_style_context(prof), "sidebar-footer-name");

    gtk_widget_set_halign(prep, GTK_ALIGN_START);
    gtk_widget_set_halign(name, GTK_ALIGN_START);
    gtk_widget_set_halign(enc, GTK_ALIGN_START);
    gtk_widget_set_halign(prof, GTK_ALIGN_START);

    gtk_box_pack_start(GTK_BOX(footer), prep, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(footer), name, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(footer), enc, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(footer), prof, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sidebar), footer, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), sidebar, FALSE, FALSE, 0);
}

// Page d'accueil
void create_page_accueil(AppData *app) {
    app->page_accueil = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *center = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(center, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(center, GTK_ALIGN_CENTER);

    GtkWidget *title = gtk_label_new("BIENVENUE");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "welcome-title");

    GtkWidget *subtitle = gtk_label_new("dans le système de gestion de structure de donnée");
    gtk_style_context_add_class(gtk_widget_get_style_context(subtitle), "welcome-subtitle");

    gtk_box_pack_start(GTK_BOX(center), title, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(center), subtitle, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(app->page_accueil), center, TRUE, TRUE, 0);

    gtk_stack_add_named(GTK_STACK(app->stack), app->page_accueil, "accueil");
}

// ===========================================
// PAGE LISTES CHAÃŽNÉES
// ===========================================

void update_list_display(AppData *app) {
    // Resize drawing area based on content
    int width = gtk_widget_get_allocated_width(app->drawing_area_liste);
    int height = get_list_drawing_height(&app->current_list, width);
    // Enforce minimum height or allow shrink? ScrolledWindow needs natural size.
    gtk_widget_set_size_request(app->drawing_area_liste, -1, height);

    gtk_widget_queue_draw(app->drawing_area_liste);
}

// Helpers for value parsing from Entry
static void get_value_from_entry(AppData *app, void *buffer) {
    const char *txt = gtk_entry_get_text(app->entry_liste_value);
    if(app->current_list.data_type == TYPE_INT) *(int*)buffer = atoi(txt);
    else if(app->current_list.data_type == TYPE_FLOAT) *(float*)buffer = atof(txt);
    else if(app->current_list.data_type == TYPE_CHAR) *(char*)buffer = txt[0];
    else if(app->current_list.data_type == TYPE_STRING) strcpy((char*)buffer, txt);
}

static void on_btn_generate_list_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    ListType ltype = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_type)) == 0 ? LISTE_SIMPLE : LISTE_DOUBLE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_data_type));

    // Clear old list
    destroy_list(&app->current_list);
    init_list(&app->current_list, ltype, dtype);

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->radio_liste_manual))) {
        // Manual Input Dialog
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Saisie Manuelle Liste", GTK_WINDOW(app->window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);

        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Ex: 10, 5, 8...");
        gtk_container_add(GTK_CONTAINER(content), entry);
        gtk_widget_show_all(dialog);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
            char *input_copy = strdup(text);
            char *token = strtok(input_copy, " ,;");

            while(token) {
                while(*token == ' ') token++; // trim leading space (extra safety)
                if (*token != '\0') {
                    if(dtype == TYPE_INT) { int v = atoi(token); insert_at_end(&app->current_list, &v); }
                    else if(dtype == TYPE_FLOAT) { float v = atof(token); insert_at_end(&app->current_list, &v); }
                    else if(dtype == TYPE_CHAR) { char v = token[0]; insert_at_end(&app->current_list, &v); }
                    else if(dtype == TYPE_STRING) {
                        char buf[MAX_STRING_LEN];
                        strncpy(buf, token, MAX_STRING_LEN - 1);
                        buf[MAX_STRING_LEN - 1] = '\0';
                        // strncpy pads with nulls if src is short, but let's be safe:
                        // Actually standard strncpy pads. If not, memset first.
                        memset(buf, 0, MAX_STRING_LEN);
                        strncpy(buf, token, MAX_STRING_LEN - 1);
                        insert_at_end(&app->current_list, buf);
                    }
                }
                token = strtok(NULL, " ,;");
            }
            free(input_copy);
        }
        gtk_widget_destroy(dialog);
    } else {
        // Random
        int count = gtk_spin_button_get_value_as_int(app->spin_liste_size);
        generate_random_list_data(&app->current_list, count);
    }
    update_list_display(app);
}

static void on_btn_insert_head_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    char buffer[MAX_STRING_LEN];
    get_value_from_entry(app, buffer);
    insert_at_beginning(&app->current_list, buffer);
    update_list_display(app);
}

static void on_btn_insert_end_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    char buffer[MAX_STRING_LEN];
    get_value_from_entry(app, buffer);
    insert_at_end(&app->current_list, buffer);
    update_list_display(app);
}

static void on_btn_insert_sorted_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    char buffer[MAX_STRING_LEN];
    get_value_from_entry(app, buffer);
    insert_sorted(&app->current_list, buffer);
    update_list_display(app);
}

static void on_btn_delete_pos_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->check_op_by_value))) {
        // By Value - Dialog
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Supprimer par Valeur", GTK_WINDOW(app->window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);
        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry_val = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry_val), "Valeur Ã  supprimer");
        gtk_container_add(GTK_CONTAINER(content), entry_val);
        gtk_widget_show_all(dialog);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            const char *txt = gtk_entry_get_text(GTK_ENTRY(entry_val));

            if(app->current_list.data_type == TYPE_INT) { int v=atoi(txt); delete_by_value(&app->current_list, &v); }
            else if(app->current_list.data_type == TYPE_FLOAT) { float v=atof(txt); delete_by_value(&app->current_list, &v); }
            else if(app->current_list.data_type == TYPE_CHAR) { char v=txt[0]; delete_by_value(&app->current_list, &v); }
            else if(app->current_list.data_type == TYPE_STRING) delete_by_value(&app->current_list, txt);
        }
        gtk_widget_destroy(dialog);
    } else {
        // By Pos
        int pos = gtk_spin_button_get_value_as_int(app->spin_liste_pos);
        delete_at_position(&app->current_list, pos);
    }
    update_list_display(app);
}

static void on_btn_clear_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    destroy_list(&app->current_list);
    ListType ltype = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_type)) == 0 ? LISTE_SIMPLE : LISTE_DOUBLE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_data_type));
    init_list(&app->current_list, ltype, dtype);
    update_list_display(app);
}

static void on_btn_modify_pos_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->check_op_by_value))) {

        GtkWidget *dialog = gtk_dialog_new_with_buttons("Modifier par Valeur", GTK_WINDOW(app->window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);
        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_container_add(GTK_CONTAINER(content), box);

        GtkWidget *lbl_old = gtk_label_new("Valeur Ã  remplacer :");
        GtkWidget *entry_old = gtk_entry_new();
        GtkWidget *lbl_new = gtk_label_new("Nouvelle Valeur :");
        GtkWidget *entry_new = gtk_entry_new();

        gtk_box_pack_start(GTK_BOX(box), lbl_old, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), entry_old, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), lbl_new, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(box), entry_new, FALSE, FALSE, 0);

        gtk_widget_show_all(dialog);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            const char *txt_old = gtk_entry_get_text(GTK_ENTRY(entry_old));
            const char *txt_new = gtk_entry_get_text(GTK_ENTRY(entry_new));

            char buf_old[MAX_STRING_LEN];
            char buf_new[MAX_STRING_LEN];

            if(app->current_list.data_type == TYPE_INT) {
                *(int*)buf_old = atoi(txt_old);
                *(int*)buf_new = atoi(txt_new);
            } else if(app->current_list.data_type == TYPE_FLOAT) {
                *(float*)buf_old = atof(txt_old);
                *(float*)buf_new = atof(txt_new);
            } else if(app->current_list.data_type == TYPE_CHAR) {
                *(char*)buf_old = txt_old[0];
                *(char*)buf_new = txt_new[0];
            } else if(app->current_list.data_type == TYPE_STRING) {
                strcpy(buf_old, txt_old);
                strcpy(buf_new, txt_new);
            }

            modify_by_value(&app->current_list, buf_old, buf_new);
        }
        gtk_widget_destroy(dialog);
    } else {
        // By Pos
        int pos = gtk_spin_button_get_value_as_int(app->spin_liste_pos);
        char buffer[MAX_STRING_LEN];
        get_value_from_entry(app, buffer);
        modify_at_position(&app->current_list, buffer, pos);
    }
    update_list_display(app);
}

static void on_btn_sort_list_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int algo = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_algo));
    if (algo == 0) sort_list_bubble(&app->current_list);
    else if (algo == 1) sort_linked_list(&app->current_list); // Insertion
    else if (algo == 2) sort_list_shell(&app->current_list);  // Shell
    else if (algo == 3) sort_list_quick(&app->current_list);  // Quick
    update_list_display(app);
}

static void on_btn_reset_list_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    destroy_list(&app->current_list);
    // Re-init with same settings
    ListType ltype = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_type)) == 0 ? LISTE_SIMPLE : LISTE_DOUBLE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_liste_data_type));
    init_list(&app->current_list, ltype, dtype);
    update_list_display(app);
}

void create_page_listes(AppData *app) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 10);

    // Title
    GtkWidget *title = gtk_label_new("Gestion des Listes Chaînées");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 5);

    // --- Frame 1: Configuration & Generation ---
    GtkWidget *frame_config = gtk_frame_new("Configuration");
    GtkWidget *box_config = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_config), 5);
    gtk_container_add(GTK_CONTAINER(frame_config), box_config);

    app->combo_liste_type = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_liste_type, "Liste Simple");
    gtk_combo_box_text_append_text(app->combo_liste_type, "Liste Double");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_liste_type), 0);

    app->combo_liste_data_type = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_liste_data_type, "Entier");
    gtk_combo_box_text_append_text(app->combo_liste_data_type, "Réel");
    gtk_combo_box_text_append_text(app->combo_liste_data_type, "Caractère");
    gtk_combo_box_text_append_text(app->combo_liste_data_type, "Chaîne");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_liste_data_type), 0);

    app->radio_liste_random = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label(NULL, "Aléatoire"));
    app->radio_liste_manual = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label_from_widget(app->radio_liste_random, "Manuel"));

    app->spin_liste_size = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(1, 10000, 1));
    gtk_spin_button_set_value(app->spin_liste_size, 5);

    GtkWidget *btn_gen = gtk_button_new_with_label("Générer");
    GtkWidget *btn_reset = gtk_button_new_with_label("Réinitialiser");
    g_signal_connect(btn_gen, "clicked", G_CALLBACK(on_btn_generate_list_clicked), app);
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_btn_reset_list_clicked), app);

    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Type:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->combo_liste_type), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Donnée:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->combo_liste_data_type), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->radio_liste_random), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->radio_liste_manual), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Taille:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->spin_liste_size), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), btn_gen, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), btn_reset, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_config, FALSE, FALSE, 5);

    // --- Frame 2: Operations ---
    GtkWidget *frame_ops = gtk_frame_new("Opérations");
    GtkWidget *box_ops = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(box_ops), 5);
    gtk_container_add(GTK_CONTAINER(frame_ops), box_ops);

    // Row 1: Insertions
    GtkWidget *row_insert = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    app->entry_liste_value = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(app->entry_liste_value, "Valeur");
    gtk_widget_set_size_request(GTK_WIDGET(app->entry_liste_value), 80, -1);

    GtkWidget *btn_ins_head = gtk_button_new_with_label("Insérer Début");
    GtkWidget *btn_ins_tail = gtk_button_new_with_label("Insérer Fin");
    GtkWidget *btn_ins_sort = gtk_button_new_with_label("Insérer Trié");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_ins_head), "btn-green");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_ins_tail), "btn-green");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_ins_sort), "btn-blue");

    g_signal_connect(btn_ins_head, "clicked", G_CALLBACK(on_btn_insert_head_clicked), app);
    g_signal_connect(btn_ins_tail, "clicked", G_CALLBACK(on_btn_insert_end_clicked), app);
    g_signal_connect(btn_ins_sort, "clicked", G_CALLBACK(on_btn_insert_sorted_clicked), app);

    gtk_box_pack_start(GTK_BOX(row_insert), gtk_label_new("Valeur:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_insert), GTK_WIDGET(app->entry_liste_value), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_insert), btn_ins_head, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_insert), btn_ins_tail, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_insert), btn_ins_sort, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_ops), row_insert, FALSE, FALSE, 0);

    // Row 2: Modify/Delete & Sort
    GtkWidget *row_mod = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    app->spin_liste_pos = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(0, 100, 1));
    app->check_op_by_value = GTK_CHECK_BUTTON(gtk_check_button_new_with_label("Par Valeur"));

    GtkWidget *btn_del = gtk_button_new_with_label("Supprimer");
    GtkWidget *btn_mod = gtk_button_new_with_label("Modifier");
    GtkWidget *btn_clear = gtk_button_new_with_label("Vider");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_del), "btn-red");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_mod), "btn-orange");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_clear), "btn-red");

    g_signal_connect(btn_del, "clicked", G_CALLBACK(on_btn_delete_pos_clicked), app);
    g_signal_connect(btn_mod, "clicked", G_CALLBACK(on_btn_modify_pos_clicked), app);
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_btn_clear_clicked), app);

    gtk_box_pack_start(GTK_BOX(row_mod), gtk_label_new("Position:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_mod), GTK_WIDGET(app->spin_liste_pos), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_mod), GTK_WIDGET(app->check_op_by_value), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_mod), btn_del, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(row_mod), btn_mod, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(row_mod), btn_clear, FALSE, FALSE, 5);

    // Separator
    gtk_box_pack_start(GTK_BOX(row_mod), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 10);

    // Sort
    app->combo_liste_algo = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_liste_algo, "Tri Bulle");
    gtk_combo_box_text_append_text(app->combo_liste_algo, "Tri Insertion");
    gtk_combo_box_text_append_text(app->combo_liste_algo, "Tri Shell");
    gtk_combo_box_text_append_text(app->combo_liste_algo, "Tri Rapide");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_liste_algo), 0);

    GtkWidget *btn_sort = gtk_button_new_with_label("Trier");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_sort), "btn-blue");
    g_signal_connect(btn_sort, "clicked", G_CALLBACK(on_btn_sort_list_clicked), app);

    gtk_box_pack_start(GTK_BOX(row_mod), gtk_label_new("Algo:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_mod), GTK_WIDGET(app->combo_liste_algo), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_mod), btn_sort, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box_ops), row_mod, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), frame_ops, FALSE, FALSE, 5);

    // --- Drawing Area in ScrolledWindow ---
    GtkWidget *scrolled_liste = gtk_scrolled_window_new(NULL, NULL);
    app->drawing_area_liste = gtk_drawing_area_new();
    gtk_widget_set_size_request(app->drawing_area_liste, -1, 400); // Initial size
    g_signal_connect(app->drawing_area_liste, "draw", G_CALLBACK(draw_linked_list), &app->current_list);

    gtk_container_add(GTK_CONTAINER(scrolled_liste), app->drawing_area_liste);

    GtkWidget *frame_viz = gtk_frame_new("Visualisation");
    gtk_container_add(GTK_CONTAINER(frame_viz), scrolled_liste);
    gtk_box_pack_start(GTK_BOX(page), frame_viz, TRUE, TRUE, 0);

    gtk_stack_add_named(GTK_STACK(app->stack), page, "listes");
}


// ===========================================
// PAGE ALGORITHMES DE TRI
// ===========================================

// --- Utilities ---
static void free_curve_data(AppData *app) {
    for(int i=0; i<4; i++) {
        if(app->curve_times[i]) { free(app->curve_times[i]); app->curve_times[i] = NULL; }
    }
    if(app->curve_sizes) { free(app->curve_sizes); app->curve_sizes = NULL; }
    app->curve_points = 0;
}

static void free_sort_array(AppData *app) {
    if (app->sort_array) {
        if (app->sort_data_type == TYPE_STRING) {
             char **arr = (char **)app->sort_array;
             // Assume we own strings for now
             for(int i=0; i<app->sort_array_size; i++) free(arr[i]);
             free(arr);
        } else {
            free(app->sort_array);
        }
        app->sort_array = NULL;
    }
}

static void append_to_log(AppData *app, const char *msg) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(app->text_buffer_data, &end);
    gtk_text_buffer_insert(app->text_buffer_data, &end, msg, -1);
    gtk_text_buffer_insert(app->text_buffer_data, &end, "\n", -1);
}

static void display_array(AppData *app, const char *title) {
    if (!app->sort_array) { g_print("Display Array: No Data\n"); return; }

    g_print("Displaying Array: %s\n", title);

    append_to_log(app, "-------------------");
    append_to_log(app, title);
    append_to_log(app, "-------------------");


    int limit = 2000;
    int n = app->sort_array_size;

    char val_buf[64];

    for(int i=0; i<n && i<limit; i++) {
        if(app->sort_data_type == TYPE_INT) snprintf(val_buf, 64, "%d, ", ((int*)app->sort_array)[i]);
        else if(app->sort_data_type == TYPE_FLOAT) snprintf(val_buf, 64, "%.2f, ", ((float*)app->sort_array)[i]);
        else if(app->sort_data_type == TYPE_CHAR) snprintf(val_buf, 64, "%c, ", ((char*)app->sort_array)[i]);
        else if(app->sort_data_type == TYPE_STRING) snprintf(val_buf, 64, "%s, ", ((char**)app->sort_array)[i]);

        GtkTextIter end;
        gtk_text_buffer_get_end_iter(app->text_buffer_data, &end);
        gtk_text_buffer_insert(app->text_buffer_data, &end, val_buf, -1);
    }
    if(n > limit) {
        char msg[64];
        snprintf(msg, 64, "... (+%d éléments cachés)", n - limit);
        append_to_log(app, msg);
    }
    append_to_log(app, "");
}

// --- Logic ---

static void run_curve_benchmark(AppData *app) {
    free_curve_data(app);

    int points = 10;
    app->curve_points = points;
    app->curve_sizes = malloc(points * sizeof(int));
    for(int i=0; i<4; i++) app->curve_times[i] = malloc(points * sizeof(double));

    // Dynamic range: from 100 up to user selected size
    int max_size = gtk_spin_button_get_value_as_int(app->spin_size);
    if(max_size < 100) max_size = 100; // Minimum safety

    int start_size = max_size / points;
    if (start_size < 10) start_size = 10;

    int step = (max_size - start_size) / (points - 1);
    if(step < 1) step = 1;

    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_type));

    append_to_log(app, "Calcul des courbes en cours...");
    while (gtk_events_pending()) gtk_main_iteration(); // Force UI update

    for(int p=0; p<points; p++) {
        int n = start_size + p * step;
        app->curve_sizes[p] = n;

        // Generate temporary data
        void *data = NULL;
        size_t sz = 0;

        if (dtype == TYPE_INT) {
            sz = sizeof(int);
            int *arr = malloc(n * sz);
            for(int i=0; i<n; i++) arr[i] = rand() % 100000;
            data = arr;
        } else if (dtype == TYPE_FLOAT) {
            sz = sizeof(float);
            float *arr = malloc(n * sz);
            for(int i=0; i<n; i++) arr[i] = (float)rand()/(float)(RAND_MAX/100000.0);
            data = arr;
        } else if (dtype == TYPE_CHAR) {
            sz = sizeof(char);
            char *arr = malloc(n * sz);
            for(int i=0; i<n; i++) arr[i] = 'A' + (rand() % 26);
            data = arr;
        } else { // String
             sz = sizeof(char*);
             char **arr = malloc(n * sz);
             for(int i=0; i<n; i++) {
                 int len = 4 + rand() % 7;
                 arr[i] = malloc(len+1);
                 for(int k=0; k<len; k++) arr[i][k] = 'a' + (rand()%26);
                 arr[i][len] = '\0';
             }
             data = arr;
        }

        // Run Sorts
        TriStats stats;
        void *temp = malloc(n * sz);

        // Bulle
        memcpy(temp, data, n * sz);
        if(dtype == TYPE_INT) tri_bulle(temp, n, &stats);
        else if(dtype == TYPE_FLOAT) tri_bulle_float(temp, n, &stats);
        else if(dtype == TYPE_CHAR) tri_bulle_char(temp, n, &stats);
        else if(dtype == TYPE_STRING) tri_bulle_string(temp, n, &stats);
        app->curve_times[0][p] = stats.time_taken;

        // Insert
        memcpy(temp, data, n * sz);
        if(dtype == TYPE_INT) tri_insertion(temp, n, &stats);
        else if(dtype == TYPE_FLOAT) tri_insertion_float(temp, n, &stats);
        else if(dtype == TYPE_CHAR) tri_insertion_char(temp, n, &stats);
        else if(dtype == TYPE_STRING) tri_insertion_string(temp, n, &stats);
        app->curve_times[1][p] = stats.time_taken;

        // Shell
        memcpy(temp, data, n * sz);
        if(dtype == TYPE_INT) tri_shell(temp, n, &stats);
        else if(dtype == TYPE_FLOAT) tri_shell_float(temp, n, &stats);
        else if(dtype == TYPE_CHAR) tri_shell_char(temp, n, &stats);
        else if(dtype == TYPE_STRING) tri_shell_string(temp, n, &stats);
        app->curve_times[2][p] = stats.time_taken;

        // Quick
        memcpy(temp, data, n * sz);
        if(dtype == TYPE_INT) tri_quick_wrapper(temp, n, &stats);
        else if(dtype == TYPE_FLOAT) tri_quick_wrapper_float(temp, n, &stats);
        else if(dtype == TYPE_CHAR) tri_quick_wrapper_char(temp, n, &stats);
        else if(dtype == TYPE_STRING) tri_quick_wrapper_string(temp, n, &stats);
        app->curve_times[3][p] = stats.time_taken;

        free(temp);

        if (dtype == TYPE_STRING) {
            char **d = (char**)data;
            for(int k=0; k<n; k++) free(d[k]);
        }
        free(data);
    }
    append_to_log(app, "Calcul terminé.");
}

// Draw Callback for Curve (Enhanced)
gboolean on_draw_curve(GtkWidget *widget, cairo_t *cr, gpointer data) {
    AppData *app = (AppData *)data;

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    // Background
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    if (app->curve_points == 0) {
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 16);
        cairo_move_to(cr, width/2 - 80, height/2);
        cairo_show_text(cr, "Graphique (Lancer 'Voir Courbes')");
        return FALSE;
    }

    // Margins
    double ml = 60.0, mr = 40.0, mt = 40.0, mb = 50.0;
    double draw_w = width - ml - mr;
    double draw_h = height - mt - mb;

    double max_time = 0.0000001;
    for(int i=0; i<4; i++) for(int p=0; p<app->curve_points; p++) if(app->curve_times[i][p] > max_time) max_time = app->curve_times[i][p];

    // Grid
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
    cairo_set_line_width(cr, 1);

    // Vertical grid lines
    int v_grid = 5;
    for(int i=0; i<=v_grid; i++) {
        double x = ml + (draw_w * i / v_grid);
        cairo_move_to(cr, x, mt);
        cairo_line_to(cr, x, height - mb);
    }
    // Horizontal grid lines
    int h_grid = 5;
    for(int i=0; i<=h_grid; i++) {
        double y = mt + (draw_h * i / h_grid);
        cairo_move_to(cr, ml, y);
        cairo_line_to(cr, width - mr, y);
    }
    cairo_stroke(cr);

    // Axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.5);
    cairo_move_to(cr, ml, mt);
    cairo_line_to(cr, ml, height - mb); // Y
    cairo_line_to(cr, width - mr, height - mb); // X
    cairo_stroke(cr);

    // Axis Labels
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 12);

    // X Label
    cairo_move_to(cr, width/2 - 40, height - 15);
    cairo_show_text(cr, "Taille des données");

    // Y Label (rotated)
    cairo_save(cr);
    cairo_move_to(cr, 15, height/2 + 40);
    cairo_rotate(cr, -3.14159/2);
    cairo_show_text(cr, "Temps (ms)");
    cairo_restore(cr);

    // X Axis ticks (Sizes)
    cairo_set_font_size(cr, 10);
    for(int i=0; i<=v_grid; i++) {
         char buf[32];

         int min_sz = app->curve_sizes[0];
         int max_sz = app->curve_sizes[app->curve_points-1];
         int sz = min_sz + (max_sz - min_sz) * i / v_grid;
         // Format roughly K
         if(sz >= 1000) snprintf(buf, 32, "%.1fK", sz/1000.0);
         else snprintf(buf, 32, "%d", sz);

         double x = ml + (draw_w * i / v_grid) - 10;
         cairo_move_to(cr, x, height - mb + 15);
         cairo_show_text(cr, buf);
    }

    // Y Axis ticks (Times ms)
    for(int i=0; i<=h_grid; i++) {
         char buf[32];

         double val = max_time - (max_time * i / h_grid);
         // If val < 0 (float error), clamp
         if(val < 0) val = 0;
         snprintf(buf, 32, "%.1f", val * 1000.0); // Show in ms

         double y = mt + (draw_h * i / h_grid) + 3;
         cairo_move_to(cr, 20, y);
         cairo_show_text(cr, buf);
    }

    // Curves
    double colors[4][3] = {{1,0,0}, {0,0,1}, {0,1,0}, {1,0.8,0}}; // R, B, G, Y

    cairo_set_line_width(cr, 2.0);
    for(int i=0; i<4; i++) {
        cairo_set_source_rgb(cr, colors[i][0], colors[i][1], colors[i][2]);

        double end_x = 0, end_y = 0;

        for(int p=0; p<app->curve_points; p++) {
            double x = ml + (double)p / (app->curve_points - 1) * draw_w;
            double y = (height - mb) - (app->curve_times[i][p] / max_time) * draw_h;

            if (p==0) cairo_move_to(cr, x, y);
            else cairo_line_to(cr, x, y);

            if(p == app->curve_points-1) { end_x=x; end_y=y; }
        }
        cairo_stroke(cr);

        // Draw End Point
        cairo_arc(cr, end_x, end_y, 4, 0, 2*3.14159);
        cairo_fill(cr);

        // Draw End Label
        char lbl[32];
        snprintf(lbl, 32, "%.2f ms", app->curve_times[i][app->curve_points-1]*1000.0);
        cairo_move_to(cr, end_x - 40, end_y - 10);
        cairo_show_text(cr, lbl);
    }

    return TRUE;
}

// --- Button Actions ---

static void on_btn_reset_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    free_sort_array(app);
    free_curve_data(app);
    gtk_text_buffer_set_text(app->text_buffer_data, "", -1);
    gtk_widget_queue_draw(app->drawing_area_curve);
}

static void on_btn_gen_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    gtk_text_buffer_set_text(app->text_buffer_data, "", -1); // Clear log
    g_print("Generation Clicked!\n");
    free_sort_array(app);

    app->sort_data_type = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_type));

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->radio_manual))) {
        // Mode Manuel: Prompt user via Dialog
        GtkWidget *dialog = gtk_dialog_new_with_buttons("Saisie Manuelle", GTK_WINDOW(app->window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);

        GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Ex: 10, 5, 8, 2...");
        gtk_container_add(GTK_CONTAINER(content), entry);
        gtk_widget_show_all(dialog);

        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
            const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
             // Parse
             // ... Similar parsing logic as before
             int n = 1; for(const char *c = text; *c; c++) if(*c == ',') n++;
             app->sort_array_size = n;
             char *input_copy = strdup(text);
             char *token = strtok(input_copy, ",");
             int i = 0;
             // Alloc & fill based on type...
             if(app->sort_data_type == TYPE_INT) {
                 int *dat = malloc(n*sizeof(int));
                 while(token && i<n) { dat[i++] = atoi(token); token = strtok(NULL, ","); }
                 app->sort_array = dat;
             } else if(app->sort_data_type == TYPE_FLOAT) {
                 float *dat = malloc(n*sizeof(float));
                 while(token && i<n) { dat[i++] = atof(token); token = strtok(NULL, ","); }
                 app->sort_array = dat;
             } else if(app->sort_data_type == TYPE_CHAR) {
                 char *dat = malloc(n*sizeof(char));
                 while(token && i<n) { while(*token==' ') token++; dat[i++] = token[0]; token=strtok(NULL,","); }
                 app->sort_array = dat;
             } else { // String
                 char **dat = malloc(n*sizeof(char*));
                 while(token && i<n) { while(*token==' ') token++; dat[i++] = strdup(token); token=strtok(NULL,","); }
                 app->sort_array = dat;
             }
             free(input_copy);
             gtk_spin_button_set_value(app->spin_size, n);
             append_to_log(app, "Données manuelles chargées.");
             display_array(app, "Tableau Initial");
        }
        gtk_widget_destroy(dialog);
    } else {
        // Random Unique (Fisher-Yates) for Integers
        int n = gtk_spin_button_get_value_as_int(app->spin_size);
        app->sort_array_size = n;
        srand(time(NULL));

        if (app->sort_data_type == TYPE_INT) {
            // Pool-based generation for Uniqueness + Large Numbers
            int pool_size = (n > 100000) ? n : 100000;
            int *pool = malloc(pool_size * sizeof(int));
            for(int i=0; i<pool_size; i++) pool[i] = i + 1;

            // Partial Shuffle
            for (int i = 0; i < n; i++) {
                int j = i + rand() % (pool_size - i);
                int temp = pool[i];
                pool[i] = pool[j];
                pool[j] = temp;
            }

            int *arr = malloc(n * sizeof(int));
            memcpy(arr, pool, n * sizeof(int));
            free(pool);

            app->sort_array = arr;
        } else if (app->sort_data_type == TYPE_FLOAT) {
            float *arr = malloc(n * sizeof(float));
            for(int i=0; i<n; i++) arr[i] = (float)rand()/(float)(RAND_MAX/100000.0);
            app->sort_array = arr;
        } else if (app->sort_data_type == TYPE_CHAR) {
            char *arr = malloc(n * sizeof(char));
            for(int i=0; i<n; i++) arr[i] = 'A' + (rand() % 26);
            app->sort_array = arr;
        } else if (app->sort_data_type == TYPE_STRING) {
            char **arr = malloc(n * sizeof(char*));
            for(int i=0; i<n; i++) {
                 int len = 4 + rand() % 7;
                 arr[i] = malloc(len+1);
                 for(int k=0; k<len; k++) arr[i][k] = 'a' + (rand()%26);
                 arr[i][len] = '\0';
            }
            app->sort_array = arr;
        }
        append_to_log(app, "Données aléatoires générées.");
        g_print("Calling display_array... N=%d\n", n);
        display_array(app, "Tableau Initial");
    }
}

// Single Sort
static void run_one_sort(AppData *app, int algo_idx, gboolean display_res) {
    if(!app->sort_array) { append_to_log(app, "Aucune donnée ! Cliquez sur Générer."); return; }

    int n = app->sort_array_size;
    size_t sz = 0;
    if(app->sort_data_type == TYPE_INT) sz=sizeof(int);
    else if(app->sort_data_type == TYPE_FLOAT) sz=sizeof(float);
    else if(app->sort_data_type == TYPE_CHAR) sz=sizeof(char);

    void *temp;
    if (app->sort_data_type == TYPE_STRING) {
        char **src = (char**)app->sort_array;
        char **dst = malloc(n * sizeof(char*));
        for(int i=0; i<n; i++) dst[i] = strdup(src[i]);
        temp = dst;
    } else {
        temp = malloc(n * sz);
        memcpy(temp, app->sort_array, n * sz);
    }

    TriStats *stats = &app->tri_stats[algo_idx];

    if (algo_idx == 0) {
        if(app->sort_data_type == TYPE_INT) tri_bulle(temp, n, stats);
        else if(app->sort_data_type == TYPE_FLOAT) tri_bulle_float(temp, n, stats);
        else if(app->sort_data_type == TYPE_CHAR) tri_bulle_char(temp, n, stats);
        else if(app->sort_data_type == TYPE_STRING) tri_bulle_string(temp, n, stats);
        append_to_log(app, "Tri Bulle terminé.");
    }
    else if (algo_idx == 1) {
        if(app->sort_data_type == TYPE_INT) tri_insertion(temp, n, stats);
        else if(app->sort_data_type == TYPE_FLOAT) tri_insertion_float(temp, n, stats);
        else if(app->sort_data_type == TYPE_CHAR) tri_insertion_char(temp, n, stats);
        else if(app->sort_data_type == TYPE_STRING) tri_insertion_string(temp, n, stats);
        append_to_log(app, "Tri Insertion terminé.");
    }
    else if (algo_idx == 2) {
        if(app->sort_data_type == TYPE_INT) tri_shell(temp, n, stats);
        else if(app->sort_data_type == TYPE_FLOAT) tri_shell_float(temp, n, stats);
        else if(app->sort_data_type == TYPE_CHAR) tri_shell_char(temp, n, stats);
        else if(app->sort_data_type == TYPE_STRING) tri_shell_string(temp, n, stats);
        append_to_log(app, "Tri Shell terminé.");
    }
    else if (algo_idx == 3) {
        if(app->sort_data_type == TYPE_INT) tri_quick_wrapper(temp, n, stats);
        else if(app->sort_data_type == TYPE_FLOAT) tri_quick_wrapper_float(temp, n, stats);
        else if(app->sort_data_type == TYPE_CHAR) tri_quick_wrapper_char(temp, n, stats);
        else if(app->sort_data_type == TYPE_STRING) tri_quick_wrapper_string(temp, n, stats);
        append_to_log(app, "Tri Quick terminé.");
    }

    char buf[128];
    snprintf(buf, 128, "Temps exécution: %.2f ms", stats->time_taken * 1000.0);
    append_to_log(app, buf);

    if (display_res) {
        // Swap to display sorted temp
        void *original = app->sort_array;
        app->sort_array = temp;
        display_array(app, "Tableau Trié");
        app->sort_array = original; // Restore
    }

    if (app->sort_data_type == TYPE_STRING) {
        char **dst = (char**)temp;
        for(int i=0; i<n; i++) free(dst[i]);
        free(dst);
    } else {
        free(temp);
    }
}

static void on_btn_launch_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    gtk_text_buffer_set_text(app->text_buffer_data, "", -1); // Clear log
    int algo = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_algorithm));
    if(algo < 0) return;
    run_one_sort(app, algo, TRUE);
}

static void on_btn_compare_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    gtk_text_buffer_set_text(app->text_buffer_data, "", -1); // Clear log
    append_to_log(app, "--- Comparaison ---");
    for(int i=0; i<4; i++) run_one_sort(app, i, FALSE);
}

static void on_btn_curves_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    run_curve_benchmark(app);
    gtk_widget_queue_draw(app->drawing_area_curve);
}


void create_page_tri(AppData *app) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 10);

    // 1. Paramétrage (Top)
    GtkWidget *frame_config = gtk_frame_new("Paramétrage du Tri");
    GtkWidget *hbox_config = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    gtk_container_set_border_width(GTK_CONTAINER(hbox_config), 10);
    gtk_container_add(GTK_CONTAINER(frame_config), hbox_config);

    // Type
    app->combo_type = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_type, "Entiers");
    gtk_combo_box_text_append_text(app->combo_type, "Réels");
    gtk_combo_box_text_append_text(app->combo_type, "Caractères");
    gtk_combo_box_text_append_text(app->combo_type, "Chaîne");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_type), 0);

    // Algo
    app->combo_algorithm = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_algorithm, "Bulle");
    gtk_combo_box_text_append_text(app->combo_algorithm, "Insertion");
    gtk_combo_box_text_append_text(app->combo_algorithm, "Shell");
    gtk_combo_box_text_append_text(app->combo_algorithm, "Quicksort");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_algorithm), 0);

    // Mode (Radio)
    app->radio_random = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label(NULL, "Aléatoire"));
    app->radio_manual = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label_from_widget(app->radio_random, "Manuel"));

    // Taille
    app->spin_size = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(10, 100000, 10));
    gtk_spin_button_set_value(app->spin_size, 100);

    // Gen Button
    GtkWidget *btn_gen = gtk_button_new_with_label("Générer");
    g_signal_connect(btn_gen, "clicked", G_CALLBACK(on_btn_gen_clicked), app);

    gtk_box_pack_start(GTK_BOX(hbox_config), gtk_label_new("Type:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), GTK_WIDGET(app->combo_type), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), gtk_label_new("Algo:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), GTK_WIDGET(app->combo_algorithm), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), GTK_WIDGET(app->radio_random), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), GTK_WIDGET(app->radio_manual), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), gtk_label_new("Taille:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), GTK_WIDGET(app->spin_size), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_config), btn_gen, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_config, FALSE, FALSE, 0);

    // 2. Main Content (Left: Data/Results, Right: Curve)
    GtkWidget *hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_size_request(hpaned, -1, 400);

    // Left
    GtkWidget *frame_data = gtk_frame_new("Données / Résultats");
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    app->text_view_data = GTK_TEXT_VIEW(gtk_text_view_new());
    app->text_buffer_data = gtk_text_view_get_buffer(app->text_view_data);
    gtk_text_view_set_editable(app->text_view_data, FALSE); // Read only log
    gtk_text_view_set_wrap_mode(app->text_view_data, GTK_WRAP_WORD_CHAR); // Enable wrapping
    gtk_widget_show(GTK_WIDGET(app->text_view_data)); // Explicit show
    gtk_container_add(GTK_CONTAINER(scrolled), GTK_WIDGET(app->text_view_data));
    gtk_container_add(GTK_CONTAINER(frame_data), scrolled);
    gtk_paned_add1(GTK_PANED(hpaned), frame_data);

    // Right
    GtkWidget *frame_curve = gtk_frame_new("Courbe de Performance");
    GtkWidget *vbox_curve = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Legend
    GtkWidget *lbl_legend = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_legend),
        "<span foreground='#FF0000'><b>Bulle</b></span> | "
        "<span foreground='#0000FF'><b>Insertion</b></span> | "
        "<span foreground='#00AA00'><b>Shell</b></span> | "
        "<span foreground='#FFA500'><b>Quick</b></span>");
    gtk_box_pack_start(GTK_BOX(vbox_curve), lbl_legend, FALSE, FALSE, 5);

    app->drawing_area_curve = gtk_drawing_area_new();
    g_signal_connect(app->drawing_area_curve, "draw", G_CALLBACK(on_draw_curve), app);
    gtk_box_pack_start(GTK_BOX(vbox_curve), app->drawing_area_curve, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(frame_curve), vbox_curve);
    gtk_paned_add2(GTK_PANED(hpaned), frame_curve);

    gtk_paned_set_position(GTK_PANED(hpaned), 400); // Initial split

    gtk_box_pack_start(GTK_BOX(page), hpaned, TRUE, TRUE, 10);

    // 3. Controls (Bottom)
    GtkWidget *hbox_ctrl = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_widget_set_halign(hbox_ctrl, GTK_ALIGN_CENTER);

    app->btn_reset = gtk_button_new_with_label("RÉINITIALISER");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_reset), "btn-red");
    g_signal_connect(app->btn_reset, "clicked", G_CALLBACK(on_btn_reset_clicked), app);

    app->btn_launch = gtk_button_new_with_label("LANCER LE TRI");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_launch), "btn-green");
    g_signal_connect(app->btn_launch, "clicked", G_CALLBACK(on_btn_launch_clicked), app);

    app->btn_compare = gtk_button_new_with_label("COMPARER TOUT");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_compare), "btn-blue");
    g_signal_connect(app->btn_compare, "clicked", G_CALLBACK(on_btn_compare_clicked), app);

    app->btn_curves = gtk_button_new_with_label("VOIR COURBES");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_curves), "btn-orange");
    g_signal_connect(app->btn_curves, "clicked", G_CALLBACK(on_btn_curves_clicked), app);

    gtk_box_pack_start(GTK_BOX(hbox_ctrl), app->btn_reset, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_ctrl), app->btn_launch, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_ctrl), app->btn_compare, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_ctrl), app->btn_curves, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), hbox_ctrl, FALSE, FALSE, 10);

    gtk_stack_add_named(GTK_STACK(app->stack), page, "tri");
}



// ===========================================
// PAGE ARBRES
// ===========================================

static void update_tree_display(AppData *app) {
    int w, h;
    get_tree_visualization_size(&app->current_tree, &w, &h);
    // Ensure min size matches viewport if small
    if (w < 400) w = 400;
    if (h < 400) h = 400;

    gtk_widget_set_size_request(app->drawing_area_tree, w, h);
    gtk_widget_queue_draw(app->drawing_area_tree);
}

static void tree_log(AppData *app, const char *msg) {
    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_tree_log);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buf, &end);
    gtk_text_buffer_insert(buf, &end, msg, -1);
    gtk_text_buffer_insert(buf, &end, "\n", -1);
}

// Forward Declaration
static void stop_tree_animation(AppData *app);

static void on_btn_tree_gen_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app); // Stop pending animation to avoid crash
    destroy_tree(&app->current_tree);

    int type_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_type));
    TreeType ttype = (type_idx == 0) ? ARBRE_BINAIRE : ARBRE_NAIRE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_dtype));

    init_tree(&app->current_tree, ttype, dtype);



    // Let's implement Random Generation here.
    int count = gtk_spin_button_get_value_as_int(app->spin_tree_size);
    for(int i=0; i<count; i++) {
        if (dtype == TYPE_INT) { int v = rand()%100; insert_node_tree(&app->current_tree, &v); }
        else if (dtype == TYPE_FLOAT) { float v = (float)(rand()%1000)/10.0; insert_node_tree(&app->current_tree, &v); }
        else if (dtype == TYPE_CHAR) { char v = 'A' + rand()%26; insert_node_tree(&app->current_tree, &v); }
        else if (dtype == TYPE_STRING) {
             char buf[32]; sprintf(buf, "N%d", i);
             insert_node_tree(&app->current_tree, buf);
        }
    }
    update_tree_display(app);
    tree_log(app, "Arbre généré (Aléatoire).");
}

static void on_btn_tree_manual_input_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);
    destroy_tree(&app->current_tree);

    int type_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_type));
    TreeType ttype = (type_idx == 0) ? ARBRE_BINAIRE : ARBRE_NAIRE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_dtype));

    init_tree(&app->current_tree, ttype, dtype);

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Saisie Manuelle Arbre", GTK_WINDOW(app->window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Ex: 10, 5, 8... (Racine en premier)");
    gtk_container_add(GTK_CONTAINER(content), entry);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        char *input_copy = strdup(text);
        char *token = strtok(input_copy, " ,;");

        while(token) {
            while(*token == ' ') token++;
            if (*token != '\0') {
                if(dtype == TYPE_INT) { int v = atoi(token); insert_node_tree(&app->current_tree, &v); }
                else if(dtype == TYPE_FLOAT) { float v = atof(token); insert_node_tree(&app->current_tree, &v); }
                else if(dtype == TYPE_CHAR) { char v = token[0]; insert_node_tree(&app->current_tree, &v); }
                else if(dtype == TYPE_STRING) {
                    char buf[MAX_STRING_LEN];
                    strncpy(buf, token, MAX_STRING_LEN - 1);
                    buf[MAX_STRING_LEN - 1] = '\0';
                    insert_node_tree(&app->current_tree, buf);
                }
            }
            token = strtok(NULL, " ,;");
        }
        free(input_copy);
        tree_log(app, "Arbre généré (Manuel).");
    }
    gtk_widget_destroy(dialog);
    update_tree_display(app);
}

static void on_btn_tree_insert_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);
    const char *txt = gtk_entry_get_text(app->entry_tree_val);

    // Check empty input
    if (strlen(txt) == 0) return;

    if (app->current_tree.root == NULL) {
         tree_log(app, "Attention: L'arbre est vide. Insertion comme racine.");
    }

    int success = 0;
    if (app->current_tree.data_type == TYPE_INT) { int v = atoi(txt); success = insert_node_tree(&app->current_tree, &v); }
    else if (app->current_tree.data_type == TYPE_FLOAT) { float v = atof(txt); success = insert_node_tree(&app->current_tree, &v); }
    else if (app->current_tree.data_type == TYPE_CHAR) { char v = txt[0]; success = insert_node_tree(&app->current_tree, &v); }
    else if (app->current_tree.data_type == TYPE_STRING) { success = insert_node_tree(&app->current_tree, txt); }

    if (success) {
        update_tree_display(app);
        tree_log(app, "Noeud inséré.");
    } else {
        // Show Duplicate Error
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app->window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_WARNING,
                                         GTK_BUTTONS_OK,
                                         "La valeur '%s' existe déjÃ  dans l'arbre.", txt);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        tree_log(app, "Echec insertion : doublon.");
    }
}

static void on_btn_tree_delete_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);
    const char *txt = gtk_entry_get_text(app->entry_tree_val);

    if (app->current_tree.type == ARBRE_BINAIRE) {
         if (app->current_tree.data_type == TYPE_INT) { int v = atoi(txt); delete_node_tree(&app->current_tree, &v); }
         else if (app->current_tree.data_type == TYPE_FLOAT) { float v = atof(txt); delete_node_tree(&app->current_tree, &v); }
         else if (app->current_tree.data_type == TYPE_CHAR) { char v = txt[0]; delete_node_tree(&app->current_tree, &v); }
         else if (app->current_tree.data_type == TYPE_STRING) { delete_node_tree(&app->current_tree, txt); }
         update_tree_display(app);
    } else {
        tree_log(app, "Suppression supportée uniquement pour Arbre Binaire (BST).");
    }
}

// ANIMATION LOGIC

static gboolean tree_anim_tick(gpointer data) {
    AppData *app = (AppData *)data;

    if (!app->anim_queue) {
        // Fin animation
        app->current_tree.highlight_node = NULL;
        update_tree_display(app);
        app->anim_timer_id = 0;
        tree_log(app, "Parcours terminé.");
        return FALSE; // Clean up timer
    }

    // Pop first
    GList *head = app->anim_queue;
    NodeArbre *node = (NodeArbre *)head->data;
    app->anim_queue = g_list_delete_link(app->anim_queue, head);

    // Set highlight
    app->current_tree.highlight_node = node;
    update_tree_display(app);

    // Log value
    char buf[64];

    if(app->current_tree.data_type == TYPE_INT) snprintf(buf, 64, "%d", *(int*)node->data);
    else if(app->current_tree.data_type == TYPE_FLOAT) snprintf(buf, 64, "%.1f", *(float*)node->data);
    else if(app->current_tree.data_type == TYPE_CHAR) snprintf(buf, 64, "%c", *(char*)node->data);
    else if(app->current_tree.data_type == TYPE_STRING) snprintf(buf, 64, "%s", (char*)node->data);

    tree_log(app, buf);

    return TRUE;
}

static void stop_tree_animation(AppData *app) {
    if (app->anim_timer_id > 0) {
        g_source_remove(app->anim_timer_id);
        app->anim_timer_id = 0;
    }
    if (app->anim_queue) {
        g_list_free(app->anim_queue);
        app->anim_queue = NULL;
    }
    app->current_tree.highlight_node = NULL;
}

static void run_tree_animation(AppData *app) {
    // Stop any existing
    if (app->anim_timer_id > 0) g_source_remove(app->anim_timer_id);

    // Reverse queue because we used prepend (O(1)) for construction
    app->anim_queue = g_list_reverse(app->anim_queue);

    tree_log(app, "Démarrage du parcours animé...");
    app->anim_timer_id = g_timeout_add(500, tree_anim_tick, app); // 500ms slower animation for visibility
}

// HANDLERS

static void on_btn_tree_pre_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app); // Clear previous

    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_tree_log);
    gtk_text_buffer_set_text(buf, "Pré-fixe : \n", -1);

    parcours_prefixe(app->current_tree.root, app->current_tree.data_type, NULL, &app->anim_queue);
    run_tree_animation(app);
}

static void on_btn_tree_in_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);

    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_tree_log);
    gtk_text_buffer_set_text(buf, "In-fixe : \n", -1);

    parcours_infixe(app->current_tree.root, app->current_tree.data_type, NULL, &app->anim_queue);
    run_tree_animation(app);
}

static void on_btn_tree_post_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);

    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_tree_log);
    gtk_text_buffer_set_text(buf, "Post-fixe : \n", -1);

    parcours_postfixe(app->current_tree.root, app->current_tree.data_type, NULL, &app->anim_queue);
    run_tree_animation(app);
}

static void on_btn_tree_width_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    stop_tree_animation(app);

    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_tree_log);
    gtk_text_buffer_set_text(buf, "Largeur : \n", -1);

    parcours_largeur(&app->current_tree, NULL, &app->anim_queue);
    run_tree_animation(app);
}

static void on_btn_tree_sort_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    sort_tree_nodes(&app->current_tree);
    update_tree_display(app);
    tree_log(app, "Arbre trié (Enfants ordonnés).");
}

static void on_btn_tree_size_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int s = tree_size(app->current_tree.root);
    char msg[64];
    snprintf(msg, 64, "Taille de l'arbre : %d noeuds", s);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app->window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "%s", msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    tree_log(app, msg);
}

static void on_btn_tree_height_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int h = tree_height(app->current_tree.root, app->current_tree.type);
    char msg[64];
    snprintf(msg, 64, "Hauteur de l'arbre : %d", h);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(app->window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "%s", msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    tree_log(app, msg);
}

static void on_btn_tree_transform_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if(app->current_tree.type == ARBRE_NAIRE) {
        transform_to_binary(&app->current_tree);
        tree_log(app, "Transformé en Binaire (Isomorphisme LCRS).");
        update_tree_display(app);
    } else {
        tree_log(app, "L'arbre est déjà  de type Binaire.");
    }
}

static void on_btn_tree_reset_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    destroy_tree(&app->current_tree);

    // Stop Animation logic
    stop_tree_animation(app);

    // Reinit empty
    int type_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_type));
    TreeType ttype = (type_idx == 0) ? ARBRE_BINAIRE : ARBRE_NAIRE;
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_tree_dtype));
    init_tree(&app->current_tree, ttype, dtype);

    update_tree_display(app);
    tree_log(app, "Arbre réinitialisé.");
}

void create_page_arbres(AppData *app) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 10);

    GtkWidget *title = gtk_label_new("Gestion des Arbres");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 5);

    // Config
    GtkWidget *frame_conf = gtk_frame_new("Configuration");
    GtkWidget *box_conf = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(frame_conf), box_conf);
    gtk_container_set_border_width(GTK_CONTAINER(box_conf), 5);

    app->combo_tree_type = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_tree_type, "Arbre Binaire");
    gtk_combo_box_text_append_text(app->combo_tree_type, "Arbre N-Aire");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_tree_type), 0);

    app->combo_tree_dtype = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_tree_dtype, "Entier");
    gtk_combo_box_text_append_text(app->combo_tree_dtype, "Réel");
    gtk_combo_box_text_append_text(app->combo_tree_dtype, "Caractère");
    gtk_combo_box_text_append_text(app->combo_tree_dtype, "Chaîne");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_tree_dtype), 0);

    app->spin_tree_size = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(1, 100, 1));
    gtk_spin_button_set_value(app->spin_tree_size, 10);

    GtkWidget *btn_gen_rand = gtk_button_new_with_label("Générer Aléatoire");
    GtkWidget *btn_gen_man = gtk_button_new_with_label("Saisie Manuelle");

    g_signal_connect(btn_gen_rand, "clicked", G_CALLBACK(on_btn_tree_gen_clicked), app);
    g_signal_connect(btn_gen_man, "clicked", G_CALLBACK(on_btn_tree_manual_input_clicked), app);

    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Type:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->combo_tree_type), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Donnée:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->combo_tree_dtype), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Taille:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->spin_tree_size), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), btn_gen_rand, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), btn_gen_man, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_conf, FALSE, FALSE, 5);

    // Actions
    GtkWidget *frame_act = gtk_frame_new("Opérations / Parcours");
    GtkWidget *box_act = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(frame_act), box_act);
    gtk_container_set_border_width(GTK_CONTAINER(box_act), 5);

    GtkWidget *row_edit = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    app->entry_tree_val = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(app->entry_tree_val, "Valeur du nœud");
    GtkWidget *btn_add = gtk_button_new_with_label("Insérer");
    GtkWidget *btn_del = gtk_button_new_with_label("Supprimer");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_add), "btn-green");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_del), "btn-red");

    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_btn_tree_insert_clicked), app);
    g_signal_connect(btn_del, "clicked", G_CALLBACK(on_btn_tree_delete_clicked), app);

    gtk_box_pack_start(GTK_BOX(row_edit), GTK_WIDGET(app->entry_tree_val), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_edit), btn_add, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_edit), btn_del, FALSE, FALSE, 0);

    // Traversal Buttons
    GtkWidget *row_trav = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *btn_pre = gtk_button_new_with_label("Pré-ordre");
    GtkWidget *btn_in = gtk_button_new_with_label("In-ordre");
    GtkWidget *btn_post = gtk_button_new_with_label("Post-ordre");
    GtkWidget *btn_width = gtk_button_new_with_label("Largeur");

    // New Buttons
    GtkWidget *btn_trier = gtk_button_new_with_label("Trier");
    GtkWidget *btn_taille = gtk_button_new_with_label("Taille");
    GtkWidget *btn_hauteur = gtk_button_new_with_label("Hauteur");

    GtkWidget *btn_trans = gtk_button_new_with_label("Transf. Binaire");
    GtkWidget *btn_reset = gtk_button_new_with_label("Réinitialiser");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_pre), "btn-blue");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_in), "btn-blue");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_post), "btn-blue");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_width), "btn-orange");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_trier), "btn-green");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_taille), "btn-blue");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_hauteur), "btn-blue");

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_trans), "btn-orange");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_reset), "btn-red");

    g_signal_connect(btn_pre, "clicked", G_CALLBACK(on_btn_tree_pre_clicked), app);
    g_signal_connect(btn_in, "clicked", G_CALLBACK(on_btn_tree_in_clicked), app);
    g_signal_connect(btn_post, "clicked", G_CALLBACK(on_btn_tree_post_clicked), app);
    g_signal_connect(btn_width, "clicked", G_CALLBACK(on_btn_tree_width_clicked), app);

    g_signal_connect(btn_trier, "clicked", G_CALLBACK(on_btn_tree_sort_clicked), app);
    g_signal_connect(btn_taille, "clicked", G_CALLBACK(on_btn_tree_size_clicked), app);
    g_signal_connect(btn_hauteur, "clicked", G_CALLBACK(on_btn_tree_height_clicked), app);

    g_signal_connect(btn_trans, "clicked", G_CALLBACK(on_btn_tree_transform_clicked), app);
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_btn_tree_reset_clicked), app);

    gtk_box_pack_start(GTK_BOX(row_trav), btn_pre, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_in, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_post, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_width, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 5);

    // Pack New Buttons
    gtk_box_pack_start(GTK_BOX(row_trav), btn_trier, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_taille, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_hauteur, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(row_trav), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 5);

    gtk_box_pack_start(GTK_BOX(row_trav), btn_trans, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row_trav), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(row_trav), btn_reset, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box_act), row_edit, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_act), row_trav, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_act, FALSE, FALSE, 5);

    // Viz
    GtkWidget *paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    GtkWidget *scrolled_draw = gtk_scrolled_window_new(NULL, NULL);
    app->drawing_area_tree = gtk_drawing_area_new();
    gtk_widget_set_size_request(app->drawing_area_tree, -1, 400);
    g_signal_connect(app->drawing_area_tree, "draw", G_CALLBACK(draw_tree), &app->current_tree);
    gtk_container_add(GTK_CONTAINER(scrolled_draw), app->drawing_area_tree);

    GtkWidget *scrolled_log = gtk_scrolled_window_new(NULL, NULL);
    app->text_view_tree_log = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_view_set_editable(app->text_view_tree_log, FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_log), GTK_WIDGET(app->text_view_tree_log));

    gtk_paned_pack1(GTK_PANED(paned), scrolled_draw, TRUE, FALSE);
    gtk_paned_pack2(GTK_PANED(paned), scrolled_log, FALSE, TRUE);
    gtk_paned_set_position(GTK_PANED(paned), 400);

    gtk_box_pack_start(GTK_BOX(page), paned, TRUE, TRUE, 0);

    gtk_stack_add_named(GTK_STACK(app->stack), page, "arbres");
}

// ===========================================
// PAGE GRAPHES
// ===========================================

// --- Graph Helpers & Callbacks ---

// Forward Declaration
static void graph_log(AppData *app, const char *msg);

static void save_state(AppData *app) {
    if (app->graph_history_top < GRAPH_HISTORY_SIZE) {
        copy_graph(&app->graph_history[app->graph_history_top], &app->current_graph);
        app->graph_history_top++;
    } else {
        // Shift logic:
        for(int i=0; i<GRAPH_HISTORY_SIZE - 1; i++) {
            copy_graph(&app->graph_history[i], &app->graph_history[i+1]);
        }
        copy_graph(&app->graph_history[GRAPH_HISTORY_SIZE - 1], &app->current_graph);
    }
    if (app->btn_graph_undo) gtk_widget_set_sensitive(app->btn_graph_undo, TRUE);
}

static void undo_last_action(AppData *app) {
    if (app->graph_history_top > 0) {
        app->graph_history_top--;
        copy_graph(&app->current_graph, &app->graph_history[app->graph_history_top]);
        app->selected_node_id = -1;
        gtk_widget_queue_draw(app->drawing_area_graph);
        graph_log(app, "Action annulée.");
    } else {
        graph_log(app, "Aucune action à annuler.");
    }
    if (app->graph_history_top == 0 && app->btn_graph_undo) {
        gtk_widget_set_sensitive(app->btn_graph_undo, FALSE);
    }
}

static void on_btn_graph_undo_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    undo_last_action(app);
}

static void graph_log(AppData *app, const char *msg) {
    if (!app->text_view_graph_log) return;
    GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_graph_log);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buf, &end);
    gtk_text_buffer_insert(buf, &end, msg, -1);
    gtk_text_buffer_insert(buf, &end, "\n", -1);
}

static void helper_remove_edge(Graphe *g, int src, int dest) {
    if (src < 0 || src >= g->node_count) return;

    EdgeGraphe *prev = NULL;
    EdgeGraphe *curr = g->nodes[src]->head;
    while(curr) {
        if (curr->dest_index == dest) {
            if (prev) prev->next = curr->next;
            else g->nodes[src]->head = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static void helper_remove_node(Graphe *g, int id) {
    if (id < 0 || id >= g->node_count) return;

    // Remove edges To this node
    for(int i=0; i<g->node_count; i++) {
        helper_remove_edge(g, i, id);
    }

    // Remove edges From this node
    if (g->nodes[id]) {
        EdgeGraphe *e = g->nodes[id]->head;
        while(e) {
            EdgeGraphe *tmp = e;
            e = e->next;
            free(tmp);
        }
        if (g->nodes[id]->data) free(g->nodes[id]->data);
        free(g->nodes[id]);
    }

    // Shift
    for (int i=id; i < g->node_count - 1; i++) {
        g->nodes[i] = g->nodes[i+1];
        if (g->nodes[i]) g->nodes[i]->id = i;
    }
    g->nodes[g->node_count - 1] = NULL;
    g->node_count--;

    // Fix indices in edges
    for(int i=0; i<g->node_count; i++) {
        EdgeGraphe *e = g->nodes[i]->head;
        while(e) {
            if (e->dest_index > id) e->dest_index--;
            e = e->next;
        }
    }
}

static void on_btn_graph_new_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    save_state(app);
    clear_graph(&app->current_graph);

    int directed = (gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_type)) == 0);
    int w_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_nature));
    int type_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_dtype));

    init_graph(&app->current_graph, (DataType)type_idx, directed);
    app->current_graph.is_weighted = (w_idx == 0); // 0=Pondéré

    // Toggle Weight Input
    gtk_widget_set_sensitive(GTK_WIDGET(app->entry_graph_edge_weight), app->current_graph.is_weighted);

    app->selected_node_id = -1;
    gtk_widget_queue_draw(app->drawing_area_graph);

    if(app->text_view_graph_log) {
        GtkTextBuffer *buf = gtk_text_view_get_buffer(app->text_view_graph_log);
        gtk_text_buffer_set_text(buf, "Nouveau graphe créé.\n", -1);
    }
}

static void on_btn_graph_reset_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    save_state(app);
    clear_graph(&app->current_graph);
    app->selected_node_id = -1;
    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Graphe réinitialisé.");
}

static void on_btn_graph_add_node_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    const char *val = gtk_entry_get_text(app->entry_graph_node_val);
    if(strlen(val) == 0) return;

    save_state(app);

    double x = 50 + rand() % 400;
    double y = 50 + rand() % 300;

    if (app->current_graph.data_type == TYPE_INT) { int v=atoi(val); add_node_graph(&app->current_graph, x, y, &v); }
    else if (app->current_graph.data_type == TYPE_FLOAT) { float v=atof(val); add_node_graph(&app->current_graph, x, y, &v); }
    else if (app->current_graph.data_type == TYPE_CHAR) { char v=val[0]; add_node_graph(&app->current_graph, x, y, &v); }
    else if (app->current_graph.data_type == TYPE_STRING) { add_node_graph(&app->current_graph, x, y, val); }

    gtk_widget_queue_draw(app->drawing_area_graph);
    char buf[64]; snprintf(buf, 64, "Noeud ajouté: %s", val);
    graph_log(app, buf);
}

static void on_btn_graph_mod_node_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if (app->selected_node_id == -1) {
        graph_log(app, "Erreur: Veuillez sélectionner un noeud sur le dessin.");
        return;
    }
    save_state(app);
    const char *val = gtk_entry_get_text(app->entry_graph_node_val);
    NodeGraphe *n = app->current_graph.nodes[app->selected_node_id];
    if(n) {
        if (app->current_graph.data_type == TYPE_INT) *(int*)n->data = atoi(val);
        else if (app->current_graph.data_type == TYPE_FLOAT) *(float*)n->data = atof(val);
        else if (app->current_graph.data_type == TYPE_CHAR) *(char*)n->data = val[0];
        else if (app->current_graph.data_type == TYPE_STRING) strncpy((char*)n->data, val, MAX_STRING_LEN);
    }
    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Noeud modifié.");
}

static void on_btn_graph_del_node_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if (app->selected_node_id == -1) {
         graph_log(app, "Erreur: Veuillez sélectionner un noeud.");
         return;
    }
    save_state(app);
    helper_remove_node(&app->current_graph, app->selected_node_id);
    app->selected_node_id = -1;
    app->current_graph.selected_id = -1;
    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Noeud supprimé.");
}

static void on_btn_graph_add_edge_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    const char *s_src = gtk_entry_get_text(app->entry_graph_edge_src);
    const char *s_dest = gtk_entry_get_text(app->entry_graph_edge_dest);
    const char *s_w = gtk_entry_get_text(app->entry_graph_edge_weight);

    if(strlen(s_src)==0 || strlen(s_dest)==0) return;

    save_state(app);
    int src = atoi(s_src);
    int dest = atoi(s_dest);
    float w = 1.0;
    if (app->current_graph.is_weighted && strlen(s_w) > 0) w = atof(s_w);

    add_edge_graph(&app->current_graph, src, dest, w);
    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Arête ajoutée.");
}

static void on_btn_graph_del_edge_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    const char *s_src = gtk_entry_get_text(app->entry_graph_edge_src);
    const char *s_dest = gtk_entry_get_text(app->entry_graph_edge_dest);
    if(strlen(s_src)==0 || strlen(s_dest)==0) return;

    save_state(app);
    int src = atoi(s_src);
    int dest = atoi(s_dest);

    helper_remove_edge(&app->current_graph, src, dest);
    if(!app->current_graph.is_directed) helper_remove_edge(&app->current_graph, dest, src);

    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Arête supprimée.");
}

static void on_btn_graph_draw_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Actualisé.");
}

static int resolve_id_from_string(Graphe *g, const char *input) {
    if (!g || !input) return -1;
    char *endptr;
    long val = strtol(input, &endptr, 10);
    if (*endptr == '\0') {
        if (val >= 0 && val < g->node_count) return (int)val;
    }

    for(int i=0; i<g->node_count; i++) {
        NodeGraphe *n = g->nodes[i];
        if (!n || !n->data) continue;

        if (g->data_type == TYPE_INT) {
            char buf[32]; sprintf(buf, "%d", *(int*)n->data);
            if (strcmp(buf, input) == 0) return n->id;
        } else if (g->data_type == TYPE_FLOAT) {
            char buf[32]; sprintf(buf, "%.1f", *(float*)n->data);
            if (strcmp(buf, input) == 0) return n->id;
        } else if (g->data_type == TYPE_CHAR) {
            char c = *(char*)n->data;
            if (input[0] == c && input[1] == '\0') return n->id;
        } else if (g->data_type == TYPE_STRING) {
            if (strcmp((char*)n->data, input) == 0) return n->id;
        }
    }

    return (int)val;
}

static void on_btn_graph_calc_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int algo = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_algo));
    const char *s_s = gtk_entry_get_text(app->entry_graph_algo_start);
    const char *s_e = gtk_entry_get_text(app->entry_graph_algo_end);

    if(strlen(s_s)==0 || strlen(s_e)==0) return;

    int start = resolve_id_from_string(&app->current_graph, s_s);
    int end = resolve_id_from_string(&app->current_graph, s_e);

    if (start < 0 || start >= app->current_graph.node_count ||
        end < 0 || end >= app->current_graph.node_count) {
        graph_log(app, "Erreur: Noeuds départ/arrivée introuvables.");
        return;
    }

    int res = 0;
    if (algo == 0) res = algo_dijkstra(&app->current_graph, start, end);
    else if (algo == 1) res = algo_bellman_ford(&app->current_graph, start, end);
    else if (algo == 2) res = algo_floyd_warshall(&app->current_graph, start, end);

    char path_buf[4096];
    find_all_paths_print(&app->current_graph, start, end, path_buf, 4096);
    graph_log(app, path_buf);

    if(res) {
        char buf[128];
        snprintf(buf, 128, "Chemin minimum trouvé! Longueur: %d noeuds.", app->current_graph.path_len);
        graph_log(app, buf);

        // Compute Cost
        float cost = 0;
        for(int k=0; k<app->current_graph.path_len-1; k++) {
            int u = app->current_graph.path_indices[k];
            int v = app->current_graph.path_indices[k+1];
            EdgeGraphe *e = app->current_graph.nodes[u]->head;
            while(e) {
                if(e->dest_index == v) { cost += e->weight; break; }
                e = e->next;
            }
        }
        snprintf(buf, 128, "Cout total du chemin: %.2f", cost);
        graph_log(app, buf);

    } else {
        graph_log(app, "Chemin impossible ou erreur.");
        app->current_graph.path_len = 0;
    }
    gtk_widget_queue_draw(app->drawing_area_graph);
}

static void on_graph_canvas_interact(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    AppData *app = (AppData *)data;

    // --- Double Click: Ajouter Noeud ---
    if (event->type == GDK_2BUTTON_PRESS) {
        NodeGraphe *clicked = get_node_at(&app->current_graph, event->x, event->y);
        if (!clicked) {
            save_state(app);
            // Smart Data Generation
            if (app->current_graph.data_type == TYPE_INT) {
                int v = app->current_graph.node_count;
                add_node_graph(&app->current_graph, event->x, event->y, &v);
            } else if (app->current_graph.data_type == TYPE_CHAR) {
                char v = 'A' + (app->current_graph.node_count % 26);
                add_node_graph(&app->current_graph, event->x, event->y, &v);
            } else if (app->current_graph.data_type == TYPE_FLOAT) {
                float v = (float)app->current_graph.node_count;
                add_node_graph(&app->current_graph, event->x, event->y, &v);
            } else if (app->current_graph.data_type == TYPE_STRING) {
                char v[32]; snprintf(v, 32, "N%d", app->current_graph.node_count);
                add_node_graph(&app->current_graph, event->x, event->y, v);
            }
            gtk_widget_queue_draw(app->drawing_area_graph);
            graph_log(app, "Noeud ajouté.");
        }
        return;
    }

    // --- Simple Click: Sélection / Liaison ---
    if (event->type == GDK_BUTTON_PRESS) {
        NodeGraphe *clicked = get_node_at(&app->current_graph, event->x, event->y);

        if (clicked) {
            if (app->selected_node_id == -1) {
                // Select First
                app->selected_node_id = clicked->id;
                app->current_graph.selected_id = clicked->id;
                gtk_widget_queue_draw(app->drawing_area_graph);
            } else {
                if (app->selected_node_id != clicked->id) {

                    float w = 1.0;
                    if (app->current_graph.is_weighted) {
                         GtkWidget *dialog = gtk_dialog_new_with_buttons("Poids", GTK_WINDOW(app->window),
                             GTK_DIALOG_MODAL, "OK", GTK_RESPONSE_ACCEPT, NULL);
                         GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
                         GtkWidget *e = gtk_entry_new();
                         gtk_entry_set_text(GTK_ENTRY(e), "1");
                         gtk_container_add(GTK_CONTAINER(content), e);
                         gtk_widget_show_all(dialog);
                         if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                             w = atof(gtk_entry_get_text(GTK_ENTRY(e)));
                         }
                         gtk_widget_destroy(dialog);
                    }

                    save_state(app); // Inserted here
                    if (!app->current_graph.is_directed) {
                         // Add reverse edge for undirected visual
                         // already handled by add_edge? NO, add_edge doesn't auto add for us if we don't call it.
                         // Wait, in graphe.c add_edge_graph automatically adds reverse if !is_directed.
                         // So we don't need to call it twice.
                         // But let's check graphe.c ...
                         // Yes: "if (!g->is_directed) { ... }" in add_edge_graph
                    }

                    add_edge_graph(&app->current_graph, app->selected_node_id, clicked->id, w);

                    graph_log(app, "Arête ajoutée.");

                    app->selected_node_id = -1;
                    app->current_graph.selected_id = -1;
                    gtk_widget_queue_draw(app->drawing_area_graph);
                } else {
                    // Deselect if clicking self
                    app->selected_node_id = -1;
                    app->current_graph.selected_id = -1;
                    gtk_widget_queue_draw(app->drawing_area_graph);
                }
            }
        } else {
            // Clicked empty space -> Deselect
            app->selected_node_id = -1;
            app->current_graph.selected_id = -1;
            gtk_widget_queue_draw(app->drawing_area_graph);
        }
    }
}

static void on_draw_graph_wrapper(GtkWidget *widget, cairo_t *cr, gpointer data) {
    draw_graph(widget, cr, data);
}

static void on_btn_graph_random_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;

    int count = gtk_spin_button_get_value_as_int(app->spin_graph_count);
    int directed = (gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_type)) == 0);
    int w_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_nature));
    int type_idx = gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_graph_dtype));

    save_state(app);

    // Reset/Init
    clear_graph(&app->current_graph);
    init_graph(&app->current_graph, (DataType)type_idx, directed);
    app->current_graph.is_weighted = (w_idx == 0);

    generate_random_graph(&app->current_graph, count, directed, app->current_graph.is_weighted);

    gtk_widget_set_sensitive(GTK_WIDGET(app->entry_graph_edge_weight), app->current_graph.is_weighted);
    app->selected_node_id = -1;

    gtk_widget_queue_draw(app->drawing_area_graph);
    graph_log(app, "Graphe aléatoire généré.");
}

void create_page_graphes(AppData *app) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 10);

    // Title
    GtkWidget *title = gtk_label_new("Gestion des Graphes");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 5);

    // --- 1. CONFIGURATION ---
    GtkWidget *frame_conf = gtk_frame_new("Configuration du Graphe");
    GtkWidget *box_conf = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_conf), 5);
    gtk_container_add(GTK_CONTAINER(frame_conf), box_conf);

    app->combo_graph_type = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_graph_type, "Orienté");
    gtk_combo_box_text_append_text(app->combo_graph_type, "Non Orienté");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_graph_type), 0);

    app->combo_graph_nature = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_graph_nature, "Pondéré");
    gtk_combo_box_text_append_text(app->combo_graph_nature, "Non Pondéré");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_graph_nature), 0);

    app->combo_graph_dtype = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_graph_dtype, "Entier");
    gtk_combo_box_text_append_text(app->combo_graph_dtype, "Réel");
    gtk_combo_box_text_append_text(app->combo_graph_dtype, "Caractère");
    gtk_combo_box_text_append_text(app->combo_graph_dtype, "Chaîne");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_graph_dtype), 0);

    GtkWidget *btn_new = gtk_button_new_with_label("Nouveau Graphe");
    GtkWidget *btn_rst = gtk_button_new_with_label("Réinitialiser");

    // Random Gen Controls
    app->spin_graph_count = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(2, MAX_NODES, 1));
    gtk_spin_button_set_value(app->spin_graph_count, 5);
    GtkWidget *btn_rand = gtk_button_new_with_label("Générer Aléatoire");

    // UNDO BUTTON
    app->btn_graph_undo = gtk_button_new_with_label("Retour ");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_graph_undo), "btn-orange"); // Orange for distinctive look
    g_signal_connect(app->btn_graph_undo, "clicked", G_CALLBACK(on_btn_graph_undo_clicked), app);
    gtk_widget_set_sensitive(app->btn_graph_undo, FALSE);

    gtk_style_context_add_class(gtk_widget_get_style_context(btn_new), "btn-green");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_rst), "btn-red");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_rand), "btn-blue");

    g_signal_connect(btn_new, "clicked", G_CALLBACK(on_btn_graph_new_clicked), app);
    g_signal_connect(btn_rst, "clicked", G_CALLBACK(on_btn_graph_reset_clicked), app);
    g_signal_connect(btn_rand, "clicked", G_CALLBACK(on_btn_graph_random_clicked), app);

    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Type:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->combo_graph_type), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Nature:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->combo_graph_nature), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Donnée:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->combo_graph_dtype), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), btn_new, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), btn_rst, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), app->btn_graph_undo, FALSE, FALSE, 0); // Pack Undo

    gtk_box_pack_start(GTK_BOX(box_conf), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(box_conf), gtk_label_new("Nb Noeuds:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), GTK_WIDGET(app->spin_graph_count), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_conf), btn_rand, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_conf, FALSE, FALSE, 5);

    // --- CANVAS (INTERACTION DIRECTE) ---
    GtkWidget *fr_draw = gtk_frame_new("Zone de Dessin (Double-cliquer: Ajouter Noeud | Cliquer-Cliquer: Ajouter Arête)");
    GtkWidget *box_draw = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(fr_draw), box_draw);

    app->drawing_area_graph = gtk_drawing_area_new();
    gtk_widget_set_size_request(app->drawing_area_graph, -1, 400);
    gtk_widget_add_events(app->drawing_area_graph, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(app->drawing_area_graph, "draw", G_CALLBACK(on_draw_graph_wrapper), &app->current_graph);
    g_signal_connect(app->drawing_area_graph, "button-press-event", G_CALLBACK(on_graph_canvas_interact), app);

    GtkWidget *btn_draw = gtk_button_new_with_label("Dessiner / Actualiser");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_draw), "btn-blue");
    g_signal_connect(btn_draw, "clicked", G_CALLBACK(on_btn_graph_draw_clicked), app);

    gtk_box_pack_start(GTK_BOX(box_draw), app->drawing_area_graph, TRUE, TRUE, 0);

    // --- ALGOS & LOG ---
    GtkWidget *box_lower = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *frame_algo = gtk_frame_new("Algorithmes de Chemin Minimum");
    GtkWidget *box_algo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_algo), 5);
    gtk_container_add(GTK_CONTAINER(frame_algo), box_algo);

    app->combo_graph_algo = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_graph_algo, "Dijkstra");
    gtk_combo_box_text_append_text(app->combo_graph_algo, "Bellman-Ford");
    gtk_combo_box_text_append_text(app->combo_graph_algo, "Floyd-Warshall");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_graph_algo), 0);

    app->entry_graph_algo_start = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(app->entry_graph_algo_start, "Départ ");
    gtk_widget_set_size_request(GTK_WIDGET(app->entry_graph_algo_start), 60, -1);

    app->entry_graph_algo_end = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(app->entry_graph_algo_end, "Arrivée ");
    gtk_widget_set_size_request(GTK_WIDGET(app->entry_graph_algo_end), 60, -1);

    GtkWidget *btn_calc = gtk_button_new_with_label("Calculer");
    gtk_style_context_add_class(gtk_widget_get_style_context(btn_calc), "btn-orange");
    g_signal_connect(btn_calc, "clicked", G_CALLBACK(on_btn_graph_calc_clicked), app);

    gtk_box_pack_start(GTK_BOX(box_algo), gtk_label_new("Algorithme:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), GTK_WIDGET(app->combo_graph_algo), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), gtk_label_new("De :"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), GTK_WIDGET(app->entry_graph_algo_start), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), gtk_label_new("Vers :"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), GTK_WIDGET(app->entry_graph_algo_end), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_algo), btn_calc, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box_lower), frame_algo, FALSE, FALSE, 0);

    // Results
    GtkWidget *frame_res = gtk_frame_new("Résultats & Messages");
    GtkWidget *scrolled_log = gtk_scrolled_window_new(NULL, NULL);
    app->text_view_graph_log = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_view_set_editable(app->text_view_graph_log, FALSE);
    gtk_container_add(GTK_CONTAINER(scrolled_log), GTK_WIDGET(app->text_view_graph_log));
    gtk_container_add(GTK_CONTAINER(frame_res), scrolled_log);
    gtk_widget_set_size_request(scrolled_log, -1, 100);

    gtk_box_pack_start(GTK_BOX(box_lower), frame_res, TRUE, TRUE, 0);

    // --- LAYOUT PRINCIPAL (PANED) ---
    GtkWidget *paned_main = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_paned_pack1(GTK_PANED(paned_main), fr_draw, TRUE, FALSE);
    gtk_paned_pack2(GTK_PANED(paned_main), box_lower, FALSE, TRUE);
    gtk_paned_set_position(GTK_PANED(paned_main), 450);

    gtk_box_pack_start(GTK_BOX(page), paned_main, TRUE, TRUE, 0);

    gtk_stack_add_named(GTK_STACK(app->stack), page, "graphes");

    init_graph(&app->current_graph, TYPE_INT, 1);
}


// ===========================================
// PAGE PILES & FILES
// ===========================================

// --- Drawing Callbacks ---

static void draw_stack_plate(cairo_t *cr, double x, double y, double w, double h, const char *text, int is_top) {
    // Plate style
    cairo_set_source_rgb(cr, 0.2, 0.6, 1.0); // Blueish
    if (is_top) cairo_set_source_rgb(cr, 1.0, 0.6, 0.2); // Orange for top

    // Draw rounded rect (simplified as rect)
    cairo_rectangle(cr, x, y, w, h);
    cairo_fill_preserve(cr);
    cairo_set_source_rgb(cr, 0.1, 0.3, 0.5); // Border
    cairo_stroke(cr);

    // Text
    cairo_set_source_rgb(cr, 1, 1, 1); // White text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, text, &extents);
    cairo_move_to(cr, x + (w - extents.width)/2, y + (h + extents.height)/2);
    cairo_show_text(cr, text);
}

static void draw_queue_item(cairo_t *cr, double x, double y, double w, double h, const char *text, int is_head, int is_tail) {

    // Fill
    cairo_set_source_rgb(cr, 0.95, 0.95, 0.95); // Off-white
    if (is_head) cairo_set_source_rgb(cr, 1.0, 0.8, 0.8); // Red tint for head
    if (is_tail) cairo_set_source_rgb(cr, 0.8, 0.8, 1.0); // Blue tint for tail

    cairo_rectangle(cr, x, y, w, h);
    cairo_fill_preserve(cr);

    // Border
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 2);
    cairo_stroke(cr);


    // Text
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);

    cairo_text_extents_t extents;
    cairo_text_extents(cr, text, &extents);
    cairo_move_to(cr, x + (w - extents.width)/2, y + (h + extents.height)/2);
    cairo_show_text(cr, text);

    // Labels underneath
    cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
    cairo_set_font_size(cr, 10);
    if (is_head) {
        cairo_move_to(cr, x, y - 5);
        cairo_show_text(cr, "Tete");
    }
    if (is_tail) {
        cairo_move_to(cr, x, y + h + 12);
        cairo_show_text(cr, "Fin");
    }
}

gboolean on_draw_pile_file(GtkWidget *widget, cairo_t *cr, gpointer data) {
    AppData *app = (AppData *)data;

    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);

    // BG
    cairo_set_source_rgb(cr, 1, 1, 1); // White
    cairo_paint(cr);

    char buf[256];

    if (app->pile_file_mode == 0) {
        // --- STACK (Vertical) ---
        // Base
        cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
        cairo_move_to(cr, width/2 - 100, height - 50);
        cairo_line_to(cr, width/2 + 100, height - 50);
        cairo_set_line_width(cr, 5);
        cairo_stroke(cr);

        // Items
        double item_w = 120;
        double item_h = 30;
        // double start_y = height - 55 - item_h; // Unused
        double x = width/2 - item_w/2;

        PFNode *curr = app->current_stack.top;

        int count = 0;
        PFNode *it = app->current_stack.top;
        while(it) { count++; it = it->next; }


        double current_y = (height - 50) - (count * item_h);
        curr = app->current_stack.top;
        int idx = 0;
        while(curr) {
             pf_get_value_string(curr->data, app->current_stack.data_type, buf, 256);
             draw_stack_plate(cr, x, current_y + idx*item_h, item_w, item_h, buf, (idx==0));
             curr = curr->next;
             idx++;
        }

    } else {
        // --- QUEUE (Horizontal) ---
        // Draw line/guides
        cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
        cairo_rectangle(cr, 50, height/2 - 40, width - 100, 80);
        cairo_fill(cr);

        double item_w = 60;
        double item_h = 40;
        double spacing = 0; // Touching
        double start_x = 60;
        double y = height/2 - item_h/2;

        // Draw decorative "Pipe" tunnel
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
        cairo_rectangle(cr, 40, y-10, width-80, item_h+20);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
        cairo_set_line_width(cr, 1);
        cairo_rectangle(cr, 40, y-10, width-80, item_h+20);
        cairo_stroke(cr);

        PFNode *curr = app->current_queue.front;
        while(curr) {
            pf_get_value_string(curr->data, app->current_queue.data_type, buf, 256);
            int is_head = (curr == app->current_queue.front);
            int is_tail = (curr == app->current_queue.rear);
            draw_queue_item(cr, start_x, y, item_w, item_h, buf, is_head, is_tail);

            start_x += item_w + spacing;
            // Prevent drawing OOB
            if(start_x > width - 80) break;

            curr = curr->next;
        }
    }

    return FALSE;
}

static void update_pf_display(AppData *app) {
    // Update Size Label/buffer
    char buf[64];
    int size = (app->pile_file_mode == 0) ? app->current_stack.size : app->current_queue.size;
    snprintf(buf, 64, "Taille actuelle : %d", size);
    gtk_label_set_text(app->lbl_pf_status, buf);

    gtk_widget_queue_draw(app->drawing_area_pf);
}

// --- Callbacks ---

static void on_pf_mode_changed(GtkComboBox *widget, gpointer data) {
    AppData *app = (AppData *)data;
    app->pile_file_mode = gtk_combo_box_get_active(widget);

    // Update Button Labels & Visibility
    if (app->pile_file_mode == 0) {
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action1), "Empiler");
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action2), "Dépiler");
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action3), "Sommet");
    } else {
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action1), "Enfiler ");
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action2), "Défiler ");
        gtk_button_set_label(GTK_BUTTON(app->btn_pf_action3), "Tête ");
    }

    update_pf_display(app);
}

static void on_btn_pf_generate_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    int size = gtk_spin_button_get_value_as_int(app->spin_pf_size);
    DataType dtype = (DataType)gtk_combo_box_get_active(GTK_COMBO_BOX(app->combo_pf_dtype));



    if (app->pile_file_mode == 0) {
        reset_stack(&app->current_stack);
        app->current_stack.data_type = dtype;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->radio_pf_random))) {
            stack_generate_random(&app->current_stack, size);
        } else {
             // Manual Dialog
            GtkWidget *dialog = gtk_dialog_new_with_buttons("Saisie Manuelle Pile", GTK_WINDOW(app->window),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);

            GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
            GtkWidget *entry = gtk_entry_new();
            gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Ex: 10, 5, 8...");
            gtk_container_add(GTK_CONTAINER(content), entry);
            gtk_widget_show_all(dialog);

            if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
                char *input_copy = strdup(text);
                char *token = strtok(input_copy, " ,;");

                while(token) {
                    while(*token == ' ') token++;
                    if (*token != '\0') {
                         union { int i; float f; char c; char s[MAX_STRING_LEN]; } val;
                         if(dtype == TYPE_INT) { val.i = atoi(token); stack_push(&app->current_stack, &val.i); }
                         else if(dtype == TYPE_FLOAT) { val.f = atof(token); stack_push(&app->current_stack, &val.f); }
                         else if(dtype == TYPE_CHAR) { val.c = token[0]; stack_push(&app->current_stack, &val.c); }
                         else if(dtype == TYPE_STRING) { strncpy(val.s, token, MAX_STRING_LEN); stack_push(&app->current_stack, val.s); }
                    }
                    token = strtok(NULL, " ,;");
                }
                free(input_copy);
            }
            gtk_widget_destroy(dialog);
        }
    } else {
        reset_queue(&app->current_queue);
        app->current_queue.data_type = dtype;
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->radio_pf_random))) {
            queue_generate_random(&app->current_queue, size);
        } else {
             // Manual Dialog
            GtkWidget *dialog = gtk_dialog_new_with_buttons("Saisie Manuelle File", GTK_WINDOW(app->window),
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                "Valider", GTK_RESPONSE_ACCEPT, "Annuler", GTK_RESPONSE_REJECT, NULL);

            GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
            GtkWidget *entry = gtk_entry_new();
            gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Ex: 10, 5, 8...");
            gtk_container_add(GTK_CONTAINER(content), entry);
            gtk_widget_show_all(dialog);

            if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
                const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
                char *input_copy = strdup(text);
                char *token = strtok(input_copy, " ,;");
                while(token) {
                    while(*token == ' ') token++;
                    if (*token != '\0') {
                         union { int i; float f; char c; char s[MAX_STRING_LEN]; } val;
                         if(dtype == TYPE_INT) { val.i = atoi(token); queue_enqueue(&app->current_queue, &val.i); }
                         else if(dtype == TYPE_FLOAT) { val.f = atof(token); queue_enqueue(&app->current_queue, &val.f); }
                         else if(dtype == TYPE_CHAR) { val.c = token[0]; queue_enqueue(&app->current_queue, &val.c); }
                         else if(dtype == TYPE_STRING) { strncpy(val.s, token, MAX_STRING_LEN); queue_enqueue(&app->current_queue, val.s); }
                    }
                    token = strtok(NULL, " ,;");
                }
                free(input_copy);
            }
            gtk_widget_destroy(dialog);
        }
    }

    gtk_label_set_text(app->lbl_pf_message, "Généré avec succès.");
    update_pf_display(app);
}

static void on_btn_pf_reset_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    if (app->pile_file_mode == 0) reset_stack(&app->current_stack);
    else reset_queue(&app->current_queue);

    gtk_label_set_text(app->lbl_pf_message, "Structure réinitialisée.");
    update_pf_display(app);
}

static void on_btn_pf_action1_clicked(GtkWidget *widget, gpointer data) { // Push / Enqueue
    AppData *app = (AppData *)data;
    const char *txt = gtk_entry_get_text(app->entry_pf_val);
    if(strlen(txt) == 0) {
         gtk_label_set_text(app->lbl_pf_message, "Erreur: Entrée vide.");
         return;
    }

    DataType dtype = (app->pile_file_mode == 0) ? app->current_stack.data_type : app->current_queue.data_type;

    // Parse
    union { int i; float f; char c; char s[MAX_STRING_LEN]; } val;

    if(dtype == TYPE_INT) val.i = atoi(txt);
    else if(dtype == TYPE_FLOAT) val.f = atof(txt);
    else if(dtype == TYPE_CHAR) val.c = txt[0];
    else if(dtype == TYPE_STRING) strncpy(val.s, txt, MAX_STRING_LEN);

    int res = 0;
    if (app->pile_file_mode == 0) {
        if(dtype == TYPE_STRING) res = stack_push(&app->current_stack, val.s);
        else res = stack_push(&app->current_stack, &val);
    } else {
         if(dtype == TYPE_STRING) res = queue_enqueue(&app->current_queue, val.s);
        else res = queue_enqueue(&app->current_queue, &val);
    }

    if(res) gtk_label_set_text(app->lbl_pf_message, "Succès.");
    else gtk_label_set_text(app->lbl_pf_message, "Erreur: Impossible (Plein?).");

    update_pf_display(app);
    gtk_entry_set_text(app->entry_pf_val, "");
}

static void on_btn_pf_action2_clicked(GtkWidget *widget, gpointer data) { // Pop / Dequeue
    AppData *app = (AppData *)data;
    int res = 0;
    if (app->pile_file_mode == 0) res = stack_pop(&app->current_stack, NULL);
    else res = queue_dequeue(&app->current_queue, NULL);

    if(res) gtk_label_set_text(app->lbl_pf_message, "Élément retiré.");
    else gtk_label_set_text(app->lbl_pf_message, "Erreur: Vide.");

    update_pf_display(app);
}

static void on_btn_pf_action3_clicked(GtkWidget *widget, gpointer data) {
    AppData *app = (AppData *)data;
    char buf[256];
    char val_str[128];
    DataType dtype;
    int res = 0;

    // Use a temp buffer to hold data
    union { int i; float f; char c; char s[MAX_STRING_LEN]; } val;

    if (app->pile_file_mode == 0) {
        dtype = app->current_stack.data_type;
        res = stack_peek(&app->current_stack, &val);
    } else {
        dtype = app->current_queue.data_type;
        res = queue_front(&app->current_queue, &val);
    }

    if(res) {
        if(dtype == TYPE_STRING) pf_get_value_string(val.s, dtype, val_str, 128);
        else pf_get_value_string(&val, dtype, val_str, 128);
        snprintf(buf, 256, "Élément : %s", val_str);
        gtk_label_set_text(app->lbl_pf_message, buf);
    } else {
        gtk_label_set_text(app->lbl_pf_message, "Erreur: Vide.");
    }
}



// --- Creation ---

void create_page_pile_file(AppData *app) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 10);

    // Title
    GtkWidget *title = gtk_label_new("Gestion des Piles & Files");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "section-title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 5);

    // --- Mode Selection & Config ---
    GtkWidget *frame_config = gtk_frame_new("Configuration");
    GtkWidget *box_config = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_config), 5);
    gtk_container_add(GTK_CONTAINER(frame_config), box_config);

    // Mode
    app->combo_pf_choice = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_pf_choice, "Pile - LIFO");
    gtk_combo_box_text_append_text(app->combo_pf_choice, "File - FIFO");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_pf_choice), 0);
    g_signal_connect(app->combo_pf_choice, "changed", G_CALLBACK(on_pf_mode_changed), app);

    // Data Type
    app->combo_pf_dtype = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
    gtk_combo_box_text_append_text(app->combo_pf_dtype, "Entier");
    gtk_combo_box_text_append_text(app->combo_pf_dtype, "Réel");
    gtk_combo_box_text_append_text(app->combo_pf_dtype, "Caractère");
    gtk_combo_box_text_append_text(app->combo_pf_dtype, "Chaîne");
    gtk_combo_box_set_active(GTK_COMBO_BOX(app->combo_pf_dtype), 0);

    // Size/Generation
    app->spin_pf_size = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(1, 20, 1));
    gtk_spin_button_set_value(app->spin_pf_size, 5);

    app->radio_pf_random = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label(NULL, "Aléatoire"));
    app->radio_pf_manual = GTK_RADIO_BUTTON(gtk_radio_button_new_with_label_from_widget(app->radio_pf_random, "Manuel"));

    GtkWidget *btn_gen = gtk_button_new_with_label("Générer");
    g_signal_connect(btn_gen, "clicked", G_CALLBACK(on_btn_pf_generate_clicked), app);

    GtkWidget *btn_reset = gtk_button_new_with_label("Réinitialiser");
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_btn_pf_reset_clicked), app);

    // Pack Config
    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Mode:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->combo_pf_choice), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Type:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->combo_pf_dtype), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->radio_pf_random), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->radio_pf_manual), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), gtk_label_new("Taille:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), GTK_WIDGET(app->spin_pf_size), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), btn_gen, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_config), btn_reset, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_config, FALSE, FALSE, 5);

    // --- Operations ---
    GtkWidget *frame_ops = gtk_frame_new("Opérations");
    GtkWidget *box_ops = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box_ops), 5);
    gtk_container_add(GTK_CONTAINER(frame_ops), box_ops);

    app->entry_pf_val = GTK_ENTRY(gtk_entry_new());
    gtk_entry_set_placeholder_text(app->entry_pf_val, "Valeur");

    app->btn_pf_action1 = gtk_button_new_with_label("Empiler");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_pf_action1), "btn-green");

    app->btn_pf_action2 = gtk_button_new_with_label("Dépiler");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_pf_action2), "btn-red");

    app->btn_pf_action3 = gtk_button_new_with_label("Sommet");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_pf_action3), "btn-blue");

    app->btn_pf_clear = gtk_button_new_with_label("Vider tout");
    gtk_style_context_add_class(gtk_widget_get_style_context(app->btn_pf_clear), "btn-orange"); // Reuse orange for now



    g_signal_connect(app->btn_pf_action1, "clicked", G_CALLBACK(on_btn_pf_action1_clicked), app);
    g_signal_connect(app->btn_pf_action2, "clicked", G_CALLBACK(on_btn_pf_action2_clicked), app);
    g_signal_connect(app->btn_pf_action3, "clicked", G_CALLBACK(on_btn_pf_action3_clicked), app);
    g_signal_connect(app->btn_pf_clear, "clicked", G_CALLBACK(on_btn_pf_reset_clicked), app);


    gtk_box_pack_start(GTK_BOX(box_ops), gtk_label_new("Saisie:"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_ops), GTK_WIDGET(app->entry_pf_val), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_ops), app->btn_pf_action1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_ops), app->btn_pf_action2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_ops), app->btn_pf_action3, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box_ops), app->btn_pf_clear, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(page), frame_ops, FALSE, FALSE, 5);

    // --- Viz ---
    GtkWidget *frame_viz = gtk_frame_new("Visualisation Graphique");
    app->drawing_area_pf = gtk_drawing_area_new();
    // Set min size
    gtk_widget_set_size_request(app->drawing_area_pf, 600, 400);
    g_signal_connect(app->drawing_area_pf, "draw", G_CALLBACK(on_draw_pile_file), app);
    gtk_container_add(GTK_CONTAINER(frame_viz), app->drawing_area_pf);
    gtk_box_pack_start(GTK_BOX(page), frame_viz, TRUE, TRUE, 0);

    // --- Infos ---
    GtkWidget *box_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    app->lbl_pf_status = GTK_LABEL(gtk_label_new("Taille: 0"));
    app->lbl_pf_message = GTK_LABEL(gtk_label_new("Prêt."));

    // Make message bold
    PangoAttrList *attr_list = pango_attr_list_new();
    pango_attr_list_insert(attr_list, pango_attr_weight_new(PANGO_WEIGHT_BOLD));
    gtk_label_set_attributes(app->lbl_pf_message, attr_list);
    pango_attr_list_unref(attr_list);

    gtk_box_pack_start(GTK_BOX(box_info), GTK_WIDGET(app->lbl_pf_status), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box_info), GTK_WIDGET(app->lbl_pf_message), TRUE, TRUE, 0); // Center msg

    gtk_box_pack_start(GTK_BOX(page), box_info, FALSE, FALSE, 10);

    gtk_stack_add_named(GTK_STACK(app->stack), page, "piles_files");

    // Init Defaults
    init_stack(&app->current_stack, TYPE_INT, 20);
    init_queue(&app->current_queue, TYPE_INT, 20);
    app->pile_file_mode = 0; // Stack
}
