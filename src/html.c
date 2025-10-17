// html.c
#include <stdio.h>
#include <string.h>
#include "klasker.h"

void klasker_parse_html_stub(const char *html) {
    printf("[Parser Stub] Parsing HTML title...\n");
    const char *start = strstr(html, "<title>");
    const char *end = strstr(html, "</title>");
    if (start && end) {
        start += 7; // skip "<title>"
        size_t len = end - start;
        char title[256] = {0};
        strncpy(title, start, len < 255 ? len : 255);
        printf("[Parser Stub] Title: %s\n", title);
    } else {
        printf("[Parser Stub] Title not found\n");
    }
}
