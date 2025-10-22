#include "html.h"

/* Basic display function: cleans text and shows it in GtkTextView */
void klasker_display_html(const gchar *html, GtkTextView *text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

    if (!html || !*html) {
        gtk_text_buffer_set_text(buffer, "(No HTML content)", -1);
        return;
    }

    // Very simple HTML stripping — future Gumbo integration placeholder
    gchar *cleaned = g_strdup(html);

    // Replace < and > with spaces to avoid raw tags
    for (char *p = cleaned; *p; p++) {
        if (*p == '<' || *p == '>') *p = ' ';
    }

    gtk_text_buffer_set_text(buffer, cleaned, -1);
    g_free(cleaned);
}
