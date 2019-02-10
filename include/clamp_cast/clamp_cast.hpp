/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2019, Philippe Groarke
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 **/

#pragma once
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace detail {
template <class T>
[[nodiscard]] constexpr auto big_max() {
	using floatmax_t = long double;
	if constexpr (std::is_floating_point_v<T>) {
		return static_cast<floatmax_t>(std::numeric_limits<T>::max());
	} else if constexpr (std::is_signed_v<T>) {
		return static_cast<intmax_t>(std::numeric_limits<T>::max());
	} else {
		return static_cast<uintmax_t>(std::numeric_limits<T>::max());
	}
}

#pragma warning(push)
#pragma warning(disable : 4756)
template <class In, class Out>
[[nodiscard]] Out clamp_hi_in(In in) {
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
[[nodiscard]] Out clamp_hi_in_unsigned(InUnsigned in) {
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
[[nodiscard]] Out clamp_cast(In in) {
	static_assert(std::is_arithmetic_v<In>,
			"saturate_cast : input type must be arithmetic");
	static_assert(std::is_arithmetic_v<Out>,
			"saturate_cast : output type must be arithmetic");

	constexpr auto in_max = detail::big_max<In>();
	constexpr auto out_max = detail::big_max<Out>();

	if constexpr (std::is_same_v<In, Out>) {
		return in;
	} else if constexpr (std::is_unsigned_v<In> && std::is_signed_v<Out>) {
		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in_unsigned<In, Out>(in);
		} else {
			return static_cast<Out>(in);
		}
	} else if constexpr (std::is_signed_v<In> && std::is_unsigned_v<Out>) {
		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in_unsigned<In, Out>(in);
		} else {
			if (in < static_cast<In>(0)) {
				return Out{ 0 };
			}
			return static_cast<Out>(in);
		}
	} else {
		if constexpr (in_max > out_max) {
			return detail::clamp_hi_in<In, Out>(in);
		} else {
			return static_cast<Out>(in);
		}
	}
}
