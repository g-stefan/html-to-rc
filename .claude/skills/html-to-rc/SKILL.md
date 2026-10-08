---
name: html-to-rc
description: >-
  How to use html-to-rc, the XYO tool and C++ library (namespace
  XYO::HTMLToRC) on top of xyo-system that converts a folder of web files
  (HTML, CSS, JS, images) into Windows resource script source, one line
  per file, name HTML "path" (RT_HTML, 23), the name being the file's path
  inside the folder (css/site.css), so rc.exe compiles the whole tree into
  an .exe / .dll, read back with FindResource(module, "css/site.css",
  RT_HTML) / LoadResource / LockResource / SizeofResource or shown in a
  WebBrowser control through res:// (flat folders only): the html-to-rc
  command line (--in folder or * / ? pattern, never a single file,
  --file-out, --append, --touch recursive, @response files, exit codes,
  silent empty output for a missing input); the output format (\r\n
  lines, upper case names, paths relative to where the tool ran and how
  rc.exe resolves them, no spaces / non ASCII names); the function
  htmlToRC with basePath; embedding the tool through
  html-to-rc.application.static and XYO::HTMLToRC::Application::Application
  (fabricare's htmlToRC(...)); the *.Source.rc + #include + --touch build
  pattern. Use when writing or reviewing code that includes
  <XYO/HTMLToRC.hpp> or <XYO/HTMLToRC.Application.hpp>, depends on
  "html-to-rc" in fabricare.json, calls html-to-rc or htmlToRC in a build
  script, #includes a generated *.Source.rc with HTML resources, wants to
  embed a web user interface / help pages / static site in a Windows
  program, uses RT_HTML or res://, or when working inside the html-to-rc
  repository.
---

# html-to-rc

Turns a folder tree of web files into Windows resource script source so
the resource compiler puts every file into the program as an `HTML`
resource named by its path. Two faces:

- the **`html-to-rc` command line tool**, run from build scripts (in
  fabricare scripts also as the built-in `htmlToRC(...)`);
- the **C++ library** `XYO::HTMLToRC`: `htmlToRC`, plus the tool as a
  class.

Built on `xyo-system` (see the `xyo-system` skill and the layers below it:
their rules apply). Siblings: `file-to-rc` (one file, bytes inline as
`RCDATA`; skill `file-to-rc`) and `file-to-cs` (one file as a C/C++ array,
portable; skill `file-to-cs`).

Full documentation: `docs/` in the html-to-rc repository
(`X:\Storage\XYO\Gitea\CPP\html-to-rc\docs` on this machine): README,
getting-started, command-line, **output-format**, library, reference.
The whole implementation is `source/XYO/HTMLToRC/Library.cpp` and
`source/XYO/HTMLToRC.Application/Application.cpp`.

## Output

```
html-to-rc --in=www --file-out=Web.Source.rc
```

```rc
about.html HTML "www\\about.html"
index.html HTML "www\\index.html"
logo.png HTML "www\\logo.png"
css/site.css HTML "www\\css\\site.css"
js/app.js HTML "www\\js\\app.js"
js/lib/l.js HTML "www\\js\\lib\\l.js"
```

One `name HTML "path"\r\n` line per file: files of a folder first
(alphabetical on NTFS), then subfolders depth first. Name = path inside
`--in` with `/`; path = as the tool saw it, `/` → `\`, `\` doubled. Every
extension gets the type `HTML`. The bytes are **not** in the `.rc`:
`rc.exe` reads each file at compile time and stores it exactly (no
terminator, no padding).

## Pick an approach

| Need | Do |
|------|----|
| A web UI / help folder in a Windows program | `html-to-rc --in=www --file-out=Web.Source.rc` |
| Only some top level files | `"--in=www/*.html"` (subfolders only if their own name matches, then all of their files) |
| One file | write the line by hand (`index.html HTML "index.html"`) or use `file-to-rc` |
| The `.rc` must contain the bytes (self-contained) | `file-to-rc`, one file at a time |
| Several folders, names must keep the folder (`help/index.html`) | library: `htmlToRC("help", out, true, "")` |
| Regenerate only if something changed | `--touch=Application.rc` |
| Show pages in a WebBrowser control | `res://MyApp.exe/index.html`, **flat folder only** |
| Serve the files from a web server / WebView2 handler | `FindResourceA(module, urlPathWithoutLeadingSlash, RT_HTML)` |
| Data on Linux or in portable code | `file-to-cs` instead (resources are Windows only) |

## The standard build pattern

```
source/XYO/MyApp/Application.rc
source/XYO/MyApp/www/index.html, site.css, app.js
```

`fabricare/make.prepare.js`:

```js
messageAction("make.prepare");

runInPath("source/XYO/MyApp", function() {
	exitIf(htmlToRC("--touch=Application.rc", "--in=www", "--file-out=Web.Source.rc"));
});
```

`source/XYO/MyApp/Application.rc`:

```rc
#include <XYO/MyApp/Web.Source.rc>
```

Run time (Windows):

```cpp
HRSRC resource = FindResourceA(nullptr, "css/site.css", RT_HTML); // any case, no leading '/'
HGLOBAL handle = LoadResource(nullptr, resource);
const void *data = LockResource(handle);        // not 0x00 terminated, no free needed
DWORD size = SizeofResource(nullptr, resource);  // exact file size
```

For a DLL pass its `HMODULE` instead of `nullptr`.

## Hard rules

1. **`--in` is a folder or a pattern, never a single file.** Without `*`
   / `?` the tool appends `/*`; `--in=www/index.html` lists nothing.
   A single file **or a missing folder** → the output is created
   **empty** and the exit code is `0`, no message. An empty generated
   file means the input was wrong.
2. **Required options**: `--in`, `--file-out`. Missing → usage printed,
   exit `1`. Empty value (`--in=`) → `Error: ... is empty`, exit `1`.
   Output not writable (missing folder, no access) → exit `1` with **no
   message**; `htmlToRC` returns `false` only in that case. Always
   `exitIf(...)`. The output folder is never created.
3. **Paths are relative to where the tool ran.** `rc.exe` finds a
   relative path only from its current directory, the directory of the
   top level `.rc` it compiles (not of an `#include`d one), or `/i`
   paths. fabricare runs `rc` from the repository root with `/i source`.
   So run the tool (`runInPath`) in the folder of the `.rc` that
   `#include`s the output, with `--in` relative to it. Otherwise:
   `error RC2135 : file not found`. Never commit output made from an
   absolute `--in`. Do not end `--in` with `/` (it doubles separators).
4. **Names come from file names**, unquoted: stored upper case (lookup
   ignores case), `/` kept; only digits → numeric id. A **space** or a
   **non ASCII** character in a file or folder name breaks `rc.exe`
   (the output is UTF-8, `rc` reads ANSI): keep names ASCII, no spaces.
5. **Lookup** with `FindResourceA(module, "css/site.css", RT_HTML)`. A
   leading `/` or a `\` is not found: strip the `/` of URL paths.
6. **`res://` only for a flat folder.** `res://module/css/site.css` is
   parsed as type `CSS`, name `SITE.CSS`: not found. Checked on Windows 11
   with the WebBrowser control: in a flat folder the page's relative CSS and
   script loaded, in subfolders they did not.
7. **Name generated files `*.Source.rc`.** fabricare compiles every
   `*.rc` in the source path except names containing `.Source.`; a
   generated `X.rc` that is also `#include`d would be compiled twice →
   `CVTRES : fatal error CVT1100: duplicate resource` at link.
8. **Never put the output inside `--in`**: it is opened before the folder
   is read, so it lists itself.
9. **`--touch=f`**: if the output exists and **no file of the tree**
   (any depth) is newer, exit `0` and do nothing (not even touch).
   Otherwise write, then touch `f` only if it exists. It does **not**
   notice removed or renamed files (then `rc` fails with `file not found`):
   delete the output or `fabricare clean`.
10. **`--append` accumulates**: every run adds again. Start each sequence
    with one call without `--append`. Names are relative to each `--in`,
    so two folders with `index.html` collide (`CVT1100`); keep the folder
    in the name with the library (`basePath = ""`). Do not mix `--append`
    with `--touch` on a shared output.
11. **The data is not a C string**: no terminator; use the size from
    `SizeofResource`. It stays valid while the module is loaded.
12. **Generated files** (`*.Source.rc`) are outputs: never edit them, edit
    the folder and rebuild.
13. **`@file`** response files are expanded first (split like a command
    line, quotes group words); a missing one → `Error: file not found -
    file`, exit `1`. Unknown options and non `--` arguments are ignored.

## Depend on it

```json
"dependency": [ "html-to-rc" ]                    // DLL
"dependency": [ "html-to-rc.static" ]             // static, defines XYO_HTMLTORC_LIBRARY
"dependency": [ "html-to-rc.application.static" ] // embed the tool, no main
```

```cpp
#include <XYO/HTMLToRC.hpp>

// htmlToRC(pathOrFileNameIn, fileNameOut, append, basePath = nullptr)
// basePath nullptr: names relative to the folder; "": names keep the full path as found;
// otherwise a prefix of pathOrFileNameIn ending with a separator ("site/" for "site/www").
if (!XYO::HTMLToRC::htmlToRC("www", "Web.Source.rc", false)) {
	printf("* Error: Web.Source.rc\n");
	return 1;
};
XYO::HTMLToRC::htmlToRC("help", "Web.Source.rc", true, ""); // help/index.html ...
```

`--touch` in code: `if (Shell::isChangedRecursive(out, in)) { htmlToRC(in,
out, false); Shell::touchIfExists(includingRc); }`.

Embedded tool (as fabricare's `htmlToRC(...)`): build a plain `char *`
array (`cmdS[0]` = program name, `TDynamicArray` is not contiguous) and
call `XYO::HTMLToRC::Application::Application application;
application.main(cmdN, cmdS)`: it returns the exit code and prints to
stdout. The command line always uses `basePath = nullptr`.

Metadata: `XYO::HTMLToRC::Version::version()` from
`<XYO/HTMLToRC/Version.hpp>` (not included by `<XYO/HTMLToRC.hpp>`); the
tool's is `XYO::HTMLToRC::Application::Version::version()`.

## Working inside this repository

- Four projects in `fabricare.json`: `html-to-rc` (DLL, version key
  `html-to-rc.library`), `html-to-rc.static`,
  `html-to-rc.application.static`, `html-to-rc` (exe).
- `fabricare make`, `fabricare install`. There is no test project; try
  changes with the built `output/bin/html-to-rc` on a sample tree
  (files at the top, nested subfolders, a wildcard, a missing folder),
  compile the result with `rc.exe` and link it (`link /DLL /NOENTRY
  x.res` makes a resource-only DLL) to catch `RC2135` and `CVT1100`.
  In a plain shell `cl.exe` / `rc.exe` / `link.exe` are missing: run after
  `vcvars64.bat`.
- fabricare bootstraps this tool without itself
  (`build/fabricare.compile.json` in the fabricare repository compiles
  `HTMLToRC.Application.Amalgam.cpp`): keep it self-contained.
- Keep the docs in `docs/` and this skill in step with `Library.hpp` /
  `Application.cpp` when behavior changes (the output samples here and
  in `docs/output-format.md` are real tool output).
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); build scripts, config,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with `reuse lint`).
- Files use CRLF line endings; edit them with tools that keep it (`sed -i`
  in Git Bash rewrites them as LF).
