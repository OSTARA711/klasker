#include "main.h"
#include "html.h"
#include "net.h"
#include "klasker-net.h"  /* Ensure klasker_net_init() and klasker_net_cleanup() are declared */

static GtkWidget *entry;
static GtkWidget *text_view;
static GdkCursor *hand_cursor = NULL;
static GdkCursor *default_cursor = NULL;

/* --- Search button callback --- */
static void on_search_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;

    const gchar *url = gtk_entry_get_text(GTK_ENTRY(entry));
    if (url && *url)
        klasker_fetch_url(url, GTK_TEXT_VIEW(text_view));
}

/* --- TextView click handler --- */
static gboolean on_textview_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    (void)user_data;
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {
        GtkTextView *view = GTK_TEXT_VIEW(widget);
        GtkTextIter iter;
        gint x, y;

        gtk_text_view_window_to_buffer_coords(view, GTK_TEXT_WINDOW_WIDGET,
                                              (gint)event->x, (gint)event->y, &x, &y);
        gtk_text_view_get_iter_at_location(view, &iter, x, y);

        GSList *tags = gtk_text_iter_get_tags(&iter);
        for (GSList *l = tags; l != NULL; l = l->next) {
            GtkTextTag *tag = GTK_TEXT_TAG(l->data);
            const gchar *href = g_object_get_data(G_OBJECT(tag), "href");
            if (href) {
                g_print("[Klasker] Clicking link: %s\n", href);
                klasker_fetch_url(href, view);
                g_slist_free(tags);
                return TRUE;
            }
        }
        g_slist_free(tags);
    }
    return FALSE;
}

/* --- Mouse motion: change cursor when over a link --- */
static gboolean on_textview_motion(GtkWidget *widget, GdkEventMotion *event, gpointer user_data) {
    (void)user_data;
    GtkTextView *view = GTK_TEXT_VIEW(widget);
    GtkTextIter iter;
    gint x, y;

    gtk_text_view_window_to_buffer_coords(view, GTK_TEXT_WINDOW_WIDGET,
                                          (gint)event->x, (gint)event->y, &x, &y);
    gtk_text_view_get_iter_at_location(view, &iter, x, y);

    GSList *tags = gtk_text_iter_get_tags(&iter);
    gboolean over_link = FALSE;
    for (GSList *l = tags; l != NULL; l = l->next) {
        GtkTextTag *tag = GTK_TEXT_TAG(l->data);
        const gchar *href = g_object_get_data(G_OBJECT(tag), "href");
        if (href) {
            over_link = TRUE;
            break;
        }
    }
    g_slist_free(tags);

    GdkWindow *win = gtk_text_view_get_window(view, GTK_TEXT_WINDOW_TEXT);
    if (over_link && hand_cursor)
        gdk_window_set_cursor(win, hand_cursor);
    else if (default_cursor)
        gdk_window_set_cursor(win, default_cursor);

    return FALSE;
}

/* --- Simple function to mark text as a clickable link --- */
void klasker_mark_link(GtkTextBuffer *buffer, GtkTextIter *start, GtkTextIter *end, const gchar *href) {
    GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
    GtkTextTag *link_tag = gtk_text_tag_table_lookup(table, href);
    if (!link_tag) {
        link_tag = gtk_text_tag_new(NULL);
        g_object_set(link_tag,
                     "foreground", "blue",
                     "underline", PANGO_UNDERLINE_SINGLE,
                     NULL);
        gtk_text_tag_table_add(table, link_tag);
        g_object_set_data_full(G_OBJECT(link_tag), "href", g_strdup(href), g_free);
    }
    gtk_text_buffer_apply_tag(buffer, link_tag, start, end);
}

/* --- UI setup --- */
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    klasker_net_init();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Klasker Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* CSS */
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "entry { border: 2px solid #3399ff; border-radius: 4px; padding: 4px; }"
        "button { background-color: #3399ff; color: white; border-radius: 4px; padding: 4px 12px; }",
        -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    /* Layout */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    entry = gtk_entry_new();
    GtkWidget *button = gtk_button_new_with_label("Search");

    gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

    text_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* Connect signals */
    g_signal_connect(button, "clicked", G_CALLBACK(on_search_button_clicked), NULL);
    g_signal_connect(text_view, "button-press-event", G_CALLBACK(on_textview_click), NULL);
    g_signal_connect(text_view, "motion-notify-event", G_CALLBACK(on_textview_motion), NULL);
    gtk_widget_add_events(text_view, GDK_POINTER_MOTION_MASK);

    /* Prepare cursors */
    GdkDisplay *display = gdk_display_get_default();
    hand_cursor = gdk_cursor_new_from_name(display, "pointer");
    default_cursor = gdk_cursor_new_from_name(display, "text");

    gtk_widget_show_all(window);

    /* Start empty — no pre-filled example.com */
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter URL here...");

    gtk_main();

    klasker_net_cleanup();
    g_print("[Klasker] Network cleaned up\n");
    return 0;
}
