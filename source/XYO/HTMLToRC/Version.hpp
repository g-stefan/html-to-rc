// HTML To RC
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_VERSION_HPP
#define XYO_HTMLTORC_VERSION_HPP

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#	include <XYO/HTMLToRC/Dependency.hpp>
#endif

namespace XYO::HTMLToRC::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
