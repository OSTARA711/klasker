# Changelog

All notable changes to Klasker will be documented in this file.

## [Alpha 1.0] - 2025-10-22
- Initial release of Klasker browser
- GTK3 interface with a search bar and “Fetch” button
- Asynchronous HTTP fetching implemented using libsoup-2.4
- Raw HTML display working (tested with https://cnn.com)
- Logging message “Fetching...” shown during network request
- First compilation and successful launch on Ubuntu/Zorin 22.04 "Jammy Jellyfish"

## [Pre-Alpha / Development] - 2025-10-17 to 2025-10-21
- Meson/Ninja build system integrated
- Initial `main.c`, `html.c`, `net.c` source files created
- GTK3 window layout with search bar and text view for content display
- Basic networking setup with libsoup-2.4
- Multiple build errors resolved related to function signatures and `GtkTextView` usage
- Internal testing of asynchronous callbacks
