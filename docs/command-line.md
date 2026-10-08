# Command line

```
html-to-rc [options] [@file ...]
```

## Options

| Option | Effect |
|--------|--------|
| `--help`, `--usage` | print usage, exit 0 |
| `--license` | print the license, exit 0 |
| `--version` | print the tool's own version, exit 0 |
| `--in=folder` | input folder, or a wildcard pattern (`folder/*.html`); required |
| `--file-out=file` | output `.rc` file; required; its folder must exist |
| `--append` | add the lines to the end of the output instead of replacing it |
| `--touch=file` | regenerate only if a file of the input tree is newer than the output; afterwards touch `file` |
| `@file` | read more arguments from `file` (a response file) |

`--help`, `--usage`, `--license` and `--version` act as soon as they are
seen. Arguments that do not start with `--` (other than `@file`) and
unknown options are ignored. An empty `--in=`, `--file-out=` or `--touch=`
is an error. When a required option is missing, the usage is printed and
the exit code is `1`.

The exact output is in [Output format](output-format.md).

## What --in accepts

| `--in` | Lines written | Resource names |
|--------|---------------|----------------|
| `www` (a folder) | every file of `www` and of all its subfolders | relative to `www`: `index.html`, `css/site.css` |
| `www/*.html` (a pattern) | the files of `www` matching `*.html`; subfolders only if their **own name** matches, then all of their files | relative to `www`: `index.html` |
| `C:\site\www` (absolute) | as for a folder | relative to the folder; the paths in the output are absolute |
| `www\css` (backslash) | as for a folder | relative to `www\css`: `site.css` |
| `www/index.html` (a single file) | **nothing**: the output is created empty, exit `0` | — |
| `missing` (does not exist) | **nothing**: the output is created empty, exit `0` | — |
| `www/` (ends with a separator) | as for `www`, but every path has a doubled separator (`www\\\\index.html`) | as for `www` |

- An `--in` without `*` or `?` is taken as a folder and `/*` is added to
  it. With `*` or `?` it is used as the pattern for the top folder only.
- Inside the folder, the files come first (in the order the file system
  lists them, alphabetical on NTFS), then each subfolder, depth first.
- To embed a single file, write its line yourself
  (`index.html HTML "index.html"`) or use
  [file-to-rc](https://github.com/g-stefan/file-to-rc).

## Order of work

1. Expand the `@file` response files, then read the options.
2. Check `--in` and `--file-out`.
3. `--touch`: if the output exists and no file of the input tree is
   **newer** than it, stop with exit code `0`; nothing is written or
   touched.
4. Write the output, replacing it or (`--append`) appending to it.
5. `--touch`: touch the file if it exists (a missing one is not an error).

## --touch

The generated file is `#include`d by a hand written `.rc`. A build tool
that checks only the time of the `.rc` will not see a change in the
generated file, and regenerating on every build changes its time and
recompiles it every time. `--touch=Application.rc` handles both:

- no file of the input tree newer than the output → nothing happens, the
  build stays incremental;
- a file newer (at any depth), or no output yet → the output is written
  and `Application.rc` gets the current time, so the resources are
  recompiled.

The check (`Shell::isChangedRecursive`) compares the last write time of
the output with every file under `--in`, in all subfolders (links are
followed, link loops are skipped).

What `--touch` does **not** see: a file **removed** from the folder, or an
older file copied in with its old time. The output then keeps (or lacks)
its line until something else in the folder changes; `rc.exe` fails with
`file not found` for a removed file. Delete the output (or
`fabricare clean`) after removing or renaming files.

Do not combine `--touch` with `--append` on a shared output: once the
first input has written the file, the next ones are compared against it
and skipped.

## --append

```
html-to-rc --in=www --file-out=Web.Source.rc
html-to-rc --in=help --file-out=Web.Source.rc --append
```

The first call replaces the output, the next ones add the lines of one
more folder. Every run of an `--append` call adds again: start the
sequence with a call without `--append` (or remove the output first), or
the file collects duplicate resources. `rc.exe` accepts them; the link
fails later with `CVTRES : fatal error CVT1100: duplicate resource`.

The names are relative to each `--in`, so two folders that both have an
`index.html` give two `index.html` resources: a duplicate. The command
line can not keep the folder name in front; the library can
(`basePath`, see [Library](library.md#htmltorc)).

## Exit codes

| Code | Meaning |
|------|---------|
| `0` | written (also when nothing was found, see above); up to date (`--touch`); `--help` / `--usage` / `--license` / `--version` |
| `1` | missing or empty required option, response file not found, output can not be written |

Messages go to `stdout`. Option errors print `Error: in is empty`,
`Error: file-out is empty`, `Error: touch filename is empty`, or
`Error: file not found - <file>` for a response file. An output that can
not be written (output folder missing, no access) prints **nothing**:
only the exit code says it failed. An input that does not exist is not
reported at all: check that the output is not empty if that matters.

## Response files

`@file` is replaced by the arguments read from `file`, split like a command
line (quotes group words). Several `@file` and normal arguments can be
mixed:

```
--in=www
"--file-out=Web.Source.rc"
```

```
html-to-rc @web.args --touch=Application.rc
```

## From fabricare

Inside fabricare the tool is linked in (`html-to-rc.application.static`)
and is available to scripts as the function `htmlToRC(...)`, which takes
the same arguments as strings, one per parameter, and returns the exit
code:

```js
runInPath("source/XYO/MyApp", function() {
	exitIf(htmlToRC("--touch=Application.rc", "--in=www", "--file-out=Web.Source.rc"));
});
```

The installed executable is on the `PATH` of the build as well:

```js
exitIf(Shell.system("html-to-rc --touch=Application.rc --in=www --file-out=Web.Source.rc"));
```

## Examples

A folder tree:

```
html-to-rc --in=www --file-out=Web.Source.rc
```

Only the `.html` files of the top folder:

```
html-to-rc "--in=www/*.html" --file-out=Pages.Source.rc
```

Only when a file changed:

```
html-to-rc --touch=Application.rc --in=www --file-out=Web.Source.rc
```
