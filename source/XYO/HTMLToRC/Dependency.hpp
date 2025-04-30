// HTML To RC
// Copyright (c) 2007-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_HTMLTORC_DEPENDENCY_HPP
#define XYO_HTMLTORC_DEPENDENCY_HPP

#ifndef XYO_SYSTEM_HPP
#	include <XYO/System.hpp>
#endif

// -- Export

#ifdef HTML_TO_RC_INTERNAL
#	ifndef XYO_HTMLTORC_INTERNAL
#		define XYO_HTMLTORC_INTERNAL
#	endif
#endif

#ifdef HTML_TO_RC_DLL_INTERNAL
#	ifndef XYO_HTMLTORC_DLL_INTERNAL
#		define XYO_HTMLTORC_DLL_INTERNAL
#	endif
#endif

#ifdef XYO_HTMLTORC_DLL_INTERNAL
#	define XYO_HTMLTORC_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_HTMLTORC_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_HTMLTORC_LIBRARY
#	undef XYO_HTMLTORC_EXPORT
#	define XYO_HTMLTORC_EXPORT
#endif

namespace XYO::HTMLToRC {
	using namespace XYO::System;
};

#endif
