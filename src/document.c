#include "document.h"
#include <string.h>

/*
 * Internal recursive function to extract <a> and <img> attributes.
 */
static void klasker_extract_nodes(GumboNode *node, KlDocument *doc) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboElement *element = &node->v.element;
    // const gchar *tag_name = gumbo_normalized_tagname(element->tag);

    if (element->tag == GUMBO_TAG_TITLE) {
        if (element->children.length > 0) {
            GumboNode *text_node = element->children.data[0];
            if (text_node->type == GUMBO_NODE_TEXT)
                doc->title = g_strdup(text_node->v.text.text);
        }
    }

    if (element->tag == GUMBO_TAG_A) {
        GumboAttribute *href = gumbo_get_attribute(&element->attributes, "href");
        if (href)
            doc->links = g_list_append(doc->links, g_strdup(href->value));
    }

    if (element->tag == GUMBO_TAG_IMG) {
        GumboAttribute *src = gumbo_get_attribute(&element->attributes, "src");
        if (src)
            doc->images = g_list_append(doc->images, g_strdup(src->value));
    }

    // Recurse into children
    for (unsigned int i = 0; i < element->children.length; i++)
        klasker_extract_nodes(element->children.data[i], doc);
}

/*
 * Main parsing entry point.
 */
KlDocument *klasker_document_parse(const gchar *html) {
    if (!html)
        return NULL;

    KlDocument *doc = g_new0(KlDocument, 1);
    GumboOutput *output = gumbo_parse(html);

    klasker_extract_nodes(output->root, doc);
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    return doc;
}

/*
 * Memory cleanup.
 */
void klasker_document_free(KlDocument *doc) {
    if (!doc)
        return;

    g_free(doc->title);
    g_list_free_full(doc->links, g_free);
    g_list_free_full(doc->images, g_free);
    g_free(doc);
}
