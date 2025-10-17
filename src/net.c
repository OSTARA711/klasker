#include <stdio.h>
#include <libsoup/soup.h>
#include "klasker.h"

static void on_response(SoupSession *session, SoupMessage *msg, gpointer user_data) {
    (void)session;
    (void)user_data;

    char buf[512];

    if (msg->status_code == SOUP_STATUS_OK) {
        snprintf(buf, sizeof(buf), "[Network] Response received (%d bytes)",
                 (int)msg->response_body->length);
        klasker_ui_log_info(buf);
        klasker_parse_html_stub(msg->response_body->data);
    } else {
        SoupURI *uri = soup_message_get_uri(msg);
        const char *uri_str = uri ? soup_uri_to_string(uri, FALSE) : "(unknown)";
        snprintf(buf, sizeof(buf), "[Network] Failed to fetch URL: %s (status %d)",
                 uri_str, msg->status_code);
        klasker_ui_log_info(buf);
    }
}

void klasker_fetch_async(const char *url) {
    char buf[256];
    snprintf(buf, sizeof(buf), "[Network] Fetching URL asynchronously: %s", url);
    klasker_ui_log_info(buf);

    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new("GET", url);
    if (!msg) {
        klasker_ui_log_info("[Network] Failed to create message.");
        g_object_unref(session);
        return;
    }

    soup_session_queue_message(session, msg, on_response, NULL);
}
