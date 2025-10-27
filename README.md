# Klasker Browser running on Linux/GTK

Klasker browser focuses exclusively on Linux systems (GTK frontend).

Goals:
- Lightweight browser for Linux desktop (GTK3)
- Modernized JS engine (QuickJS planned)
- Security hardening & faster builds.
- An ancient Celtic-inspired identity (Klasker = “Seeker” in Breton), since a browser is a seeker and displayer of webpages & information.

## LEGAL

🔹 Klasker

- No known trademarks (as of 2025) in the EUIPO or USPTO databases for Klasker.
- Not used commercially or in software names.
- Breton in form and meaning and some cognates appear in Gaulish personal names. Linguists reconstruct the Proto-Celtic ancestor as *klaskā- or *klask-yo-, meaning “to seek, to request.”

## Features to Implement

- Fetch and display HTML5 webpages from URLs
- Simple GTK3 interface with CSS3 and JavaScript support
- Cross-platform compatibility with Linux distros using GTK3
- Asynchronous fetching via `libsoup-2.4`

## Operational Requirements

- Linux OS (tested on Ubuntu/Zorin 22.04 "Jammy Jellyfish")
- GTK3 development libraries (`libgtk-3-dev`)
- libsoup-2.4 (`libsoup2.4-dev`)
- Gumbo HTML parser (`libgumbo-dev`)
- Ninja & Meson build system
