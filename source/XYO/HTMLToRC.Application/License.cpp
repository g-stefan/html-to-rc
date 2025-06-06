// HTML To RC
// Copyright (c) 2007-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/HTMLToRC.Application/License.hpp>

namespace XYO::HTMLToRC::Application::License {

	std::string license() {
		return XYO::HTMLToRC::License::license();
	};

	std::string shortLicense() {
		return XYO::HTMLToRC::License::shortLicense();
	};

};
