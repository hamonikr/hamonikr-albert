---
layout: docs
title: External extensions communication protocol (v2)
permalink: /docs/extensions/external/v2/
---

#### `METADATA`
The application wants to get the metadata of the extension. It should have the
following keys:

* `iid` (string, mandatory)
* `version` (string, defaults to 'N/A')
* `name` (string, defaults to $id)
* `trigger` (string, defaults to 'empty')
* `author` (string, defaults to 'N/A')
* `dependencies` (array of strings, defaults to 'empty')

The interface id `iid` (currently `org.albert.extension.external/v2.0` tells the application the type and version of the communication protocol . If the `iid` is incompatible this plugin will not show up in the plugins list. The remaining keys should be self-explanatory. Errors in this step are fatal: loading will not be continued.

#### `INITIALIZE`
The request to initialize the plugin. The plugin should load potential state from persistant storage, check if all requirements are met and set the exit code accordingly. (Everything but zero is an error). Errors in this step are fatal: loading will not be continued.

#### `FINALIZE`
The request to finalize the plugin. The plugin should save the state to persistent storage.

#### `SETUPSESSION`
The request to setup for a session, meaning prepare for user queries. This message is sent when the users starts a session, i.e.  the main window opens.

#### `TEARDOWNSESSION`
The request to teardown a session. This message is sent when the users ends a session, i.e. the main window closes.

#### `QUERY`
The request to handle a query. The environment variable `ALBERT_QUERY` contains the _complete_ query as the user entered it into the input box, i.e. including potential triggers.

**Note:** The process handling `QUERY` can be terminated at any time. Do _not_ change state in this code segment.

Return the results by an array "items" containing JSON objects representing the results. A result object has to contain the following entries: `id`, `name`, `description`, `icon` and `actions`.

  - `id` is the plugin wide unique id of the result
  - `name` is the name of the result
  - `description` is the description of the result
  - `completion` is the completions string of the result
  - `icon` is the icon of the result (name or path)
  - `actions` is a array of objects representing the actions for the item.

The `id` of the item will be used to sort the items by usage. The `name`, `icon` and `description` will be displayed together as an item in the results list. `completion` is the string that will be used to replace the text in the input box when the user pressed <kbd>Tab</kbd>. If `icon` is an absolute path, the given file will be used. If the `icon` is not an absolute path, the algorithm described in the [Icon Theme Specification](https://freedesktop.org/wiki/Specifications/icon-theme-spec/) will be used to locate the icon. An object representing an action has to contain the following values: `name`, `command` and `arguments`.

- `name` is the actions name
- `command` is the program to be execute
- `arguments` is an array of parameters for `command`

An example:
```json
{
 "items": [{
   "id":"extension.wide.unique.id",
   "name":"An Item",
   "description":"Nice description.",
   "icon":"/path/to/icon",
   "actions":[{
     "name":"Action name 1",
     "command":"program",
     "arguments":["-a", "-b"]
   },{
     "name":"Action name 2",
     "command":"program2",
     "arguments":["-C", "-D"]
   }]
 }],
 "variables": {
   "some_var":"variable",
   "some_other_var":"cool state"
 }
}
```
