# HTML to RC Source

Utility and C++ library to convert an HTML file tree to RC source
- Embeds a whole folder of web files (HTML, CSS, JavaScript, images, ...)
in a Windows executable or DLL, one `HTML` (`RT_HTML`) resource per file,
named by its path in the folder: `css/site.css HTML "www\\css\\site.css"`.
Read at run time with `FindResource(module, "css/site.css", RT_HTML)`, or
shown in a WebBrowser control through `res://`.
- `html-to-rc` command line tool: `--in` (a folder, or a `*` / `?`
pattern), `--file-out`, `--append` (several folders in one file), `--touch`
(regenerate only when a file of the tree changed, then touch the `.rc` that
includes it), `@file` response files.
- Library `XYO::HTMLToRC`: `htmlToRC`, and the tool as a class to embed in
other programs.

Built on `xyo-system`; linked into `fabricare`, where build scripts call it
as `htmlToRC(...)`.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, depend on it, use it in a build, first program
- [Command line](docs/command-line.md) - `html-to-rc` options, what `--in` accepts, `--touch`, `--append`, exit codes, examples
- [Output format](docs/output-format.md) - exact output, names, paths, reading the resources, `res://`, pitfalls
- [Library](docs/library.md) - `htmlToRC`, `basePath`, embedding the tool
- [API reference](docs/reference.md)

A Claude Code skill for this library is in
[.claude/skills/html-to-rc](.claude/skills/html-to-rc/SKILL.md).

## License

Copyright (c) 2007-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
