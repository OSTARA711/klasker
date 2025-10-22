#include "main.h"
#include <libsoup/soup.h>
#include <gtk/gtk.h>

static void on_response(GObject *source_object, GAsyncResult *res, gpointer user_data);

void klasker_fetch_url(const gchar *url, GtkTextView *view) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    gtk_text_buffer_set_text(buffer, "Fetching...\n", -1);

    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new("GET", url);

    /* Send asynchronously */
    soup_session_send_async(session, msg, NULL, on_response, view);
}

static void on_response(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GtkTextView *view = GTK_TEXT_VIEW(user_data);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    GtkTextBuffer *log_buffer = buffer;

    SoupSession *session = SOUP_SESSION(source_object);
    GError *error = NULL;
    GInputStream *stream = soup_session_send_finish(session, res, &error);

    if (error) {
        gtk_text_buffer_set_text(log_buffer, error->message, -1);
        g_error_free(error);
        g_object_unref(session);
        return;
    }

    /* Read response body */
    GString *body = g_string_new(NULL);
    char chunk[4096];
    gssize nread;
    while ((nread = g_input_stream_read(stream, chunk, sizeof(chunk), NULL, NULL)) > 0) {
        g_string_append_len(body, chunk, nread);
    }

    if (body->len > 0) {
        klasker_display_html(body->str, view);
    } else {
        gtk_text_buffer_set_text(log_buffer, "Empty response.\n", -1);
    }

    g_string_free(body, TRUE);
    g_object_unref(stream);
    g_object_unref(session);
}
