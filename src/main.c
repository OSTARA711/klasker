#include <gtk/gtk.h>
#include "klasker.h"

static GtkTextBuffer *log_buffer = NULL;
static GtkWidget *text_view = NULL;  // keep pointer to the text view

// Log function with optional color tag
void klasker_ui_log(const char *message, const char *tag) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(log_buffer, &end);
    gtk_text_buffer_insert_with_tags_by_name(log_buffer, &end, message, -1, tag, NULL);
    gtk_text_buffer_insert(log_buffer, &end, "\n", -1);

    // Auto-scroll
    GtkAdjustment *vadj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(text_view));
    gtk_adjustment_set_value(vadj, gtk_adjustment_get_upper(vadj));
}

// Convenience wrappers
void klasker_ui_log_info(const char *msg) { klasker_ui_log(msg, "info"); }
void klasker_ui_log_title(const char *msg) { klasker_ui_log(msg, "title"); }

static void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    (void)user_data;
    const char *url = gtk_entry_get_text(entry);

    char buf[256];
    snprintf(buf, sizeof(buf), "[UI] Enter pressed: %s", url);
    klasker_ui_log_info(buf);

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

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    log_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    // Define color/style tags
    gtk_text_buffer_create_tag(log_buffer, "info", "foreground", "gray50", NULL);
    gtk_text_buffer_create_tag(log_buffer, "title", "foreground", "blue", "weight", PANGO_WEIGHT_BOLD, NULL);

    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled), text_view);
    gtk_container_add(GTK_CONTAINER(window), scrolled);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.klasker.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
