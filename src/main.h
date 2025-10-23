#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <libsoup/soup.h>

/* UI widgets */
extern GtkWidget *entry;
extern GtkWidget *text_view;

/* Global libsoup session */
extern SoupSession *klasker_session;

/* API */
void klasker_display_html(const gchar *html, GtkTextView *text_view);
void klasker_fetch_url(const gchar *url, GtkTextView *text_view);

#endif /* MAIN_H */
