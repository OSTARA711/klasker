# Changelog

All notable changes to Klasker GTK Browser will be documented in this file.

NOTE: The likelihood that this is the only project on which we will ever work is pretty low and as we engage with exciting new projects, we tend to forget concepts in previous work. When it comes time to revisit this past work, it will take time to remember everything that was done. Furthermore, if someone else comes on board between the time when we started to build this project and when we resume the work on it, they will have to read through the source code and whatever notes (have we left any?) we may have written inline, in order to understand what was done already and what still needs to be done... adding to the amount of time it takes to complete a project of this magnitude.

A good changelog also tells our users & contributors which improvements they will get from an upgrade.



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
