#include <gtk/gtk.h>
#include "net.h"
#include "html.h"

static void on_app_activate(GApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Klasker");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("org.klasker.browser", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
