# Getting started

## 1. Build and install

The library and the tool are built with
[fabricare](https://github.com/g-stefan/fabricare). `xyo-platform`,
`xyo-managed-memory`, `xyo-data-structures`, `xyo-multithreading`,
`xyo-encoding` and `xyo-system` must be installed to the SDK first. From
the repository root:

```bash
fabricare make       # build into output/
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

After `install`, `html-to-rc` is in `~/.fabricare/<platform>/bin`, which
is on the `PATH` of a fabricare build, so the scripts of other projects can
call it.

Four projects are produced (`fabricare.json`):

| Project | Kind | Use it when |
|---------|------|-------------|
| `html-to-rc` | DLL / shared library (`dll-or-lib`), the library | default, shared between several programs |
| `html-to-rc.static` | static library, static CRT | self-contained executables |
| `html-to-rc.application.static` | static library with the command line tool, no `main` | embed `html-to-rc` into another tool (fabricare does) |
| `html-to-rc` | executable | the `html-to-rc` command |

The DLL and the executable share the name `html-to-rc`, so the library
keeps its version under the key `html-to-rc.library` in `version.json`
(`"versionName"`); `html-to-rc.static` reuses it and
`html-to-rc.application.static` reuses the executable's (`"linkVersion"`).

The tool and the library build on Windows and Linux; the output is only
useful to a Windows resource compiler (`rc.exe`).

## 2. Use the tool in a build

The usual case: a Windows program keeps its web user interface in a
folder next to its sources and compiles it in as resources.

```
source/XYO/MyApp/
    Application.cpp
    Application.rc
    www/
        index.html
        site.css
        app.js
```

In the project's `fabricare/make.prepare.js`:

```js
messageAction("make.prepare");

runInPath("source/XYO/MyApp", function() {
	exitIf(htmlToRC("--touch=Application.rc", "--in=www", "--file-out=Web.Source.rc"));
});
```

(`htmlToRC(...)` is the tool built into fabricare, one argument per
parameter; `Shell.system("html-to-rc ...")` works too.) It writes
`source/XYO/MyApp/Web.Source.rc`:

```rc
app.js HTML "www\\app.js"
index.html HTML "www\\index.html"
site.css HTML "www\\site.css"
```

In `source/XYO/MyApp/Application.rc`:

```rc
#include <XYO/MyApp/Web.Source.rc>
```

- Run the tool **from the folder of the `.rc` that includes the output**
  (`runInPath`) and give `--in` relative to it. The paths in the output are
  relative to where the tool ran, and `rc.exe` looks for them next to the
  `.rc` it compiles (see
  [Output format](output-format.md#file-paths)).
- Name the generated file `*.Source.rc`: fabricare compiles every `*.rc`
  of the source folder **except** the ones whose name contains `.Source.`,
  so the generated file is compiled once, through the `#include`, and not
  a second time on its own (that would be a duplicate resource).
- Do not edit it; list it in `.gitignore` or commit it.
- `--touch=Application.rc` regenerates it only when a file of `www` (at
  any depth) is newer than it and then touches `Application.rc`, so the
  incremental build recompiles the resources. See
  [Command line](command-line.md#--touch).

At run time:

```cpp
#include <windows.h>

HRSRC resource = FindResourceA(nullptr, "site.css", RT_HTML);
HGLOBAL handle = LoadResource(nullptr, resource);
const char *data = static_cast<const char *>(LockResource(handle));
DWORD size = SizeofResource(nullptr, resource);
```

or, in a WebBrowser control, navigate to `res://MyApp.exe/index.html`.
More in [Output format](output-format.md#reading-the-resources-at-run-time).

## 3. Depend on the library from another fabricare project

In the consumer's `fabricare.json`:

```json
{
	"name": "my-packer",
	"make": "exe",
	"sourcePath": "XYO/MyPacker",
	"dependency": [
		"html-to-rc"
	]
}
```

For the static variant use `"html-to-rc.static"`; it exports
`XYO_HTMLTORC_LIBRARY` to the consumer (`dependencyDefines`), which turns
`XYO_HTMLTORC_EXPORT` into nothing. To embed the command line tool use
`"html-to-rc.application.static"` (exports
`XYO_HTMLTORC_APPLICATION_LIBRARY`, which leaves out `main`). `xyo-system`
and the layers below come in as transitive dependencies.

## 4. Include

```cpp
#include <XYO/HTMLToRC.hpp>               // the library
#include <XYO/HTMLToRC.Application.hpp>   // the library + the command line tool class
```

Namespace `XYO::HTMLToRC` contains `using namespace XYO::System;`, so
`String`, `Shell::`, `IApplication`, ... are visible inside it.

Call the function qualified, `XYO::HTMLToRC::htmlToRC(...)`. The metadata
namespaces exist in every XYO layer; here the library version is
`XYO::HTMLToRC::Version::version()` and the tool's is
`XYO::HTMLToRC::Application::Version::version()`.

## 5. First program

A tool that puts two folders, `www` and `help`, into one generated
resource script, keeping each folder's name in front of the resource
names so that `www/index.html` and `help/index.html` do not collide:

```cpp
#include <XYO/HTMLToRC.hpp>

using namespace XYO::System;

class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		int main(int cmdN, char *cmdS[]);
};

int Application::main(int cmdN, char *cmdS[]) {
	// basePath = "": names keep the folder, "www/index.html", "help/index.html"
	if (!XYO::HTMLToRC::htmlToRC("www", "Web.Source.rc", false, "")) {
		printf("* Error: Web.Source.rc\n");
		return 1;
	};
	// append = true: add the lines of the second folder to the same file
	if (!XYO::HTMLToRC::htmlToRC("help", "Web.Source.rc", true, "")) {
		printf("* Error: Web.Source.rc\n");
		return 1;
	};
	return 0;
};

XYO_APPLICATION_MAIN(Application);
```

`XYO_APPLICATION_MAIN` comes from `xyo-system`: it initializes the managed
memory registry and calls `Application::main`. A missing folder is not an
error: it adds no lines (see [Library](library.md#htmltorc)).

## 6. Conventions

- **`bool` means success**, but only for the output: `htmlToRC` returns
  `false` when the output can not be opened. A missing input folder, or an
  input that is a single file, produces **no lines** and still returns
  `true`. There are no exceptions and no messages. The tool returns exit
  code `1` without a message (see
  [Command line](command-line.md#exit-codes)).
- **`--in` is a folder or a wildcard pattern, not a file.** See
  [what `--in` accepts](command-line.md#what---in-accepts).
- **Paths are used as given**, relative to the current directory. The
  output folder must exist. Do not end `--in` with a separator.
- **The output is overwritten** unless `append` / `--append` is used. Do
  not put it inside the input folder: it would list itself.
- **Names are the file paths**: keep file and folder names to ASCII,
  without spaces (see
  [Output format](output-format.md#resource-names)).
- **The data is the file**, byte for byte: `rc.exe` reads it at compile
  time; nothing is converted or padded.
- **Nothing is cached, no locking.** Do not write the same output file from
  two processes at the same time.

## 7. Building without fabricare

The tool is one translation unit plus the XYO layers. This is how
fabricare bootstraps itself (`build/fabricare.compile.json` in the
fabricare repository compiles `HTMLToRC.Application.Amalgam.cpp` into
fabricare):

1. Put `source/` of `xyo-platform`, `xyo-managed-memory`,
   `xyo-data-structures`, `xyo-multithreading`, `xyo-encoding`,
   `xyo-system` and `html-to-rc` on the include path, with the
   configuration headers of the layers that need them (see their
   documentation).
2. Define `XYO_PLATFORM_LIBRARY`, `XYO_MANAGEDMEMORY_LIBRARY`,
   `XYO_DATASTRUCTURES_LIBRARY`, `XYO_MULTITHREADING_LIBRARY`,
   `XYO_ENCODING_LIBRARY`, `XYO_SYSTEM_LIBRARY` and `XYO_HTMLTORC_LIBRARY`
   (static, no DLL export).
3. Compile `HTMLToRC.Application.Amalgam.cpp` with
   `Platform.Amalgam.cpp`, `ManagedMemory.Amalgam.cpp`,
   `DataStructures.Amalgam.cpp`, `Multithreading.Amalgam.cpp`,
   `Encoding.Amalgam.cpp` and `System.Amalgam.cpp` as C++17.

For the library only, compile `HTMLToRC.Amalgam.cpp` instead of
`HTMLToRC.Application.Amalgam.cpp`. To embed the tool without its `main`,
also define `XYO_HTMLTORC_APPLICATION_LIBRARY`.
