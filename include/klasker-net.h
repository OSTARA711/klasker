#ifndef KLASKER_NET_H
#define KLASKER_NET_H

#include <libsoup/soup.h>

/*
 * Klasker Networking Core (libsoup-2.4)
 * --------------------------------------
 * Provides the shared SoupSession and initialization logic
 * for higher-level modules (like net.c or document.c).
 */

void klasker_net_init(void);
SoupSession *klasker_get_session(void);
void klasker_net_cleanup(void);

#endif /* KLASKER_NET_H */
