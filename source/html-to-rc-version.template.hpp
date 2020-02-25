//
// HTML to RC
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef HTML_TO_RC_VERSION_HPP
#define HTML_TO_RC_VERSION_HPP

#define HTML_TO_RC_VERSION_ABCD                $VERSION_ABCD
#define HTML_TO_RC_VERSION_STR                 "$VERSION_VERSION"
#define HTML_TO_RC_VERSION_STR_BUILD           "$VERSION_BUILD"
#define HTML_TO_RC_VERSION_STR_DATETIME        "$VERSION_DATETIME"

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

