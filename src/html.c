#include <stdio.h>
#include <string.h>
#include "klasker.h"

void klasker_parse_html_stub(const char *html) {
    printf("[Parser Stub] Parsing HTML title...\n");

    // Very simple placeholder logic
    const char *start = strstr(html, "<title>");
    const char *end   = strstr(html, "</title>");
    if (start && end && start < end) {
        start += 7; // skip "<title>"
        char title[256];
        size_t len = end - start;
        if (len >= sizeof(title)) len = sizeof(title) - 1;
        strncpy(title, start, len);
        title[len] = '\0';
        printf("[Parser Stub] Title: %s\n", title);
    } else {
        printf("[Parser Stub] No <title> tag found.\n");
    }
}
