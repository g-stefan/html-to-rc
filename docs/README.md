# HTML to RC — Documentation

`html-to-rc` turns a **folder of web files** (HTML, CSS, JavaScript,
images, ...) into **Windows resource script (`.rc`) source**: one `HTML`
resource line per file, named by the file's path inside the folder. The
resource compiler then puts every file of the folder into the `.exe` /
`.dll`, and the program reads them back at run time with `FindResource` /
`LoadResource`, or shows them through the `res://` protocol of the
WebBrowser control (MSHTML). It is both:

- **a command line tool**, `html-to-rc`, run from build scripts;
- **a C++ library**, namespace `XYO::HTMLToRC`, with the conversion as a
  function (`htmlToRC`), and the tool itself as a class that can be
  embedded in another program (fabricare does, as `htmlToRC(...)`).

One run:

```
html-to-rc --in=www --file-out=Web.Source.rc
```

```
www/
    index.html
    about.html
    logo.png
    css/site.css
    js/app.js
    js/lib/l.js
```

```rc
about.html HTML "www\\about.html"
index.html HTML "www\\index.html"
logo.png HTML "www\\logo.png"
css/site.css HTML "www\\css\\site.css"
js/app.js HTML "www\\js\\app.js"
js/lib/l.js HTML "www\\js\\lib\\l.js"
```

```
fabricare, applications ...
html-to-rc            <-- this library and tool
xyo-system            (Shell, IApplication, ShellArguments)
xyo-encoding, xyo-multithreading, xyo-data-structures,
xyo-managed-memory, xyo-platform
```

The sibling tools are [file-to-rc](https://github.com/g-stefan/file-to-rc)
(one file, bytes written inline as an `RCDATA` resource) and
[file-to-cs](https://github.com/g-stefan/file-to-cs) (one file as a C/C++
array or string, any platform).

## Why it exists

A Windows program with an HTML user interface (a WebBrowser control, an
embedded web server, a help viewer) needs its pages, styles, scripts and
images shipped with it. Keeping them as loose files next to the `.exe`
means they can go missing or be changed; compiling them in as resources
makes one self-contained file. Writing the `.rc` by hand means one line
per file, kept in step with the folder:

| Problem | What `html-to-rc` does |
|---------|------------------------|
| A whole folder tree must become resources, one per file | walks the folder and its subfolders, one `name HTML "path"` line per file |
| The program must find a file by its web path | the resource name is the path inside the folder, with `/`: `css/site.css` |
| The `.rc` must stay in step with the folder | regenerate it from the folder in the build; nothing is written by hand |
| Regenerating on every build forces a resource recompile | `--touch=file` regenerates only when a file anywhere in the tree is newer than the output, then touches the `.rc` that includes it |
| Several folders in one generated file | `--append` adds the lines of one more folder |
| Long build command lines | `@file` response files |

The data is **not** copied into the `.rc`: each line references the file
by path and `rc.exe` reads it when compiling (see
[Output format](output-format.md#file-paths)). To embed one file with its
bytes inside the `.rc`, use `file-to-rc`.

## Concepts at a glance

| Need | Command line | Library |
|------|--------------|---------|
| A folder tree as `HTML` resources | `html-to-rc --in=www --file-out=Web.Source.rc` | `XYO::HTMLToRC::htmlToRC("www", "Web.Source.rc", false)` |
| Only some files of the top folder | `--in=www/*.html` | `htmlToRC("www/*.html", ...)` |
| Several folders in one file | second and later calls with `--append` | `append = true` |
| Regenerate only when changed | `--touch=Application.rc` | `Shell::isChangedRecursive` yourself |
| Run the tool from your own program | | `XYO::HTMLToRC::Application::Application` (`html-to-rc.application.static`) |
| Read a file at run time | `FindResourceA(module, "css/site.css", RT_HTML)` + `LoadResource` + `LockResource` + `SizeofResource` | |
| Show the pages in a WebBrowser control | `res://MyApp.exe/index.html` (flat folders only, see [res://](output-format.md#the-res-protocol)) | |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build, install, depend on it (tool, DLL, static, embedded tool), first use |
| [Command line](command-line.md) | Every option, what `--in` accepts, `--touch`, `--append`, response files, exit codes, fabricare usage |
| [Output format](output-format.md) | Exact output, order, names, paths and how `rc.exe` finds them, reading the resources, `res://`, pitfalls |
| [Library](library.md) | `htmlToRC`, embedding the tool, metadata |
| [API reference](reference.md) | Every public symbol on one page |

## Source map

```
source/XYO/HTMLToRC.hpp                         library umbrella header, include this
source/XYO/HTMLToRC.Amalgam.cpp                 the library in one translation unit
source/XYO/HTMLToRC/
    Dependency.hpp                              xyo-system, export macro XYO_HTMLTORC_EXPORT, namespace
    Library[.hpp, .cpp]                         htmlToRC
    Copyright / License / Version               library metadata (XYO::HTMLToRC::Version::version(), ...)
    Version.Template.rh -> Version.rh           version header, generated by xyo-version
    Library.rc / Library.rh                     Windows DLL resource
source/XYO/HTMLToRC.Application.hpp             command line tool umbrella header
source/XYO/HTMLToRC.Application.Amalgam.cpp     library + tool in one translation unit
source/XYO/HTMLToRC.Application/
    Application[.hpp, .cpp]                     the html-to-rc command line tool
    Copyright / License / Version               tool metadata
    Application.rc / .manifest                  Windows executable resources
fabricare.json                                  the four projects (DLL, static lib, tool as lib, exe)
version.json                                    this repository's own versions
```

## AI assistant skill

A Claude Code skill describing how to use this library and tool lives in
[`.claude/skills/html-to-rc/`](../.claude/skills/html-to-rc/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use
`html-to-rc`.
