// HTML To RC
// Copyright (c) 2007-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_APPLICATION_VERSION_HPP
#define XYO_HTMLTORC_APPLICATION_VERSION_HPP

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#	include <XYO/HTMLToRC/Dependency.hpp>
#endif

namespace XYO::HTMLToRC::Application::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
