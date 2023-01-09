// HTML To RC
// Copyright (c) 2007-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#define XYO_HTMLTORC_DEPENDENCY_HPP

#ifndef XYO_SYSTEM_HPP
#	include <XYO/System.hpp>
#endif

// -- Export

#ifndef XYO_HTMLTORC_INTERNAL
#	ifdef HTML_TO_RC_INTERNAL
#		define XYO_HTMLTORC_INTERNAL
#	endif
#endif

#ifdef XYO_HTMLTORC_INTERNAL
#	define XYO_HTMLTORC_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_HTMLTORC_EXPORT XYO_LIBRARY_IMPORT
#endif

namespace XYO::HTMLToRC {
	using namespace XYO::System;
};

#endif
