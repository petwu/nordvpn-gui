---
title: ☯ Compatibility
summary: Notes on the compatibility of NordVPN CLI and GUI.
weight: 4
---

Since this application only operates on top of the NordVPN CLI (`nordvpn`),
there are some requirements when it comes to compatibility.

Of course, you are best off with the latest version, but it's recommended to use
at least NordVPN CLI `v3.7` (2020-03-12) for NordVPN GUI `>= v1.0`.

Why `v3.7`?
- It's the version this app was developed with initially.
- The new command `nordvpn set defaults` is required to restore the default
  settings.
- As of the time writing (`v3.8.10`, 2021-01-26), there were no changes to the
  CLI for almost a year. Hence at CLI is considered to be relatively stable.
  There were much more changes between `v3.7` (2020-03-12) and the first public
  release `v1.1.5` (2018-08-21). You can take a look at NordVPN's changelog
  [here](https://repo.nordvpn.com/deb/nordvpn/debian/pool/main).
- Older versions are not tested and therefore not guaranteed to be working.
