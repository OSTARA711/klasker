#include "main.h"
#include "html.h"     // for klasker_display_html()
#include "net.h"

#include <glib.h>
#include <libsoup/soup.h>
#include <string.h>

/* -------------------------------------------------------
 *  Global SoupSession (shared by all network operations)
 * ------------------------------------------------------- */
static SoupSession *klasker_session = NULL;

/* Initialize the shared session once */
void klasker_net_init(void) {
    if (!klasker_session)
        klasker_session = soup_session_new();
}

/* Container for transferring data to the main GTK thread */
typedef struct {
    gchar *body;
    gint status;
    GtkTextView *view;
} FetchResult;

/* Cleanup helper */
static void fetch_result_free(gpointer data) {
    FetchResult *r = data;
    if (!r) return;
    g_free(r->body);
    g_free(r);
}

/* -------------------------------------------------------
 *  Fetch completed (runs on main GTK thread)
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
 *  Threaded fetch (runs in worker thread)
 * ------------------------------------------------------- */
static gpointer fetch_thread_func(gpointer data) {
    FetchResult *result = data;

    SoupMessage *msg = soup_message_new("GET", result->body);
    if (!msg) {
        result->status = SOUP_STATUS_MALFORMED;
        result->body = g_strdup("Invalid URL");
        g_idle_add(fetch_idle_cb, result);
        return NULL;
    }

    SoupMessage *res = msg;
    result->status = soup_session_send_message(klasker_session, res);

    if (result->status == SOUP_STATUS_OK && res->response_body && res->response_body->data) {
    result->body = g_strdup(res->response_body->data);

    // Debug: print first few chars of response to console
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
 *  Public function to start a fetch
 * ------------------------------------------------------- */
void klasker_fetch_url(const gchar *url, GtkTextView *text_view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    gtk_text_buffer_set_text(buffer, "Fetching...", -1);

    if (!url || !*url)
        return;

    FetchResult *result = g_new0(FetchResult, 1);
    result->body = g_strdup(url);   // temporarily holds URL until thread reads it
    result->status = 0;
    result->view = text_view;

    g_thread_new("klasker-fetch", fetch_thread_func, result);
}
