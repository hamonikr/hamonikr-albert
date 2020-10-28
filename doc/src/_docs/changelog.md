---
layout: docs
title: Changelog
permalink: /docs/changelog/
---
This is the feature centric changelog. It may be a bit behind, since I update it in batches. For a complete changelog refer to the news and git log.

v0.14.14 (2017-12-06)

- New npm Python extension

v0.14.13 (2017-11-25)

- Rich text support
- New AUR Python extension
- New `scrot` Python extension


v0.14.12 (2017-11-23)

- New CopyQ Python extension (Ported from external extension)

v0.14.11 (2017-11-19)

- New `locate` Python extension

v0.14.10 (2017-11-16)

- Bugfixes

v0.14.9 (2017-11-16)

- Better HiDPI support
- New commandline option for debug output (-d)

v0.14.8 (2017-11-14)

- New Gnote Python extension (Ported from external extension)
- New Tomboy Python extension (Ported from external extension)
- New Pacman Python extension
- New Pass Python extension
- New Kill Python extension

v0.14.7 (2017-11-03)

- Bugfixes

v0.14.6 (2017-10-31)

- New Wikipedia Python extension

v0.14.5 (2017-10-30)

- Bugfixes

v0.14.4 (2017-10-25)

- New base converter Python extension

v0.14.3 (2017-10-21)

- New Google Translate Python extension

v0.14.2 (2017-10-20)

- Bugfixes

v0.14.1 (2017-10-19)

- Bugfixes

v0.14.0 (2017-10-18)

- New Python Embedding extension
- New Python Eval Python extension
- New Debugging Python extension
- New Zeal Python extension
- New GoldenDict Python extension
- New Unit Converter Python extension (Gnu units)
- New Currency Converter Python extension (Google finance)

v0.13.1 (2017-00-30)

- Bugfixes

v0.13.0 (2017-09-28)

- Modular frontends
- QML frontend
- Tree structure in file index and "smart" indexing
- New KeyValue extension
- New Hash Generator extension
- Shell like completion for terminal extension

v0.12.0 (2017-06-09)

- Git-like ignore files
- Dedicated dialog for websearch editing.

v0.11.3 (2017-05-28)

-  Bugfixes

v0.11.2 (2017-05-13)

- <kbd>Home</kbd> and <kbd>End</kbd> now work for the results list holding <kbd>Ctrl</kbd>.

v0.11.1 (2017-04-16)

- Bugfixes

v0.11.0 (2017-04-16)

- Fine-grained control of the MIME types to be indexed.
- Extensions can now have multiple triggers.
- The sorting algorithm is now stable.
- Browse mode now lists the results in lexicographical order with directories before the files.
- The use of fallbacks has been disabled for triggered queries.
- Further the websearch extension now contains an URL handler.
- The qss property `selection-color` works as expected now.

v0.10.4 (2017-04-14)

- Bugfixes

v0.10.3 (2017-04-02)

- Terminal extension does no more show suggestions.
- Any shell querying dropped.
- Bugfixes

v0.10.2 (2017-03-24)

- Bugfixes

v0.10.1 (2017-03-20)

- Bugfixes

v0.10.0 (2017-03-19)

- Tab completion.
- Hovering over the input box the mouse wheel can now be used to browse the history.
- Spotlight themes (Bright, Dark and Space).
- [MPRIS](/docs/extensions/mpris/) extension
- [Secure Shell](/docs/extensions/ssh/) extension.
- The terminal extension now provides the shell aliases too.
- File browse mode.
- The application extension allows to ignore the `OnlyShowIn`/`NotShowIn` keys.
- The bash script to clone the template extension is now deprecated and replaced by a Python script.

v0.9.5 (2017-03-13)

- Bugfixes

v0.9.4 (2017-03-01)

- Bugfixes

v0.9.3 (2017-02-05)

- Bugfixes

v0.9.2 (2017-01-30)

- Bugfixes

v0.9.1 (2017-01-23)

- Chooseable terminal command of a list of installed terminals.
- The app icon now uses a theme icon
- _Open terminal here_ action.
- Mostly fixes

v0.9.0 (2017-01-17)

- New extension system architecture
- External extensions extension
- Firefox extension
- Improved VirtualBox extension

v0.8.11 (2016-09-29)

- Plugin abstraction architecture
- New plugin support: Native C++ QPlugins
- New plugin support: Executables
- Single click activation of items
- Action modifiers
- Multithreading
- Core is now responsible for usage counting (obsoletes most serialization)
- Asynchronous query "live" results
- Tray icon
- New option: Show tray icon
- New extension: Debug

v0.8.10 (2016-06-23)

- New extension: Virtual Box
- Basic IPC
- Graceful termination on SIGHUP

v0.8.9 (2016-05-12)

- New option: Hide on close
- New option: Display shadow
- Graceful termination on SIGINT

v0.8.8 (2016-04-28)

- Single instance

v0.8.7.3 (2016-04-27), v0.8.7.2 (2016-04-03), v0.8.7.1 (2016-03-31)

- Hotfixes

v0.8.7 (2016-03-31)

- New extension: Terminal
- Project structure: Use libraries

v0.8.6 (2016-03-28)

- Restructured settings widget

v0.8.5 (2016-03-25)

- Custom icon lookup
- Make main window movable
- Show alternative actions o Tab
- New option: Always on top
- New option: Hide on focus out
- New option: Display icons
- New option: Display scrollbar
- Dozens of new themes: Dark, Bright, SolarizedDark, SolarizedBright in several colors.

v0.8.4 (2016-03-15)

- Show message box after ungraceful termination

v0.8.3 (2016-03-13)

- Restructured settings widget

v0.8.2 (2016-03-09)

- New option: Group separators for calculator
- New themes: Arc
- New theme: Numix

v0.8.1 (2016-03-04)

- Minor tasks and improvements

v0.8.0 (2015-10-27)

- New extension: System control

v0.7.7 (2015-10-16)

- Bring settings window to front if it is already open.

v0.7.6 (2015-10-15)

- Reorderable websearches
- Allow exclusive queries by trigger

v0.7.5 (2015-10-12)

- Graceful termination on SIGINT

v0.7.1 (2015-10-06), v0.7.2 (2015-10-07), v0.7.3 (2015-10-07), v0.7.4 (2015-10-08)

- Tasks, Hotfixes, minor changes

v0.7.0 (2015-10-05)

- Implement plugin architecture
- Port the modules
- Ignore file (".albertignore")
- Actions
- Threaded background indexing
- New themes

v0.6.0 (2014-11-12)

- Make action modifications configurable
- Command history

v0.5.0 (2014-11-06)

- Add configuration widget to configure the modules
- Make user interface themable
- Provide proof-of-concept themes
- Make actions modifiable
- Show action modifications in the list
- Use CMake build system

v0.4.0 (2014-10-16)

- Add settings widget
- Implement indexing and search algorithms 'prefixmatch' and 'fuzzy'

v0.3.0 (2014-09-12)

- Implement serialization of indexes
- New module: Applications
- New module: Bookmarks
- New module: Calculator
- New module: Web search

v0.2.0 (2014-09-08)

- Abstract module architecture
- New module: Files

v0.1.0 (2014-09-01)

- Basic user interface
- Hotkeymanager
