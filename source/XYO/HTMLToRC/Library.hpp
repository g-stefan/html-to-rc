// HTML To RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_LIBRARY_HPP
#define XYO_HTMLTORC_LIBRARY_HPP

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#	include <XYO/HTMLToRC/Dependency.hpp>
#endif

namespace XYO::HTMLToRC {

	XYO_HTMLTORC_EXPORT bool htmlToRC(const char *pathOrFileNameIn, const char *fileNameOut, bool append, const char *basePath = nullptr);

};

#endif
