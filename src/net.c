#include <stdio.h>
#include <libsoup/soup.h>
#include "klasker.h"

// Async callback for SoupSession
static void on_response(SoupSession *session, SoupMessage *msg, gpointer user_data) {
    (void)session;
    (void)user_data;

    if (msg->status_code == SOUP_STATUS_OK) {
        printf("[Network] Response received (%d bytes)\n", (int)msg->response_body->length);
        klasker_parse_html_stub(msg->response_body->data);
    } else {
        SoupURI *uri = soup_message_get_uri(msg);
        const char *uri_str = uri ? soup_uri_to_string(uri, FALSE) : "(unknown)";
        printf("[Network] Failed to fetch URL: %s (status %d)\n", uri_str, msg->status_code);
    }
}

void klasker_fetch_async(const char *url) {
    printf("[Network] Fetching URL asynchronously: %s\n", url);

    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new("GET", url);
    if (!msg) {
        printf("[Network] Failed to create message.\n");
        g_object_unref(session);
        return;
    }

    soup_session_queue_message(session, msg, on_response, NULL);
}
