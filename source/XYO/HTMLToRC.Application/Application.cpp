// HTML To RC
// Copyright (c) 2007-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/HTMLToRC.hpp>
#include <XYO/HTMLToRC.Application/Application.hpp>
#include <XYO/HTMLToRC.Application/Copyright.hpp>
#include <XYO/HTMLToRC.Application/License.hpp>
#include <XYO/HTMLToRC.Application/Version.hpp>

namespace XYO::HTMLToRC::Application {

	void Application::showUsage() {
		printf("HTMLToRC - Convert file to C/C++ source\n");
		showVersion();
		printf("%s\n\n", HTMLToRC::Application::Copyright::copyright());

		printf("%s",
		       "    --help                this info\n"
		       "    --usage               this info\n"
		       "    --license             show license\n"
		       "    --version             show version\n"
		       "    --in=file/folder      input file or folder\n"
		       "    --file-out=file       output file\n"
		       "    --touch=file          touch file if changed input file\n"
		       "    --append              append content\n");
		printf("\n");
	};

	void Application::showLicense() {
		printf("%s", HTMLToRC::Application::License::license().c_str());
	};

	void Application::showVersion() {
		printf("version %s build %s [%s]\n", HTMLToRC::Application::Version::version(), HTMLToRC::Application::Version::build(), HTMLToRC::Application::Version::datetime());
	};

	void Application::initMemory() {
		String::initMemory();
		TDynamicArray<String>::initMemory();
	};

	int Application::main(int cmdN, char *cmdS[]) {
		int i;
		String opt;
		size_t optIndex;
		String optValue;
		TDynamicArray<String> cmdLine;

		String pathOrFileNameIn;
		String fileNameOut;
		bool append = false;
		bool isTouch = false;
		String touchFileName;

		for (i = 1; i < cmdN; ++i) {
			if (StringCore::beginWith(cmdS[i], "@")) {
				String content;
				if (Shell::fileGetContents(&cmdS[i][1], content)) {
					XYO::System::ShellArguments shellArguments;
					int m;
					shellArguments.set(content);
					for (m = 0; m < shellArguments.cmdN; ++m) {
						cmdLine.push(shellArguments.cmdS[m]);
					};
					continue;
				};
				printf("Error: file not found - %s\n", &cmdS[i][1]);
				return 1;
			};
			cmdLine.push(cmdS[i]);
		};

		for (i = 0; i < cmdLine.length(); ++i) {
			if (StringCore::beginWith(cmdLine[i], "--")) {
				opt = cmdLine[i].index(2);
				optValue = "";
				if (opt.indexOf("=", 0, optIndex)) {
					optValue = opt.substring(optIndex + 1);
					opt = opt.substring(0, optIndex);
				};
				if (opt == "help") {
					showUsage();
					return 0;
				};
				if (opt == "usage") {
					showUsage();
					return 0;
				};
				if (opt == "license") {
					showLicense();
					return 0;
				};
				if (opt == "version") {
					showVersion();
					return 0;
				};
				if (opt == "in") {
					pathOrFileNameIn = optValue;
					if (pathOrFileNameIn.length() == 0) {
						printf("Error: in is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "file-out") {
					fileNameOut = optValue;
					if (fileNameOut.length() == 0) {
						printf("Error: file-out is empty\n");
						return 1;
					};
					continue;
				};
				if (opt == "append") {
					append = true;
					continue;
				};
				if (opt == "touch") {
					isTouch = true;
					touchFileName = optValue;
					if (touchFileName.length() == 0) {
						printf("Error: touch filename is empty\n");
						return 1;
					};
					continue;
				};
				continue;
			};
		};

		// ---

		if (pathOrFileNameIn.length() == 0 ||
		    fileNameOut.length() == 0) {
			showUsage();
			return 1;
		};

		if (isTouch) {
			if (!Shell::isChangedRecursive(fileNameOut, pathOrFileNameIn)) {
				return 0;
			};
		};

		if (!htmlToRC(
		        pathOrFileNameIn,
		        fileNameOut,
		        append)) {
			return 1;
		};

		if (isTouch) {
			Shell::touchIfExists(touchFileName);
		};

		return 0;
	};
};

#ifndef XYO_HTMLTORC_APPLICATION_LIBRARY
XYO_APPLICATION_MAIN(XYO::HTMLToRC::Application::Application);
#endif
