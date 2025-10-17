#include <gtk/gtk.h>
#include "klasker.h"

static void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    (void)user_data;
    const char *url = gtk_entry_get_text(entry);
    printf("[UI] Enter pressed: %s\n", url);
    klasker_fetch_async(url);
}

static void on_activate(GApplication *app, gpointer user_data) {
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Klasker");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *header = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), "Klasker");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter URL and press Enter");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), entry);
    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), NULL);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.klasker.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
