// HTML To RC
// Copyright (c) 2007-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/HTMLToRC/Version.hpp>
#include <XYO/HTMLToRC/Version.rh>

namespace XYO::HTMLToRC::Version {

	static const char *version_ = XYO_HTMLTORC_VERSION_STR;
	static const char *build_ = XYO_HTMLTORC_VERSION_STR_BUILD;
	static const char *versionWithBuild_ = XYO_HTMLTORC_VERSION_STR_WITH_BUILD;
	static const char *datetime_ = XYO_HTMLTORC_VERSION_STR_DATETIME;

	const char *version() {
		return version_;
	};
	const char *build() {
		return build_;
	};
	const char *versionWithBuild() {
		return versionWithBuild_;
	};
	const char *datetime() {
		return datetime_;
	};

};
