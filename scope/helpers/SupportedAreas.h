#pragma once

namespace scope {

	template<uint32_t MIN_SUPPORTED, uint32_t MAX_SUPPORTED>
	class SupportedAreas {
		public:

			SupportedAreas() {
				assert(config::totalareas >= MIN_SUPPORTED && config::totalareas <= MAX_SUPPORTED);
			}
	};

}
