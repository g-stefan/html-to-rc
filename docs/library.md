# Library

```cpp
#include <XYO/HTMLToRC.hpp>

namespace XYO::HTMLToRC {
	bool htmlToRC(const char *pathOrFileNameIn, const char *fileNameOut, bool append, const char *basePath = nullptr);
};
```

The parameters are `const char *`; a `String` converts to it implicitly.
It returns `true` on success and `false` when the output can not be
written, prints nothing and throws nothing. The exact output is in
[Output format](output-format.md).

## htmlToRC

```cpp
// index.html HTML "www\\index.html"\r\n, css/site.css HTML "www\\css\\site.css"\r\n, ...
XYO::HTMLToRC::htmlToRC("www", "Web.Source.rc", false);

// only the .html files at the top of www, added to the same file
XYO::HTMLToRC::htmlToRC("www/*.html", "Web.Source.rc", true);

// names keep the folder: help/index.html HTML "help\\index.html"
XYO::HTMLToRC::htmlToRC("help", "Web.Source.rc", true, "");
```

| Parameter | Meaning |
|-----------|---------|
| `pathOrFileNameIn` | a folder (all files, all subfolders), or a pattern with `*` / `?` for the top folder; **not** a single file (see [what `--in` accepts](command-line.md#what---in-accepts)) |
| `fileNameOut` | output; its folder must exist |
| `append` | `false`: replace the output; `true`: add to its end (create it if missing) |
| `basePath` | the prefix cut from each file path to make the resource name; `nullptr` (default): the input folder, so names are relative to it; `""`: nothing is cut, names are the full paths as found (`help/index.html`) |

- Returns `false` only if the output can not be opened (missing output
  folder, no access); write errors after that (a full disk) are not
  detected. A folder that does not exist adds no lines and returns
  `true`.
- `basePath` is cut by **length**: it must be a prefix of the paths the
  function finds, which start with `pathOrFileNameIn` as given. `""` and
  `nullptr` are always safe; any other value must be `pathOrFileNameIn`
  itself or a leading part of it ending with a separator
  (`htmlToRC("site/www", out, false, "site/")` → `www/index.html`).
- The function is recursive: each subfolder is handled by a call with
  `append = true` and the same `basePath`.
- The output is opened before the folder is read; do not put it inside the
  folder.

This is the function behind the command line tool, which always passes
`basePath = nullptr`.

## Regenerate only when changed

The library function always writes. To do what the tool's `--touch` does:

```cpp
if (Shell::isChangedRecursive(out, in)) {
	if (!XYO::HTMLToRC::htmlToRC(in, out, false)) {
		return false;
	};
	Shell::touchIfExists(includingRc);
};
```

`Shell::isChangedRecursive(target, source)` is `true` when `target` does
not exist or a file under `source` (any depth) is newer than it.

## Embedding the command line tool

Depend on `html-to-rc.application.static` and call the tool class with an
argument vector, the way fabricare does:

```cpp
#include <XYO/HTMLToRC.Application.hpp>

int runHTMLToRC(TDynamicArray<String> &arguments) {
	int cmdN = (int)arguments.length() + 1;
	char **cmdS = new char *[cmdN];
	cmdS[0] = const_cast<char *>("html-to-rc");
	for (int k = 1; k < cmdN; ++k) {
		cmdS[k] = const_cast<char *>(arguments[k - 1].value());
	};
	int retV;
	{
		XYO::HTMLToRC::Application::Application application;
		retV = application.main(cmdN, cmdS);
	};
	delete[] cmdS;
	return retV;
};
```

`cmdS[0]` is the program name and is skipped. `TDynamicArray` is not
contiguous memory, so build a plain `char *` array. The tool prints to
`stdout` and returns the exit code; it never calls `exit`. The options are
in [Command line](command-line.md).

## Library metadata

As in every XYO library (the headers are not included by
`<XYO/HTMLToRC.hpp>`):

```cpp
#include <XYO/HTMLToRC/Version.hpp>
#include <XYO/HTMLToRC/Copyright.hpp>
#include <XYO/HTMLToRC/License.hpp>

XYO::HTMLToRC::Version::version();            // "4.9.0"
XYO::HTMLToRC::Version::build();
XYO::HTMLToRC::Version::versionWithBuild();
XYO::HTMLToRC::Version::datetime();
XYO::HTMLToRC::Copyright::copyright();
XYO::HTMLToRC::License::license();            // std::string
```

The tool has the same set in `XYO::HTMLToRC::Application::Version`,
`Copyright` and `License`.
