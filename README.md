<!-- omit in toc -->
# <img src="res/img/logo.svg" height="24" />&nbsp; NordVPN GUI (Unofficial)

A small Qt applications that provides a graphical user interface (GUI) on top of
the NordVPN command line client for Linux.

> !!! WORK IN PROGRESS !!!

<!-- omit in toc -->
## Content

- [About](#about)
- [Quick Peek](#quick-peek)
- [Features](#features)
- [Install](#install)
- [Building](#building)
  - [0. Requirements](#0-requirements)
  - [1. Building an Executable](#1-building-an-executable)
  - [2. Install](#2-install)
  - [3. Building a Package](#3-building-a-package)
- [Changelog](#changelog)
- [Donate](#donate)
- [Disclaimer](#disclaimer)
  - [Nothing Official](#nothing-official)
  - [Release and Use](#release-and-use)

## About

Most information can be found on the
[project website](https://petwu.github.io/nordvpn-gui).

## Quick Peek

![Screenshot Light](https://petwu.github.io/nordvpn-gui/img/landing-page-screenshot-light.png)
![Screenshot Dark](https://petwu.github.io/nordvpn-gui/img/landing-page-screenshot-dark.png)

## Features

- Control most features of the CLI with a few clicks in a nice GUI
- Interactive map (drag, zoom, click pin to connect)
- Connect to country, city/region or a specific server
- Connect to specialty servers (Double VPN, Onion over VPN, P2P)
- Fetch server list from NordVPN API
- Display status information
- Tray icon that changes color when connected/disconnected
- Settings window for CLI settings
- Login/Logout
- Adapts to system color theme (dark/light)
- Multi-linguage (currently English, German)
- Open Source (GPLv3)

## Install

See [here](https://petwu.github.io/nordvpn-gui/docs/install) on how to install
pre-built packages.

## Building

If there are no pre-built packages or you prefer building from source, here you
go ...

### 0. Requirements

- GCC or Clang with C++17 support
- CMake >= 3.14
- Ninja >= 1.8
- Qt >=5.12
- Conan C++ Package Manager

> For a detailed list of required packages for Ubuntu or Ubuntu based
> distributios see the install steps in the [Dockerfile](Dockerfile).

### 1. Building an Executable

You can build NordVPN GUI by using the following commands:

```sh
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

You will then get a executable `bin/nordvpn-gui` as well as some resource files
in `share`.

### 2. Install

To install the built binary, resource files, .desktop file and icons:

```sh
ninja install # probably requires running with sudo
```

### 3. Building a Package

Build DEB and RPM packages:

```sh
ninja package
```

The `.deb` and `.rpm` files are in `packages`.

## Changelog

See [CHANGELOG.md](CHANGELOG.md) or
[on the website](https://petwu.github.io/nordvpn-gui/docs/changelog).

## Donate

If you like the app, consider supporting my work by buying me a coffee or two.
See [here](https://petwu.github.io/nordvpn-gui/docs/donate) for details.

## Disclaimer

### Nothing Official

This is an *UNOFFICIAL* application that was *not* developed by NordVPN.
The developer has no ties nor affiliation whatsoever with NordVPN, its services,
nor its software.

### Release and Use

This application was made for personal needs and use. The code is released on
the off chance it might be of use to someone. While I will try to respond to
any kind of feedback (issues, bugs, features requests, comments, ...), please
understand that i may not be able to respond immediately. Thank you!
