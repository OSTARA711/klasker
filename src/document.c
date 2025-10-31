#include "klasker-document.h"
#include <gumbo.h>
#include <gtk/gtk.h>
#include <string.h>

/*
 * Klasker HTML Document Parser
 * ----------------------------
 * Uses the Gumbo parser to extract visible text and key attributes.
 */

static void klasker_traverse_node(GumboNode *node, GtkTextBuffer *buffer) {
    if (node->type == GUMBO_NODE_TEXT) {
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer, &end);
        gtk_text_buffer_insert(buffer, &end, node->v.text.text, -1);
        gtk_text_buffer_insert(buffer, &end, "\n", -1);
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        GumboElement *element = &node->v.element;
        GumboTag tag = element->tag;

        // Skip unwanted tags
        if (tag == GUMBO_TAG_SCRIPT || tag == GUMBO_TAG_STYLE ||
            tag == GUMBO_TAG_NOSCRIPT || tag == GUMBO_TAG_META ||
            tag == GUMBO_TAG_LINK)
            return;

        // Extract href/src/alt if present
        GumboVector *attrs = &element->attributes;
        for (unsigned int i = 0; i < attrs->length; i++) {
            GumboAttribute *attr = attrs->data[i];
            if (!attr->name || !attr->value) continue;

            GtkTextIter end;
            gtk_text_buffer_get_end_iter(buffer, &end);
            if (strcmp(attr->name, "href") == 0) {
                gtk_text_buffer_insert(buffer, &end, "[Link: ", -1);
                gtk_text_buffer_insert(buffer, &end, attr->value, -1);
                gtk_text_buffer_insert(buffer, &end, "]\n", -1);
            } else if (strcmp(attr->name, "src") == 0) {
                gtk_text_buffer_insert(buffer, &end, "[Image src: ", -1);
                gtk_text_buffer_insert(buffer, &end, attr->value, -1);
                gtk_text_buffer_insert(buffer, &end, "]\n", -1);
            } else if (strcmp(attr->name, "alt") == 0) {
                gtk_text_buffer_insert(buffer, &end, "[Alt: ", -1);
                gtk_text_buffer_insert(buffer, &end, attr->value, -1);
                gtk_text_buffer_insert(buffer, &end, "]\n", -1);
            }
        }

        // Recursively traverse children
        GumboVector *children = &element->children;
        for (unsigned int i = 0; i < children->length; i++) {
            klasker_traverse_node(children->data[i], buffer);
        }
    }
}

void klasker_parse_document(const gchar *html, GtkTextBuffer *buffer) {
    GumboOutput *output = gumbo_parse(html);
    gtk_text_buffer_set_text(buffer, "", -1);
    klasker_traverse_node(output->root, buffer);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}
