# Output format

```
html-to-rc --in=www --file-out=Web.Source.rc
```

with

```
www/
    about.html
    index.html
    logo.png
    css/site.css
    js/app.js
    js/lib/l.js
```

writes:

```rc
about.html HTML "www\\about.html"
index.html HTML "www\\index.html"
logo.png HTML "www\\logo.png"
css/site.css HTML "www\\css\\site.css"
js/app.js HTML "www\\js\\app.js"
js/lib/l.js HTML "www\\js\\lib\\l.js"
```

- One line per file: `<name> HTML "<path>"`, ending with `\r\n`.
- `<name>` is the file's path inside the input folder, with `/` between
  the parts (`css/site.css`), whatever separator the input used.
- `HTML` is the resource type `RT_HTML` (23).
- `<path>` is the path of the file as the tool saw it: the `--in` value
  plus the path inside it, with every `/` turned into `\` and every `\`
  doubled, as a C string in the `.rc` wants.
- Order: the files of a folder first (as the file system lists them,
  alphabetical on NTFS), then each subfolder, depth first.
- Every file gets a line, whatever its extension: HTML, CSS, JavaScript,
  images, fonts, JSON, ... The type is `HTML` for all of them.
- No header, no comment, no `#include`. An input that does not exist, or
  a single file instead of a folder, gives an **empty** output.

## The data

The file's bytes are **not** written into the `.rc`: `rc.exe` opens the
file at `<path>` when it compiles the `.rc` and stores its exact content
(no conversion, no terminator, no padding; `SizeofResource` is the file
size). This keeps the `.rc` small whatever the size of the files, but the
files must exist and be found at compile time.

