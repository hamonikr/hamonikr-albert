---
layout: docs
title: Python extension
permalink: /docs/extensions/python/
---

The Python extension makes the application extendable by embedding Python modules. Since the name of the native extension providing this functionality is "Python extension" and a Python module in this context is called "Python extension" too, this article refers to the Python extensions by using the term *Python modules*.

A Python module must have a specific interface to be applicable as extension. The interface uses classes which are defined in the builtin `albertv0` module. Check the relevant sections below.

The extension interface and the built-in albert module are _not_ final. They are prototypes and intended to be improved on user feedback.

The extension interface is a versioned protocol that describes how the Python extension interacts with the Python modules. The current interface identifier is **PythonInterface/v0.2**.


## Extension interface (0.2)

Additions to PythonInterface/0.1:

##### `cacheLocation()`

Returns the writable cache location of the app. (E.g. $HOME/.cache/albert/ on Linux)

##### `configLocation()`

Returns the writable config location of the app. (E.g. $HOME/.config/albert/ on Linux)

##### `dataLocation()`

Returns the writable data location of the app. (E.g. $HOME/.local/share/albert/ on Linux)

## Extension interface (0.1)

##### `handleQuery(Query)`
Mandatory function. This is the crucial part of a Python module. When the user types a query, this function is called with a query object representing the current query execution.

This function should return a list of Item objects. See the Item class section below.

##### `initialize()`
Optional function. This function is called when the extension is loaded.

##### `finalize()`
Optional function. This function is called when the extension is unloaded.

##### The docstring of the module (`__doc__`)
The docstring of the module is used as description of the extension. This string will be displayed to the user.

##### `__iid__`
Mandatory variable (string). This variable has to hold the interface version the extension implements.

##### `__prettyname__`
Optional variable (string). This variable should hold the pretty name of the extension. This string will be displayed to the user.

##### `__version__`
Optional variable (string). This variable should hold the version of the extension. This string will be displayed to the user. The versioning scheme should follow the [semantic versioning](http://semver.org).

##### `__trigger__`
Optional variable (string). If this extension should be run exclusively, this variable has to hold the trigger that causes the extension to be executed.

##### `__author__`
Optional variable (string). This variable should hold the name of the author of the extension.

##### `__dependencies__`
Optional variable (list of strings). This string should contain any dependencies the extension needs to be used.


## Api reference - The `albertv0` module

The built-in albert module exposes several functions and classes for use with Albert.

### The `Query` class

The query class represents a query execution. It holds the necessary information to handle a Query. It is passed to the handleQuery function. It holds the following read-only properties.

##### `string`
This is the actual query string without the trigger. If the query is not triggered this string equals rawstring.

##### `rawString`
This is the full query string including the trigger. If the query is not triggered this string equals string.

##### `trigger`
This is the trigger that has been used to start this extension.

##### `isTriggered`
Indicates that this query has been triggered.

##### `isValid`
This flag indicates if the query is valid. A query is valid as long as the core cancels it. You should regularly check this flag and abort the query handling if the flag is False to release threads in the threadpool for the next query.

### Abstract classes

##### The `ActionBase` class

The base class for actions. This is a wrapper for the internal Action interface. You should not need it. If you think you do I‘d be interested why. Please contact me.

##### The `ItemBase` class

The base class for items. This is a wrapper for the internal Item interface. You should not need this unless you need the Urgency enum.

###### The `Urgency` enum
The urgency of an item. Note that this enum is defined in the scope of the ItemBase class.

### Standard action classes

Standard actions are realizations of the Action interface from the core. They implement often used actions for Albert items.

##### The `ClipAction` class
This class copies the given text to the clipboard on activation.

##### `UrlAction`
This class opens the given URL with the systems default URL handler for the scheme of the URL on activation.

##### `ProcAction`
This class executes the given commandline as a detached process on activation. Optionally the working directory of the process can be set.

##### `TermAction`
This class executes the given commandline in the terminal set in the preferences. Optionally the working directory of the process can be set.

##### The `FuncAction` class
This class is a general purpose action. On activation the callable is executed.

### The `Item` class

This class represents a result item. Objects of this class are intended to be returned by the handleQuery function.  

##### `Item(id="", icon=":python_module", text="", subtext="", completion="", urgency=Urgency::Normal, actions=[])`
This is the constructor of the item.

Note that the default icon path is ":python_module" which is an embedded resource icon depicting a Python script and the urgency defaults to normal.

##### `id`
The identifier string of the item. It is used for ranking algorithms and should not be empty.

##### `icon`
The path of the icon displayed in the item.

##### `text`
The primary text of the item.

##### `subtext`
The secondary text of the item. This text should have informative character.

##### `completion`
The completion string of the item. This string will be used to replace the input line when the user hits the Tab key on an item.

Note that the semantics may vary depending in the context. Often evaluate semantics are more appropriate.

##### `urgency`
The urgency of the item. Note that the Urgency enum is defined in the ItemBase class. See the Urgency enum.

##### `addAction(Action)`
Add an action to the item.

### Free functions

##### `debug(text)`
Log a debug message. Note that this is effectively a NOP in release builds.

##### `info(text)`
Log an info message.

##### `warning(text)`
Log a warning message.

##### `critical(text)`
Log a critical message.

##### `iconLookup(iconName)`
Perform xdg icon lookup and return a path. Empty if nothing found.

## Deployment

The extension checks its data directories for a directory called `modules`. The name of a data directory is the id of the extension. I the case of the Python extension this is `org.albert.extension.python`. The data directories reside in the data directories of the application defined by [Qt](http://doc.qt.io/qt-5/qstandardpaths.html#StandardLocation-enum). Hence the external extensions would be looked up in the following directories (in this order):

* ~/.local/share/albert/org.albert.extension.python/modules
* /usr/local/share/albert/org.albert.extension.python/modules
* /usr/share/albert/org.albert.extension.python/modules

Ids are guaranteed to be unique. This means that if several of those paths contain a plugins with identical ids, the first plugin found will be used.

# Known issues

Python exceptions thrown at the core application are not handled well and may introduce a lot of weird errors. Ensure that you catch _all_ exceptions in the Python code.

The Python interpreter shuts down when the Python extension is unloaded. After this, enabling the extension will restart the interpreter. Some modules cannot be re-initialized safely and may cause segfaults after the interpreter has been restarted (numpy!). The issue is that Python itself cannot completely unload extension modules and there are several caveats with regard to interpreter restarting. In short, not all memory may be freed, either due to Python reference cycles or user-created global data. All the details can be found in the CPython documentation.
