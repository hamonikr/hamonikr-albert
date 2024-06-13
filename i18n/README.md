# Albert launcher translations

## How to add a translation file

* Get Qt Linguist.
* Clone the entire Albert repository.
* Open a terminal and `cd` into the i18n folder.
* Create a Qt translation file for your desired language, e.g. by converting an existing one: E.g. for spanish: `lconvert -drop-translations i18n/albert_de.ts -target-language es -o i18n/albert_es.ts`
* Populate the translation file(s) using the meta tool. `./metatool.py lupdate`
* Optionally pass the file to GPT, which does a decent job at translating ts files.
* Open the file using Qt Linguist and review/translate.
* Save, commit, push and send a PR.

## how to update to ts file
cd ..
lupdate `find src -name '*.cpp' -o -name '*.h' -o -name '*.ui'` -ts i18n/albert_ko.ts