To put the bytes inside the `.rc` itself (a self-contained script), use
[file-to-rc](https://github.com/g-stefan/file-to-rc), one file at a time.

## File paths

The paths are written relative to the directory the tool ran in (or
absolute, if `--in` was absolute). `rc.exe` finds a relative path:

1. in the current directory of `rc.exe`;
2. in the directory of the `.rc` file given to `rc.exe` (the top level one,
   **not** the one that is `#include`d);
3. in the include directories (`/i`).

Nowhere else: not next to the generated file when it is `#include`d from
another folder.

fabricare runs `rc.exe` from the repository root with `/i source`, and
gives it each `*.rc` of the project's source folder. So:

| Tool run in | `--in` | Path written | Found by `rc.exe` because |
|-------------|--------|--------------|---------------------------|
| `source/XYO/MyApp` (`runInPath`) | `www` | `www\\index.html` | it is relative to `source/XYO/MyApp/Application.rc` (rule 2) |
| repository root | `source/XYO/MyApp/www` | `source\\XYO\\MyApp\\www\\index.html` | it is relative to the current directory (rule 1) |
| `source/XYO/MyApp` | `www`, but the `.rc` that includes it is elsewhere | `www\\index.html` | **not found**: `error RC2135 : file not found: www\index.html` |

Absolute paths always work, but tie the generated file to one machine; do
not commit an output made from an absolute `--in`.

An `--in` that ends with a separator doubles it in every path
(`www/` → `"www\\\\index.html"`); Windows accepts it, but leave the
separator out.

## Resource names

The name is written without quotes, so `rc.exe` decides what it is:

| File in the folder | Resource | Notes |
|--------------------|----------|-------|
| `index.html` | the string `INDEX.HTML` | string names are stored upper case; lookup ignores case |
| `css/site.css` | the string `CSS/SITE.CSS` | `/` is kept |
| `404.html`, `a-b.min.js`, `a,b.html` | `404.HTML`, `A-B.MIN.JS`, `A,B.HTML` | accepted as strings |
| `123` (only digits) | the numeric id 123 | find it with `MAKEINTRESOURCEA(123)` |
| `my page.html` (a space) | `rc` error (`file not found: HTML`) | rename the file |
| `über.html` (not ASCII) | `rc` error (`file not found`) | the output is UTF-8, `rc.exe` reads the `.rc` in the ANSI code page; keep names ASCII |

Each name must be unique in the final executable.

## Reading the resources at run time

The type is `RT_HTML` (`MAKEINTRESOURCE(23)`). The data stays in the
mapped image: no copy, no free, valid while the module is loaded.

```cpp
#include <windows.h>

bool getHTMLResource(HMODULE module, const char *name, const void *&data, size_t &size) {
	HRSRC resource = FindResourceA(module, name, RT_HTML);
	if (resource == nullptr) {
		return false;
	};
	HGLOBAL handle = LoadResource(module, resource);
	if (handle == nullptr) {
		return false;
	};
	data = LockResource(handle);
	size = SizeofResource(module, resource);
	return data != nullptr;
};

const void *data;
size_t size;
if (getHTMLResource(nullptr, "css/site.css", data, size)) {
	// size is the exact file size
};
```

- `name` is the path inside the folder with `/`, any case:
  `"css/site.css"` and `"CSS/SITE.CSS"` both work. A leading `/`
  (`"/css/site.css"`) or a `\` (`"css\\site.css"`) do **not**: when the
  name comes from a URL path, strip the leading `/`.
- `module = nullptr` is the `.exe`. For resources compiled into a DLL,
  pass the DLL's `HMODULE` (`GetModuleHandleA("web.dll")`, the `hinstDLL`
  of `DllMain`, or `LoadLibraryExA(file, nullptr, LOAD_LIBRARY_AS_DATAFILE)`
  for a resource-only DLL).
- The data is **not** `0x00` terminated. To use it as a C string, copy it
  (`String text; text.set(static_cast<const char *>(data), size);` with
  `xyo-encoding`'s `String`).

This is the way to serve the files to an embedded web server, a WebView2
`WebResourceRequested` handler, or any other code that maps a URL path to
the content: the URL path, without its leading `/`, is the resource name.

## The res protocol

The WebBrowser control (MSHTML, the Internet Explorer engine) reads `HTML`
resources by URL:

```
res://C:\path\MyApp.exe/index.html
res://web.dll/index.html
```

A page loaded this way resolves its relative links against the same
module, so `index.html` can use `<link href="site.css">` and
`<script src="app.js">`. This works **only for a flat folder**: `res://`
reads the part after the module as `[type/]name`, so
`res://MyApp.exe/css/site.css` is looked up as type `CSS`, name
`SITE.CSS`, and is not found. Checked on Windows 11 with the
WebBrowser control: in a flat folder the page's CSS and script loaded;
with `css/site.css` and `js/app.js` in subfolders neither did, although
`FindResource` finds both names.

For `res://`, keep every file at the top of `--in`; for subfolders, read
the resources with `FindResource` as above.

## Appending

With `--append` (library: `append = true`) the lines of the next folder
are written after the previous ones:

```
html-to-rc --in=www --file-out=Web.Source.rc
html-to-rc --in=www/css --file-out=Web.Source.rc --append
```

```rc
about.html HTML "www\\about.html"
index.html HTML "www\\index.html"
logo.png HTML "www\\logo.png"
css/site.css HTML "www\\css\\site.css"
js/app.js HTML "www\\js\\app.js"
js/lib/l.js HTML "www\\js\\lib\\l.js"
site.css HTML "www\\css\\site.css"
```

The names are relative to each input folder: here `site.css` and
`css/site.css` are two resources with the same data.

## Pitfalls

| Situation | What happens | Do this |
|-----------|--------------|---------|
| `--in` is a single file | empty output, exit `0`, no message | give the folder, or write the line by hand, or use `file-to-rc` |
| `--in` does not exist (a typo) | empty output, exit `0`, no message | check the path; an empty output means nothing was found |
| the generated `.rc` is included by a `.rc` in another folder than where the tool ran | `RC2135 : file not found` | run the tool in the folder of the including `.rc`, or from the directory `rc.exe` runs in |
| the output is inside the input folder | the output lists itself (it is created before the folder is read) | write it next to the folder, not in it |
| the generated `.rc` is in the source folder and named `*.rc` | fabricare compiles it on its own **and** through the `#include`: duplicate resources, `CVT1100` at link | name it `*.Source.rc` |
| `--append` run again on the next build | the output grows, duplicate resources, `CVT1100` | start every sequence with one call without `--append` |
| two folders appended that share file names (`index.html`) | duplicate resource, `CVT1100` | one folder, or the library with `basePath = ""` to keep the folder names |
| a file removed or renamed, with `--touch` | the output is not regenerated, `rc` fails with `file not found` | delete the output (or `fabricare clean`) after removing files |
| a file or folder name with a space, or not ASCII | `rc` error | rename it |
| `res://` to a file in a subfolder | not found | flat folder for `res://`, or `FindResource` |
| looking up `"/index.html"` | not found | strip the leading `/` |
| treating the data as a C string | reads past the end, no terminator | copy with the size from `SizeofResource` |
| an output made from an absolute `--in` committed | breaks on any other machine | relative `--in` |
| Linux / Emscripten | resources do not exist there | use [file-to-cs](https://github.com/g-stefan/file-to-cs) to embed data portably |
