// src/net.c
#include "main.h"
#include <glib.h>
#include <glib/gstdio.h>
#include <libsoup/soup.h>
#include <string.h>

/* A small container struct to pass result to the main thread */
typedef struct {
    gchar *body;            // owned, must be freed
    gint status;
    GtkTextView *view;      // not owned, used on main thread only
} FetchResult;

/* Free the fetch result */
static void fetch_result_free(gpointer data) {
    FetchResult *r = data;
    if (!r) return;
    g_free(r->body);
    g_free(r);
}

/* This runs on the main thread (via g_idle_add). It updates UI and calls display. */
static gboolean fetch_idle_cb(gpointer data) {
    FetchResult *r = data;
    if (!r) return G_SOURCE_REMOVE;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(r->view);

    if (r->status == SOUP_STATUS_OK && r->body && *r->body) {
        /* Replace buffer contents with the page (or delegate to DOM renderer) */
        /* klasker_display_html should free or copy as needed; we pass pointer (it won't free it) */
        klasker_display_html(r->body, r->view);
    } else {
        gchar msg[256];
        g_snprintf(msg, sizeof(msg), "Fetch failed (status %d)\n", r->status);
        gtk_text_buffer_set_text(buffer, msg, -1);
    }

    /* free result */
    fetch_result_free(r);
    return G_SOURCE_REMOVE;
}

/* Worker thread: performs a synchronous fetch using the shared session. */
static gpointer fetch_thread_func(gpointer user_data) {
    /* user_data is a struct containing the url and the GtkTextView pointer */
    struct {
        gchar *url;
        GtkTextView *view;
    } *task = user_data;

    FetchResult *result = g_new0(FetchResult, 1);
    result->body = NULL;
    result->status = 0;
    result->view = task->view;

    SoupMessage *msg = soup_message_new("GET", task->url);
    if (!msg) {
        result->status = -1;
        g_idle_add(fetch_idle_cb, result);
        g_free(task->url);
        g_free(task);
        return NULL;
    }

    /* Send synchronously on the shared session (this blocks the worker thread only) */
    soup_session_send_message(klasker_session, msg);

    result->status = msg->status_code;

    if (msg->response_body && msg->response_body->data) {
        /* duplicate the response body into a normal C string */
        result->body = g_strdup((const char *)msg->response_body->data);
    } else {
        result->body = g_strdup("");
    }

    /* IMPORTANT: destroy Gumbo output if you parse later.
       We don't parse here; parsing should be done on main thread (or produce a separate workflow). */

    /* unref message (we created it above) */
    g_object_unref(msg);

    /* schedule UI update on main loop */
    g_idle_add(fetch_idle_cb, result);

    /* cleanup */
    g_free(task->url);
    g_free(task);
    return NULL;
}

/* Public: start an async fetch (spawns worker thread). */
void klasker_fetch_url(const gchar *url, GtkTextView *view) {
    if (!url || !*url) return;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    gtk_text_buffer_set_text(buffer, "Fetching...\n", -1);

    /* Define a struct type for the task explicitly */
    typedef struct {
        gchar *url;
        GtkTextView *view;
    } FetchTask;

    FetchTask *task = g_new0(FetchTask, 1);
    task->url = g_strdup(url);
    task->view = view;

    GError *err = NULL;
    if (!g_thread_try_new("klasker-fetch-thread", fetch_thread_func, task, &err)) {
        gchar msg[256];
        g_snprintf(msg, sizeof(msg), "Failed to start fetch thread: %s\n",
                   err ? err->message : "unknown");
        gtk_text_buffer_set_text(buffer, msg, -1);
        g_clear_error(&err);
        g_free(task->url);
        g_free(task);
    }
}
