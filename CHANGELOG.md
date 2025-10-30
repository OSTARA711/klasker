# Changelog

All notable changes to Klasker GTK Browser will be documented in this file.

NOTE: The likelihood that this is the only project on which we will ever work is pretty low and as we engage with exciting new projects, we tend to forget concepts in previous work. When it comes time to revisit this past work, it will take time to remember everything that was done. Furthermore, if someone else comes on board between the time when we started to build this project and when we resume the work on it, they will have to read through the source code and whatever notes (have we left any?) we may have written inline, in order to understand what was done already and what still needs to be done... adding to the amount of time it takes to complete a project of this magnitude.

A good changelog also tells our users & contributors which improvements they will get from an upgrade.



## [Alpha 1.1] – 2025-10-30

- Improved HTML Processing and Parsing Layer
- Introduced Gumbo-based DOM parsing to transform raw HTML into clean, readable plaintext.
- Implemented modular document.c / document.h to handle DOM iteration and text extraction.
- Updated html.c to integrate the new document parser and pass extracted text to GTK text buffer.

- Streamlined network → parse → display pipeline using asynchronous libsoup-2.4.

- Verified lower RAM footprint (~34.5 MB idle / 64.5 MB under load) with smoother memory curves.
- Removed redundant terminal logging from net.c for quieter runtime output.
- Confirmed stable operation under Ubuntu/Zorin 22.04 (Jammy Jellyfish).

## [Alpha 1.1] - 2025-10-27

- Document abstraction layer added (document.c / document.h)
- Introduced an internal KlaskerDocument structure to handle HTML content and prepare for DOM parsing, styling, and content rendering.
- Improved memory separation between network retrieval (net.c) and HTML interpretation (html.c).

- Gumbo HTML parser integrated (gumbo 0.10.1)
- HTML documents are now parsed into DOM trees, enabling text node and tag extraction.
- Browser currently extracts titles and major textual nodes such as Paragraph, Hyperlink, and Title elements.

  ### RAM performance optimized:
- Idle consumption reduced to ~34.4 MB, page load peaks smoother (avg. ≈ 110 MB) with better stability and reduced GC churn.
- Klasker now sustains large modern websites like CNN with efficient memory release after parsing, down to ~62 MB of RAM consumption.

  ### Internal architecture refactorisation:
- Clearer module responsibilities between net.c, html.c and document.c.
- Cleaner callback flow between libsoup asynchronous fetch and Gumbo parsing.
- Enhanced build structure (Meson/Ninja) with modular dependency checks.

  ### Technical highlights
- Network layer: still powered by libsoup-2.4 (v2.74.2 confirmed)
- Parsing layer: integrated Gumbo parser for DOM tree construction
- GUI layer: GTK+3.24.33, still minimal but stable
- Logging: improved diagnostic messages during fetch, parse and render phases.

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
