#include <gtk/gtk.h>
#include "klasker.h"

static void on_activate(GApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Klasker Mini-Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // HeaderBar
    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Klasker");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    // URL entry
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter URL...");
    gtk_container_add(GTK_CONTAINER(window), entry);

    g_signal_connect(entry, "activate", G_CALLBACK(klasker_fetch_async), entry);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.klasker.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
