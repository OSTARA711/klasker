#ifndef KLASKER_DOCUMENT_H
#define KLASKER_DOCUMENT_H

#include <glib.h>
#include <gumbo.h>

typedef enum {
    KL_NODE_PARAGRAPH,
    KL_NODE_HEADING,
    KL_NODE_LINK
} KlNodeType;

typedef struct {
    KlNodeType type;
    gchar *text;
} KlNode;

typedef struct {
    gchar *title;
    GList *nodes;  // list of KlNode*
} KlDocument;

KlDocument *klasker_document_parse(const gchar *html);
void klasker_document_free(KlDocument *doc);

#endif /* KLASKER_DOCUMENT_H */
