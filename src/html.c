#include "html.h"
#include "document.h"
#include <gtk/gtk.h>
#include <string.h>

void klasker_display_html(const gchar *html, GtkTextView *text_view) {
    if (!html || !text_view)
        return;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, "Rendering parsed HTML...", -1);

    // Parse and display readable text via Gumbo
    klasker_parse_document(html, buffer);
}
