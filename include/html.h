#ifndef KLASKER_HTML_H
#define KLASKER_HTML_H

#include <gtk/gtk.h>

/*
 * Renders parsed HTML into a GtkTextView.
 * The actual parsing is handled by document.c via klasker_document_parse().
 */
void klasker_display_html(const gchar *html, GtkTextView *text_view);

#endif /* KLASKER_HTML_H */
