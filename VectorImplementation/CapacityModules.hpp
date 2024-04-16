#pragma once

namespace cap_adj {

	struct Continous {
		constexpr static bool adjust_capacity(int size, int& capacity) {
			if (size == capacity || size < capacity / 3) {
				capacity = std::max(3, size * 2);
				return true;
			}
			return false;
		}
	};

	struct IncreaseOnly {								// w/ C++ 20: template <IncreaseFactor F = {}> for even more modularity, where IncreaseFactor is a struct containing a float factor
		constexpr static bool adjust_capacity(int size, int& capacity) {
			if (size == capacity) {
				capacity = capacity * 1.5f;				// w/ C++20: * F.factor;
				return true;
			}
			return false;
		}
	};

}