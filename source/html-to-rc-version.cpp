//
// HTML to RC
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "html-to-rc-version.hpp"

namespace HtmlToRc {
	namespace Version {

		static const char *version_ = "1.9.0";
		static const char *build_ = "22";
		static const char *versionWithBuild_ = "1.9.0.22";
		static const char *datetime_ = "2021-11-15 16:51:47";

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
};

