#include "document.h"
#include <string.h>

static void klasker_extract_nodes(GumboNode *node, KlDocument *doc);

static gchar *get_node_text(GumboNode *node) {
    if (node->type == GUMBO_NODE_TEXT)
        return g_strdup(node->v.text.text);

    if (node->type != GUMBO_NODE_ELEMENT)
        return NULL;

    GString *result = g_string_new(NULL);
    const GumboVector *children = &node->v.element.children;
    for (unsigned i = 0; i < children->length; ++i) {
        gchar *child_text = get_node_text(children->data[i]);
        if (child_text) {
            g_string_append(result, child_text);
            g_free(child_text);
        }
    }
    return g_string_free(result, FALSE);
}

static void klasker_extract_nodes(GumboNode *node, KlDocument *doc) {
    if (!node || node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboTag tag = node->v.element.tag;
    KlNode *n = NULL;

    if (tag == GUMBO_TAG_P) {
        n = g_new0(KlNode, 1);
        n->type = KL_NODE_PARAGRAPH;
        n->text = get_node_text(node);
    } else if (tag == GUMBO_TAG_A) {
        n = g_new0(KlNode, 1);
        n->type = KL_NODE_LINK;
        n->text = get_node_text(node);
    } else if (tag == GUMBO_TAG_H1) {
        n = g_new0(KlNode, 1);
        n->type = KL_NODE_HEADING;
        n->text = get_node_text(node);
    }

    if (n && n->text && *n->text)
        doc->nodes = g_list_append(doc->nodes, n);
    else if (n)
        g_free(n);

    // Recurse
    const GumboVector *children = &node->v.element.children;
    for (unsigned i = 0; i < children->length; ++i)
        klasker_extract_nodes(children->data[i], doc);
}

KlDocument *klasker_document_parse(const gchar *html) {
    if (!html) return NULL;

    GumboOutput *output = gumbo_parse(html);
    KlDocument *doc = g_new0(KlDocument, 1);

    // --- Title ---
    const GumboVector *root_children = &output->root->v.element.children;
    for (unsigned i = 0; i < root_children->length; ++i) {
        GumboNode *node = root_children->data[i];
        if (node->type == GUMBO_NODE_ELEMENT && node->v.element.tag == GUMBO_TAG_HEAD) {
            const GumboVector *children = &node->v.element.children;
            for (unsigned j = 0; j < children->length; ++j) {
                GumboNode *child = children->data[j];
                if (child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_TITLE) {
                    if (child->v.element.children.length > 0) {
                        GumboNode *text = child->v.element.children.data[0];
                        if (text->type == GUMBO_NODE_TEXT)
                            doc->title = g_strdup(text->v.text.text);
                    }
                }
            }
        }
    }

    // --- Extract content nodes ---
    klasker_extract_nodes(output->root, doc);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return doc;
}

void klasker_document_free(KlDocument *doc) {
    if (!doc) return;
    g_free(doc->title);

    for (GList *l = doc->nodes; l; l = l->next) {
        KlNode *n = l->data;
        g_free(n->text);
        g_free(n);
    }
    g_list_free(doc->nodes);
    g_free(doc);
}
