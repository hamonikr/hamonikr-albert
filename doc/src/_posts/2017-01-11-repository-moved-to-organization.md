---
layout: page
title:  "Repository moved to organization page"
date: 2017-01-11 20:09 +0100
---

Finally albert moved from my [user page](https://github.com/manuelschneid3r) to the [albert organization page](https://github.com/albertlauncher). The repository url is now [https://github.com/albertlauncher/albert.git](https://github.com/albertlauncher/albert.git). This has some implications for developers who cloned the repository: The git remote urls are now invalid. Fix them by using the git commandline (or your git tool of choice):

```bash
git remote set-url <name> https://github.com/albertlauncher/albert
```
