---
layout: docs
title: Using Albert
permalink: /docs/using/
---

To get up and running just run `albert` in terminal and press the hotkey (which is configured upon install) to start using it. When albert is running you can configure or quit the application via the tray icon. To make albert autostart checkout [Troubleshooting](/docs/faq).

As you would expect from a launcher, the main use case is to type a query into an input box and finally to interact with some sort of results. Actually there is not much more Albert allows you to do - triggering things. No more, no less.

By pressing the hotkey the launcher is shown and the focused input line awaits a query. If you press <kbd>Esc</kbd> or the hotkey again or if the window looses focus, the window hides. The query entered in the input box is delegated to a set of plugins, which in turn return items that are somehow related to the query.

While typing a query Albert instantly shows the results in a vertical list below the input box. These results can be browsed like a regular list view. With the keys <kbd>⬆</kbd> and <kbd>⬇</kbd> the selection can be moved a single item, <kbd>PgUp</kbd> and <kbd>PgDn</kbd> moves the selection by the amount of visible items, <kbd>Ctrl</kbd>+<kbd>Home</kbd> and <kbd>Ctrl</kbd>+<kbd>End</kbd> moves the selection to the first and last item respectively. You have to hold <kbd>Ctrl</kbd> since the input line has precedence. If you found the item you are looking for, you can run its associated action by activating the item with <kbd>⏎</kbd>, <kbd>Enter</kbd> or a mouse click.

If a query yielded no results, several fallback items are provided. Fallback items are items that can handle every query, e.g. an item that opens an external search engine like google in your browser or your local dictionary application.

Items can have multiple alternative actions. By holding <kbd>Alt</kbd> a list containing the alternative actions shows up. As long as this list is visible the arrow up and down keys move the cursor in this list. Release <kbd>Alt</kbd> to return to the result items - the action list disappears.

Holding <kbd>Meta</kbd> can be used as a shortcut for the default fallback action. Holding <kbd>Meta</kbd> while activating an item runs the 1<sup>st</sup> fallback item with the entered query.

Albert stores the input query when you activate an item. This query history entries are unique and sorted chronologically. If the inputline contains text, this text serves as search term for a substring based search on the history. Note that an empty string matches all history entries. You can navigate the history search results using <kbd>Ctrl</kbd>+<kbd>⬆</kbd> and <kbd>Ctrl</kbd>+<kbd>⬇</kbd>. For convenience <kbd>⬆</kbd> behaves like <kbd>Ctrl</kbd>+<kbd>⬆</kbd> when the first item in the results is selected.

In the input box a little gear is spinning. If you left click on this gear, the settings dialog will appear. Alternatively you can simply hit <kbd>Ctrl</kbd> + <kbd>,</kbd> or <kbd>Alt</kbd> + <kbd>,</kbd> to open the settings dialog. A right click opens a context menu offering some actions including the
"Quit" action. Clicking this item will quit the application. Alternatively you
can quit the application by hitting <kbd>Alt</kbd> + <kbd>F4</kbd>.

For reference the following table lists all keys you can use to control Albert:

Key  | Action
------------- | -------------
<kbd>Esc</kbd> | Hide Albert
<kbd>Alt</kbd>  | Hold to show alternative actions for the item
<kbd>Ctrl</kbd> | Hold to show item shortcuts
<kbd>⏎</kbd>,<kbd>Enter</kbd> | Activate the primary action of selected item
<kbd>Meta/Win</kbd>+<kbd>⏎</kbd>,<br><kbd>Meta/Win</kbd>+<kbd>Enter</kbd> | Activate the 1<sup>st</sup> fallback item with the query
<kbd>Ctrl</kbd>+<kbd>,</kbd><br><kbd>Alt</kbd>+<kbd>,</kbd> | Show the settings dialog
<kbd>⬆</kbd>,<kbd>⬇</kbd>,<br><kbd>PgUp</kbd>,<kbd>PgDn</kbd>,<br><kbd>Ctrl</kbd>+<kbd>Home</kbd>,<br><kbd>Ctrl</kbd>+<kbd>End</kbd> | Navigation in the results list. If the first item is selected, <kbd>⬆</kbd> behaves like <kbd>Ctrl</kbd>+<kbd>⬆</kbd>.
<kbd>Ctrl</kbd>+<kbd>⬆</kbd><br><kbd>Ctrl</kbd>+<kbd>⬇</kbd> | Navigate the query history substring search results
<kbd>Ctrl</kbd>+<kbd>1-9</kbd> | Shortcut for item activation
<kbd>Alt</kbd>+<kbd>F4</kbd> | Quit Albert
