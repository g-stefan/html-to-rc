@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign html-to-rc

pushd output
for /r %%i in (*.exe) do call grigore-stefan.sign "HTML file/folder to RC Source" "%%i"
popd
