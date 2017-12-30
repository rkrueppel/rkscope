#pragma once

namespace scope {

	template<uint32_t MIN_SUPPORTED, uint32_t MAX_SUPPORTED>
	class SupportedAreas {
		public:

			SupportedAreas() {
				static_assert(config::nareas >= MIN_SUPPORTED && config::nareas <= MAX_SUPPORTED, "Area support does not match configured number of areas.");
			}
	};

}
