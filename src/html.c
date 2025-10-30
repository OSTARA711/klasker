#include "html.h"
#include <gtk/gtk.h>
#include <string.h>

/*
 * klasker_display_html:
 * Displays the HTML text (for now, raw content).
 * Later we can replace this with real parsing logic.
 */
void klasker_display_html(const gchar *html, GtkTextView *text_view) {
    if (!text_view || !GTK_IS_TEXT_VIEW(text_view)) return;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);

    if (!html) {
        gtk_text_buffer_set_text(buffer, "(No content)", -1);
        return;
    }

    // Prevent UI lag for huge pages
    gsize len = strlen(html);
    if (len > 1000000) { // 1 MB cap for display
        gtk_text_buffer_set_text(
            buffer,
            "⚠️ Document too large to render.\n"
            "Displaying first 1000 characters:\n\n",
            -1
        );

        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, html, 1000);
        return;
    }

    gtk_text_buffer_set_text(buffer, html, -1);
}

/*
 * klasker_parse_html:
 * Stub for future Gumbo parser.
 */
void klasker_parse_html(const gchar *html, GtkTextBuffer *buffer) {
    gtk_text_buffer_set_text(buffer, html ? html : "(empty)", -1);
}
