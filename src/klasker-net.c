#include "klasker-net.h"
#include <libsoup/soup.h>
#include <glib.h>

/*
 * Klasker Networking Core (libsoup-2.4)
 * --------------------------------------
 * Manages the global SoupSession instance for the browser.
 */

static SoupSession *session = NULL;

/* Initialize the shared SoupSession */
void klasker_net_init(void) {
    if (!session) {
        session = soup_session_new();
        g_print("[Klasker] Network initialized (libsoup 2.4)\n");
    }
}

/* Retrieve the global SoupSession */
SoupSession *klasker_get_session(void) {
    return session;
}

/* Clean up the SoupSession */
void klasker_net_cleanup(void) {
    if (session) {
        g_object_unref(session);
        session = NULL;
        g_print("[Klasker] Network cleaned up\n");
    }
}
