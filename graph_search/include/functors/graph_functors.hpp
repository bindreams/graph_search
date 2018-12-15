#pragma once

struct access_value {
	template <class T>
	decltype(auto) operator()(T&& obj) const {
		return obj.value();
	}
};