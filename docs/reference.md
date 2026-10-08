# API reference

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/HTMLToRC.hpp>` | the library: `Dependency.hpp` + `Library.hpp` |
| `<XYO/HTMLToRC/Copyright.hpp>`, `License.hpp`, `Version.hpp` | library metadata (not included by `HTMLToRC.hpp`) |
| `<XYO/HTMLToRC.Application.hpp>` | the library + `XYO::HTMLToRC::Application::Application` |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_HTMLTORC_EXPORT` | `dllexport` / `dllimport` / nothing |
| `XYO_HTMLTORC_DLL_INTERNAL` | building the DLL (exports); set from `HTML_TO_RC_DLL_INTERNAL`, which xyo-cc defines from the project name |
| `XYO_HTMLTORC_INTERNAL` | set from `HTML_TO_RC_INTERNAL`; not used by the sources |
| `XYO_HTMLTORC_LIBRARY` | static use, `XYO_HTMLTORC_EXPORT` is empty (set by `html-to-rc.static`) |
| `XYO_HTMLTORC_APPLICATION_LIBRARY` | build the tool without `main` (set by `html-to-rc.application.static`) |

## namespace XYO::HTMLToRC

`using namespace XYO::System;`

| Function | Writes | Returns |
|----------|--------|---------|
| `bool htmlToRC(const char *pathOrFileNameIn, const char *fileNameOut, bool append, const char *basePath = nullptr)` | one `name HTML "path"\r\n` line per file of the folder `pathOrFileNameIn` and its subfolders (or of the files matching a `*` / `?` pattern in the top folder); `name` = file path without `basePath` (default: the input folder), `/` separated; `path` with `\` doubled; appends if `append` | `false` if the output can not be opened; `true` otherwise, also when nothing was found |

Details and limits: [Output format](output-format.md).

## namespace XYO::HTMLToRC::Version / Copyright / License

```cpp
const char *XYO::HTMLToRC::Version::version();
const char *XYO::HTMLToRC::Version::build();
const char *XYO::HTMLToRC::Version::versionWithBuild();
const char *XYO::HTMLToRC::Version::datetime();

const char *XYO::HTMLToRC::Copyright::copyright();
const char *XYO::HTMLToRC::Copyright::publisher();
const char *XYO::HTMLToRC::Copyright::company();
const char *XYO::HTMLToRC::Copyright::contact();

std::string XYO::HTMLToRC::License::license();
std::string XYO::HTMLToRC::License::shortLicense();
```

The same functions exist for the tool in
`XYO::HTMLToRC::Application::Version`, `Copyright` and `License`.

## class XYO::HTMLToRC::Application::Application

```cpp
class Application : public virtual IApplication {
	public:
		void showUsage();
		void showLicense();
		void showVersion();
		int main(int cmdN, char *cmdS[]);
		static void initMemory();
};
```

The `html-to-rc` command line tool; `main` returns the exit code. See
[Command line](command-line.md).

## Command line options

`--help`, `--usage`, `--license`, `--version`, `--in=folder`,
`--file-out=file`, `--append`, `--touch=file`, `@file`.

| Exit code | Meaning |
|-----------|---------|
| `0` | written (also when nothing was found), up to date (`--touch`), or an info option |
| `1` | missing / empty option, response file not found, output can not be written (no message for the last) |

## Resources produced

| Item | Value |
|------|-------|
| Type | `HTML`, `RT_HTML`, 23 |
| Name | file path inside the input folder, `/` separated, stored upper case; only digits → numeric id |
| Data | the file's exact bytes, read by `rc.exe` at compile time |
| Lookup | `FindResourceA(module, "css/site.css", RT_HTML)`, any case, no leading `/` |
| `res://` | `res://module/name` for files at the top of the folder only |

## fabricare projects

| Project | Kind | Defines exported to the consumer |
|---------|------|----------------------------------|
| `html-to-rc` | DLL / shared library, version key `html-to-rc.library` | |
| `html-to-rc.static` | static library, static CRT | `XYO_HTMLTORC_LIBRARY` |
| `html-to-rc.application.static` | static library, tool without `main` | `XYO_HTMLTORC_APPLICATION_LIBRARY` |
| `html-to-rc` | executable | |
