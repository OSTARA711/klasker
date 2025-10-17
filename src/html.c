#include <stdio.h>
#include <string.h>
#include <gumbo.h>
#include "klasker.h"

// Helper: extract title text from Gumbo node tree
static const char *find_title_text(GumboNode *node) {
    if (node->type != GUMBO_NODE_ELEMENT) return NULL;

    GumboVector *children = &node->v.element.children;
    if (node->v.element.tag == GUMBO_TAG_TITLE && children->length > 0) {
        GumboNode *title_text = children->data[0];
        if (title_text->type == GUMBO_NODE_TEXT) {
            return title_text->v.text.text;
        }
    }

    for (unsigned int i = 0; i < children->length; i++) {
        const char *result = find_title_text(children->data[i]);
        if (result) return result;
    }
    return NULL;
}

void klasker_parse_html_stub(const char *html) {
    klasker_ui_log_info("[Parser Stub] Parsing HTML title...");

    GumboOutput *output = gumbo_parse(html);
    const char *title = find_title_text(output->root);

    if (title) {
        char msg[512];
        snprintf(msg, sizeof(msg), "[Parser Stub] Title: %s", title);
        klasker_ui_log_title(msg);  // << blue & bold text in GTK
    } else {
        klasker_ui_log_info("[Parser Stub] No title found.");
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}
