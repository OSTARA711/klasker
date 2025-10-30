#include "main.h"
#include "html.h"
#include "net.h"
#include <libsoup/soup.h>

/* Global GTK widgets */
GtkWidget *entry;
GtkWidget *text_view;

/* Callback for Fetch button */
static void on_fetch_clicked(GtkButton *button, gpointer user_data) {
    (void)button; // silence unused warning
    GtkTextView *view = GTK_TEXT_VIEW(user_data);
    const gchar *url = gtk_entry_get_text(GTK_ENTRY(entry));

    if (url && *url) {
        klasker_fetch_url(url, view);
    }
}

/* Build main GTK window */
static GtkWidget *create_main_window(void) {
    GtkWidget *window, *vbox, *hbox, *fetch_button, *scrolled_window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Klasker Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* URL entry + Fetch button */
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter URL...");
    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);

    fetch_button = gtk_button_new_with_label("Fetch");
    gtk_box_pack_start(GTK_BOX(hbox), fetch_button, FALSE, FALSE, 0);

    /* Text view for HTML display */
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    /* Signals */
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(fetch_button, "clicked", G_CALLBACK(on_fetch_clicked), text_view);

    return window;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    /* Initialize network subsystem */
    klasker_net_init();

    GtkWidget *window = create_main_window();
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
