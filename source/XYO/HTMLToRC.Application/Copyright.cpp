// HTML To RC
// Copyright (c) 2007-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2007-2023  Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/HTMLToRC.Application/Copyright.hpp>
#include <XYO/HTMLToRC.Application/Copyright.rh>

namespace XYO::HTMLToRC::Application::Copyright {

	static const char *copyright_ = XYO_HTMLTORC_APPLICATION_COPYRIGHT;
	static const char *publisher_ = XYO_HTMLTORC_APPLICATION_PUBLISHER;
	static const char *company_ = XYO_HTMLTORC_APPLICATION_COMPANY;
	static const char *contact_ = XYO_HTMLTORC_APPLICATION_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};
