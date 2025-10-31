#ifndef KLASKER_DOCUMENT_H
#define KLASKER_DOCUMENT_H

#include <gtk/gtk.h>
#include <glib.h>

/*
 * Klasker Document Parser (Gumbo-based)
 * -------------------------------------
 * Parses HTML and inserts readable text into a GtkTextBuffer.
 */

void klasker_parse_document(const gchar *html, GtkTextBuffer *buffer);

#endif /* KLASKER_DOCUMENT_H */
