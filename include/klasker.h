#ifndef KLASKER_H
#define KLASKER_H

void klasker_fetch_async(const char *url);
void klasker_parse_html_stub(const char *html);

// UI logging helpers
void klasker_ui_log_info(const char *message);
void klasker_ui_log_title(const char *message);

#endif // KLASKER_H
