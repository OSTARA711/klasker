#include "klasker-net.h"
#include <glib-object.h>

static SoupSession *session = NULL;

void klasker_net_init(void) {
    if (!session)
        session = soup_session_new();
}

SoupSession *klasker_get_session(void) {
    return session;
}

void klasker_net_cleanup(void) {
    if (session) {
        g_object_unref(session);
        session = NULL;
    }
}
