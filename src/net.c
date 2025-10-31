#include "klasker-net.h"
#include "html.h"     // for klasker_display_html()

#include <glib.h>
#include <libsoup/soup.h>
#include <string.h>

/* -------------------------------------------------------
 *  Global SoupSession shared across all network operations
 * ------------------------------------------------------- */
static SoupSession *klasker_session = NULL;

/* Initialize session (call once at startup) */
void klasker_net_init(void) {
    if (!klasker_session)
        klasker_session = soup_session_new();
}

/* Cleanup session at exit */
void klasker_net_cleanup(void) {
    if (klasker_session) {
        g_object_unref(klasker_session);
        klasker_session = NULL;
    }
}

/* Getter for other modules to access the session */
SoupSession *klasker_get_session(void) {
    return klasker_session;
}

/* -------------------------------------------------------
 *  Data container for async fetches
 * ------------------------------------------------------- */
typedef struct {
    gchar *url;
    gchar *body;
    gint status;
    GtkTextView *view;
} FetchResult;

/* Free a FetchResult */
static void fetch_result_free(gpointer data) {
    FetchResult *r = data;
    if (!r) return;
    g_free(r->url);
    g_free(r->body);
    g_free(r);
}

/* -------------------------------------------------------
 *  Callback: runs on the main GTK thread after fetch completes
 * ------------------------------------------------------- */
static gboolean fetch_idle_cb(gpointer user_data) {
    FetchResult *result = user_data;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(result->view);

    gtk_text_buffer_set_text(buffer, "", -1);  // clear previous content

    if (result->status == SOUP_STATUS_OK && result->body) {
        klasker_display_html(result->body, GTK_TEXT_VIEW(result->view));
    } else {
        gchar *msg = g_strdup_printf("HTTP Error: %d", result->status);
        gtk_text_buffer_set_text(buffer, msg, -1);
        g_free(msg);
    }

    fetch_result_free(result);
    return G_SOURCE_REMOVE;
}

/* -------------------------------------------------------
 *  Worker thread: runs in background to fetch URL
 * ------------------------------------------------------- */
static gpointer fetch_thread_func(gpointer data) {
    FetchResult *result = data;
    SoupSession *session = klasker_get_session();

    SoupMessage *msg = soup_message_new("GET", result->url);
    if (!msg) {
        result->status = SOUP_STATUS_MALFORMED;
        result->body = g_strdup("Invalid URL");
        g_idle_add(fetch_idle_cb, result);
        return NULL;
    }

    result->status = soup_session_send_message(session, msg);

    if (result->status == SOUP_STATUS_OK &&
        msg->response_body && msg->response_body->data) {

        result->body = g_strdup(msg->response_body->data);

        // Optional debug output
        g_print("[Klasker] Downloaded %zu bytes\n", strlen(result->body));
        g_print("[Klasker] First 200 chars:\n%.200s\n\n", result->body);

    } else {
        result->body = g_strdup_printf("HTTP Error: %d", result->status);
    }

    g_idle_add(fetch_idle_cb, result);
    g_object_unref(msg);
    return NULL;
}

/* -------------------------------------------------------
 *  Public API: start asynchronous fetch
 * ------------------------------------------------------- */
void klasker_fetch_url(const gchar *url, GtkTextView *text_view) {
    if (!url || !*url) return;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, "Fetching...", -1);

    FetchResult *result = g_new0(FetchResult, 1);
    result->url = g_strdup(url);
    result->status = 0;
    result->view = text_view;

    g_thread_new("klasker-fetch", fetch_thread_func, result);
}
