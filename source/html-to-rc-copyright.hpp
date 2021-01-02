//
// HTML to RC
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef HTML_TO_RC_COPYRIGHT_HPP
#define HTML_TO_RC_COPYRIGHT_HPP

#define HTML_TO_RC_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define HTML_TO_RC_PUBLISHER            "Grigore Stefan"
#define HTML_TO_RC_COMPANY              HTML_TO_RC_PUBLISHER
#define HTML_TO_RC_CONTACT              "g_stefan@yahoo.com"
#define HTML_TO_RC_FULL_COPYRIGHT       HTML_TO_RC_COPYRIGHT " <" HTML_TO_RC_CONTACT ">"

#ifndef XYO_RC

namespace HtmlToRc {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
