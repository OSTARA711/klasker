#include <gtk/gtk.h>
#include "document.h"

void klasker_display_html(const gchar *html, GtkTextView *view) {
    if (!html || !view)
        return;

    KlDocument *doc = klasker_document_parse(html);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    gtk_text_buffer_set_text(buffer, "", -1);

    if (!doc) {
        gtk_text_buffer_set_text(buffer, "[Parse error]", -1);
        return;
    }

    GString *content = g_string_new("");

    if (doc->title)
        g_string_append_printf(content, "Title: %s\n\n", doc->title);

    for (GList *l = doc->nodes; l; l = l->next) {
        KlNode *n = l->data;
        switch (n->type) {
            case KL_NODE_HEADING:
                g_string_append_printf(content, "[H1] %s\n\n", n->text);
                break;
            case KL_NODE_PARAGRAPH:
                g_string_append_printf(content, "%s\n\n", n->text);
                break;
            case KL_NODE_LINK:
                g_string_append_printf(content, "[LINK] %s\n", n->text);
                break;
        }
    }

    gtk_text_buffer_set_text(buffer, content->str, -1);
    g_string_free(content, TRUE);
    klasker_document_free(doc);
}
