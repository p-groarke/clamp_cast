#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace detail {
template <class T>
[[nodiscard]] constexpr auto big_max() {
	using doublemax_t = long double;
	if constexpr (std::is_floating_point_v<T>) {
		return static_cast<doublemax_t>(std::numeric_limits<T>::max());
	} else if constexpr (std::is_signed_v<T>) {
		return static_cast<intmax_t>(std::numeric_limits<T>::max());
	} else {
		return static_cast<uintmax_t>(std::numeric_limits<T>::max());
	}
}

#pragma warning(push)
#pragma warning(disable : 4756)
template <class In, class Out>
[[nodiscard]] constexpr Out clamp_hi_in(In in) {
	constexpr In low = static_cast<In>(std::numeric_limits<Out>::lowest());
	constexpr In hi = static_cast<In>(std::numeric_limits<Out>::max());

	if (in > hi) {
		return std::numeric_limits<Out>::max();
	} else if (in < low) {
		return std::numeric_limits<Out>::lowest();
	} else {
		return static_cast<Out>(in);
	}
}
#pragma warning(pop)

template <class InUnsigned, class Out>
[[nodiscard]] constexpr Out clamp_hi_in_unsigned(InUnsigned in) {
	constexpr InUnsigned low = 0;
	constexpr InUnsigned hi
			= static_cast<InUnsigned>(std::numeric_limits<Out>::max());

	if (in > hi) {
		return std::numeric_limits<Out>::max();
	} else if (in < low) {
		return static_cast<Out>(low);
	}
	return static_cast<Out>(in);
}

} // namespace detail

template <class Out, class In>
[[nodiscard]] constexpr Out clamp_cast(In in) {
	static_assert(std::is_arithmetic_v<In>,
			"saturate_cast : input type must be arithmetic");
	static_assert(std::is_arithmetic_v<Out>,
			"saturate_cast : output type must be arithmetic");

	if constexpr (std::is_same_v<In, Out>) {
		return in;
	} else if constexpr (std::is_unsigned_v<In> && std::is_signed_v<Out>) {
		constexpr auto in_max = detail::big_max<In>();
		constexpr auto out_max = detail::big_max<Out>();

		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in_unsigned<In, Out>(in);
		} else {
			return static_cast<Out>(in);
		}
	} else if constexpr (std::is_signed_v<In> && std::is_unsigned_v<Out>) {
		constexpr auto in_max = detail::big_max<In>();
		constexpr auto out_max = detail::big_max<Out>();

		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in_unsigned<In, Out>(in);
		} else {
			if (in < static_cast<In>(0)) {
				return Out{ 0 };
			}
			return static_cast<Out>(in);
		}
	} else {
		constexpr auto in_max = detail::big_max<In>();
		constexpr auto out_max = detail::big_max<Out>();

		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in<In, Out>(in);
		} else {
			return static_cast<Out>(in);
		}
	}
}
