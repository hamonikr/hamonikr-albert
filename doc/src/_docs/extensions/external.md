---
layout: docs
title: External extension
permalink: /docs/extensions/external/
---

***Note: This scripting extension is deprecated as of v0.14.0. Use the python extension instead***


Albert can be extended using regular executables. They are used like plugins, however the executables are separate processes which have separate address spaces. Therefore these executables are called _external plugins_. This documentation refers to the extension shipped with v0.13.

An external plugin (hereafter plugin) is basically an executable which has a particular interface. Imagine the plugin as a set of functions that can be dynamically invoked. Which function is invoked, is defined by the environment variable `$ALBERT_OP`. The plugin should read this environment variable and react accordingly. Additionally to the mandatory environment variable `$ALBERT_OP` there may be some other variables defined. Imagine those as the parameters to the dynamic function. The return value of this function is returned to the application through the standard output stream (stdout). Depending on `$ALBERT_OP` the expected data may differ, but all responses have to be a JSON object containing properties. The set of possible `$ALBERT_OP`s and the expected properties and their structures are defined by the [*Communication protocol v3*](/docs/extensions/external/v3/) (Since v0.13, [*v2*](/docs/extensions/external/v2/) until 0.13).

To save state between the executions the plugin can return a JSON object called "variables". The properties of the object "variables" will be set as environment variables in the next execution. Note that this properties have to be strings otherwise they will not be set in the environment.

## Deployment

> Note that the exteral extensions have to be executable to be used by albert

The extension check its data directories for a directory called `extensions`. The name of a data directory is the id of the extension. I the case of the external extension this is `org.albert.extension.externalextensions`. The data directories reside in the data directories of the application defined by [Qt](http://doc.qt.io/qt-5/qstandardpaths.html#StandardLocation-enum). Hence the external extensions would be looked up in the following directories (in this order):

* ~/.local/share/albert/org.albert.extension.externalextensions/extensions
* /usr/local/share/albert/org.albert.extension.externalextensions/extensions
* /usr/share/albert/org.albert.extension.externalextensions/extensions

Ids are guaranteed to be unique. This means that if several of those path contain a plugins with identical ids, only the first found plugin will be used.
