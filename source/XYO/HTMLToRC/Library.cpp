// HTML To RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/HTMLToRC/Library.hpp>

namespace XYO::HTMLToRC {

	bool htmlToRC(
	    const char *pathOrFileNameIn,
	    const char *fileNameOut,
	    bool append,
	    const char *basePath) {

		int k;
		size_t index;
		TDynamicArray<String> fileList;
		FILE *output;
		String pathToSearch = pathOrFileNameIn;
		String line;
		String basePathX;
		String fileName;

		if (!(StringX::indexOf(pathToSearch, "*", 0, index) || StringX::indexOf(pathToSearch, "?", 0, index))) {
			pathToSearch += "/*";
		};

		if (basePath) {
			basePathX = basePath;
		} else {
			basePathX = Shell::getFilePathX(pathToSearch);
		};

		output = fopen(fileNameOut, append ? "ab" : "wb");
		if (output != nullptr) {

			Shell::getFileList(pathToSearch, fileList);
			for (k = 0; k < fileList.length(); ++k) {
				fileName = StringX::substring(fileList[k], basePathX.length());
				line = StringX::replace(fileName, "\\", "/") +
				       " HTML \"" +
				       StringX::replace(StringX::replace(fileList[k], "/", "\\"), "\\", "\\\\") +
				       "\"\r\n";
				fwrite(line.value(), 1, line.length(), output);
			};
			fclose(output);

			fileList.empty();

			Shell::getDirList(pathToSearch, fileList);
			for (k = 0; k < fileList.length(); ++k) {
				if (!htmlToRC(fileList[k], fileNameOut, true, basePathX)) {
					return false;
				};
			};

			return true;
		};
		return false;
	};

};
