# NordVPN-GUI Translations

This directory contains translation files (`.ts`).
The _Qt Linguist_ toolset that is part of the Qt installation can be used to edit these files.

**How to add a new translation**

1. Add `<lang>.ts` to `CMakeLists.txt` where `<lang>` is the language code of the new translation, preferably a 2 letter code.
   There is a variable `TS_FILES` which is specifically for this purpose.
2. `CMakeLists.txt` contains a special `lupdate` target which executes the Qt Linguist command line tool with the same name.
   Run it in your build folder:
   ```sh
   # with ninja
   ninja update
   # with make
   make lupdate
   # ... or with your build system of choice
   ```
   `lupdate` will search for all strings that are marked as translations (e.g. with `qsTr()`) and update existing or create missing `<lang>.ts` files in this directory.
3. Open the generated `<lang>.ts` file with Qt Linguist.
4. Translate all strings and save your changes.
5. Add a `<lang>.qm` to `translations.qrc.in`. Note the extension: `.qm` instead of `.ts`, because those are the files used at runtime.
   They are generated by CMake. The function `qt5_add_translations()` calls the command line tool `lrelease` which compiles the `XML`-based `.ts` files into binary `.qm` files.
6. Add `<lang>.ts` to source control and commit.

**How to update translations**

Run the CMake target `lupdate` (see above) to update the `.ts` files in this directory. The open the `.ts` file you want to change with Qt Linguist, make your changes, save and commit them.