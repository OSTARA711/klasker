#ifndef KLASKER_DOCUMENT_H
#define KLASKER_DOCUMENT_H

#include <glib.h>
#include <gumbo.h>

/*
 * Represents a lightweight parsed HTML document.
 */
typedef struct {
    gchar *title;
    GList *links;   // list of gchar* hrefs
    GList *images;  // list of gchar* srcs
} KlDocument;

/*
 * Parse HTML into a KlDocument.
 */
KlDocument *klasker_document_parse(const gchar *html);

/*
 * Free all memory associated with the document.
 */
void klasker_document_free(KlDocument *doc);

#endif /* KLASKER_DOCUMENT_H */
