#include "document.h"
#include "klasker-net.h"
#include <gumbo.h>
#include <gtk/gtk.h>
#include <string.h>

/* Helper: insert text with optional link */
static void insert_text_with_link(GtkTextBuffer *buffer, const gchar *text, const gchar *href) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    if (href) {
        GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
        GtkTextTag *link_tag = gtk_text_tag_table_lookup(tag_table, "link");
        if (!link_tag) {
            link_tag = gtk_text_buffer_create_tag(buffer,
                                                 "link",
                                                 "foreground", "blue",
                                                 "underline", PANGO_UNDERLINE_SINGLE,
                                                 NULL);
        }
        gtk_text_buffer_insert_with_tags_by_name(buffer, &end, text, -1, "link", NULL);
        g_object_set_data(G_OBJECT(link_tag), "href", g_strdup(href));
    } else {
        gtk_text_buffer_insert(buffer, &end, text, -1);
    }
    gtk_text_buffer_insert(buffer, &end, "\n", -1);
}

/* Recursive traversal of Gumbo nodes */
static void klasker_traverse_node(GumboNode *node, GtkTextBuffer *buffer) {
    if (!node) return;

    if (node->type == GUMBO_NODE_TEXT) {
        insert_text_with_link(buffer, node->v.text.text, NULL);
    } else if (node->type == GUMBO_NODE_ELEMENT) {
        GumboElement *element = &node->v.element;
        GumboTag tag = element->tag;

        // Skip unwanted tags
        if (tag == GUMBO_TAG_SCRIPT || tag == GUMBO_TAG_STYLE ||
            tag == GUMBO_TAG_NOSCRIPT || tag == GUMBO_TAG_META ||
            tag == GUMBO_TAG_LINK)
            return;

        // Check for <a href="...">
        const gchar *href = NULL;
        if (tag == GUMBO_TAG_A) {
            GumboVector *attrs = &element->attributes;
            for (unsigned int i = 0; i < attrs->length; i++) {
                GumboAttribute *attr = (GumboAttribute *)attrs->data[i];
                if (strcmp(attr->name, "href") == 0) {
                    href = attr->value;
                    break;
                }
            }
        }

        // Traverse children
        GumboVector *children = &element->children;
        for (unsigned int i = 0; i < children->length; i++) {
            GumboNode *child_node = (GumboNode *)children->data[i];
            if (tag == GUMBO_TAG_A && href) {
                if (child_node->type == GUMBO_NODE_TEXT) {
                    insert_text_with_link(buffer, child_node->v.text.text, href);
                } else {
                    klasker_traverse_node(child_node, buffer);
                }
            } else {
                klasker_traverse_node(child_node, buffer);
            }
        }
    }
}

void klasker_parse_document(const gchar *html, GtkTextBuffer *buffer) {
    GumboOutput *output = gumbo_parse(html);
    gtk_text_buffer_set_text(buffer, "", -1);
    klasker_traverse_node(output->root, buffer);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}
