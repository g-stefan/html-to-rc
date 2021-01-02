//
// HTML to RC
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef HTML_TO_RC_VERSION_HPP
#define HTML_TO_RC_VERSION_HPP

#define HTML_TO_RC_VERSION_ABCD                1,2,0,5
#define HTML_TO_RC_VERSION_STR                 "1.2.0"
#define HTML_TO_RC_VERSION_STR_BUILD           "5"
#define HTML_TO_RC_VERSION_STR_DATETIME        "2020-12-01 21:07:55"

#ifndef XYO_RC

namespace HtmlToRc {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif

