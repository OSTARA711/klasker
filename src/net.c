#include <stdio.h>
#include <libsoup/soup.h>
#include "klasker.h"

void klasker_fetch_async(const char *url) {
    printf("[Fetch Stub] URL requested: %s\n", url);

    // Dummy HTML body
    const char *html = "<html><head><title>Dummy Page</title></head><body>Hello world</body></html>";
    printf("[Fetch Stub] HTML body:\n%s\n", html);

    // Call parser stub (declared in klasker.h, defined in html.c)
    klasker_parse_html_stub(html);
}
