#pragma once

#include <string>

namespace scratch {

	struct Resource {
		int x;
		constexpr Resource() : x(0) {}
	};

	class ResourceAccessor
	{

	public:
		static void initialize();
		static Resource acquire(const std::string&);

		ResourceAccessor(ResourceAccessor const&) = delete;
		void operator=(ResourceAccessor const&) = delete;
	};
};

