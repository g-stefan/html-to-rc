// HTML To RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_VERSION_HPP
#define XYO_HTMLTORC_VERSION_HPP

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#	include <XYO/HTMLToRC/Dependency.hpp>
#endif

namespace XYO::HTMLToRC::Version {

	XYO_HTMLTORC_EXPORT const char *version();
	XYO_HTMLTORC_EXPORT const char *build();
	XYO_HTMLTORC_EXPORT const char *versionWithBuild();
	XYO_HTMLTORC_EXPORT const char *datetime();

};

#endif
