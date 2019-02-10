#include <boost/multiprecision/cpp_bin_float.hpp>
#include <cassert>
#include <chrono>
#include <clamp_cast/clamp_cast.hpp>
#include <cstddef>
#include <cstdio>
#include <gtest/gtest.h>
#include <random>
#include <tuple>

#define test_hi_to_lo(T, U) \
	{ \
		T t = std::numeric_limits<T>::lowest(); \
		U u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::lowest()); \
\
		t = std::numeric_limits<T>::max(); \
		u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::max()); \
	}

#define test_lo_to_hi(U, T) \
	{ \
		T t = std::numeric_limits<T>::lowest(); \
		U u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::lowest()); \
\
		t = std::numeric_limits<T>::max(); \
		u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::max()); \
	}

#define test_lo_to_hi_unsigned(U, T) \
	{ \
		T t = std::numeric_limits<T>::lowest(); \
		U u = clamp_cast<U>(t); \
		EXPECT_EQ(u, U{ 0 }); \
\
		t = std::numeric_limits<T>::max(); \
		u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::max()); \
	}

#define test_hi_unsigned_to_lo(T, U) \
	{ \
		U u = std::numeric_limits<U>::lowest(); \
		T t = clamp_cast<T>(u); \
		EXPECT_EQ(t, T{ 0 }); \
\
		t = std::numeric_limits<T>::max(); \
		u = clamp_cast<U>(t); \
		EXPECT_EQ(u, std::numeric_limits<U>::max()); \
	}

namespace {
template <class... Args>
void unused(Args...) noexcept {
}

template <class Tuple, class Func>
void for_each(Tuple& tuple, Func&& func) noexcept {
	std::apply([&](auto... args) { (std::invoke(func, args), ...); }, tuple);
}

template <class T, class U>
void debug_hi_to_lo() {
	T t = std::numeric_limits<T>::lowest();
	U u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::lowest());

	t = std::numeric_limits<T>::max();
	u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::max());
}

template <class T, class U>
void debug_hi_unsigned_to_lo() {
	U u = std::numeric_limits<U>::lowest();
	T t = clamp_cast<T>(u);
	EXPECT_EQ(t, T{ 0 });

	t = std::numeric_limits<T>::max();
	u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::max());
}

template <class U, class T>
void debug_lo_to_hi() {
	T t = std::numeric_limits<T>::lowest();
	U u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::lowest());

	t = std::numeric_limits<T>::max();
	u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::max());
}

template <class U, class T>
void debug_lo_to_hi_unsigned() {
	U u = std::numeric_limits<U>::lowest();
	T t = clamp_cast<T>(u);
	EXPECT_EQ(t, T{ 0 });

	t = std::numeric_limits<T>::max();
	u = clamp_cast<U>(t);
	EXPECT_EQ(u, std::numeric_limits<U>::max());
}

TEST(clamp_cast, int_to_int) {
	test_hi_to_lo(int64_t, int32_t);
	test_hi_to_lo(int64_t, int16_t);
	test_hi_to_lo(int64_t, int8_t);

	test_lo_to_hi(int32_t, int64_t);
	test_hi_to_lo(int32_t, int16_t);
	test_hi_to_lo(int32_t, int8_t);

	test_lo_to_hi(int16_t, int64_t);
	test_lo_to_hi(int16_t, int32_t);
	test_hi_to_lo(int16_t, int8_t);

	test_lo_to_hi(int8_t, int64_t);
	test_lo_to_hi(int8_t, int32_t);
	test_lo_to_hi(int8_t, int16_t);
}

TEST(clamp_cast, float_to_float) {
	test_hi_to_lo(long double, double);
	test_hi_to_lo(long double, float);

	test_lo_to_hi(double, long double);
	test_hi_to_lo(double, float);

	test_lo_to_hi(float, long double);
	test_lo_to_hi(float, double);
}

TEST(clamp_cast, float_to_int) {
	test_hi_to_lo(long double, int64_t);
	test_hi_to_lo(long double, int32_t);
	test_hi_to_lo(long double, int16_t);
	test_hi_to_lo(long double, int8_t);

	test_hi_to_lo(double, int64_t);
	test_hi_to_lo(double, int32_t);
	test_hi_to_lo(double, int16_t);
	test_hi_to_lo(double, int8_t);

	test_hi_to_lo(float, int64_t);
	test_hi_to_lo(float, int32_t);
	test_hi_to_lo(float, int16_t);
	test_hi_to_lo(float, int8_t);
}

TEST(clamp_cast, int_to_float) {
	test_lo_to_hi(int64_t, long double);
	test_lo_to_hi(int64_t, double);
	test_lo_to_hi(int64_t, float);

	test_lo_to_hi(int32_t, long double);
	test_lo_to_hi(int32_t, double);
	test_lo_to_hi(int32_t, float);

	test_lo_to_hi(int16_t, long double);
	test_lo_to_hi(int16_t, double);
	test_lo_to_hi(int16_t, float);

	test_lo_to_hi(int8_t, long double);
	test_lo_to_hi(int8_t, double);
	test_lo_to_hi(int8_t, float);
}

TEST(clamp_cast, uint_to_uint) {
	test_hi_to_lo(uint64_t, uint32_t);
	test_hi_to_lo(uint64_t, uint16_t);
	test_hi_to_lo(uint64_t, uint8_t);

	test_lo_to_hi(uint32_t, uint64_t);
	test_hi_to_lo(uint32_t, uint16_t);
	test_hi_to_lo(uint32_t, uint8_t);

	test_lo_to_hi(uint16_t, uint64_t);
	test_lo_to_hi(uint16_t, uint32_t);
	test_hi_to_lo(uint16_t, uint8_t);

	test_lo_to_hi(uint8_t, uint64_t);
	test_lo_to_hi(uint8_t, uint32_t);
	test_lo_to_hi(uint8_t, uint16_t);
}

TEST(clamp_cast, int_to_uint) {
	test_lo_to_hi_unsigned(int64_t, uint64_t);
	test_hi_to_lo(int64_t, uint32_t);
	test_hi_to_lo(int64_t, uint16_t);
	test_hi_to_lo(int64_t, uint8_t);

	test_lo_to_hi_unsigned(int32_t, uint64_t);
	test_lo_to_hi_unsigned(int32_t, uint32_t);
	test_hi_to_lo(int32_t, uint16_t);
	test_hi_to_lo(int32_t, uint8_t);

	test_lo_to_hi_unsigned(int16_t, uint64_t);
	test_lo_to_hi_unsigned(int16_t, uint32_t);
	test_lo_to_hi_unsigned(int16_t, uint16_t);
	test_hi_to_lo(int16_t, uint8_t);

	test_lo_to_hi_unsigned(int8_t, uint64_t);
	test_lo_to_hi_unsigned(int8_t, uint32_t);
	test_lo_to_hi_unsigned(int8_t, uint16_t);
	test_lo_to_hi_unsigned(int8_t, uint8_t);
}

TEST(clamp_cast, float_to_uint) {
	test_hi_to_lo(long double, uint64_t);
	test_hi_to_lo(long double, uint32_t);
	test_hi_to_lo(long double, uint16_t);
	test_hi_to_lo(long double, uint8_t);

	test_hi_to_lo(double, uint64_t);
	test_hi_to_lo(double, uint32_t);
	test_hi_to_lo(double, uint16_t);
	test_hi_to_lo(double, uint8_t);

	test_hi_to_lo(float, uint64_t);
	test_hi_to_lo(float, uint32_t);
	test_hi_to_lo(float, uint16_t);
	test_hi_to_lo(float, uint8_t);
}

TEST(clamp_cast, uint_to_int) {
	test_hi_unsigned_to_lo(uint64_t, int64_t);
	test_hi_unsigned_to_lo(uint64_t, int32_t);
	test_hi_unsigned_to_lo(uint64_t, int16_t);
	test_hi_unsigned_to_lo(uint64_t, int8_t);

	test_lo_to_hi(uint32_t, int64_t);
	test_hi_unsigned_to_lo(uint32_t, int32_t);
	test_hi_unsigned_to_lo(uint32_t, int16_t);
	test_hi_unsigned_to_lo(uint32_t, int8_t);

	test_lo_to_hi(uint16_t, int64_t);
	test_lo_to_hi(uint16_t, int32_t);
	test_hi_unsigned_to_lo(uint16_t, int16_t);
	test_hi_unsigned_to_lo(uint16_t, int8_t);

	test_lo_to_hi(uint8_t, int64_t);
	test_lo_to_hi(uint8_t, int32_t);
	test_lo_to_hi(uint8_t, int16_t);
	test_hi_unsigned_to_lo(uint8_t, int8_t);
}

TEST(clamp_cast, uint_to_float) {
	test_lo_to_hi(uint64_t, long double);
	test_lo_to_hi(uint64_t, double);
	test_lo_to_hi(uint64_t, float);

	test_lo_to_hi(uint32_t, long double);
	test_lo_to_hi(uint32_t, double);
	test_lo_to_hi(uint32_t, float);

	test_lo_to_hi(uint16_t, long double);
	test_lo_to_hi(uint16_t, double);
	test_lo_to_hi(uint16_t, float);

	test_lo_to_hi(uint8_t, long double);
	test_lo_to_hi(uint8_t, double);
	test_lo_to_hi(uint8_t, float);
}

TEST(clamp_cast, all_permutations) {
	std::tuple<uint64_t, uint32_t, uint16_t, uint8_t, int64_t, int32_t, int16_t,
			int8_t, long double, double, float>
			all_types{};

	for_each(all_types, [&](auto val1) {
		for_each(all_types, [&](auto val2) {
			unused(val1, val2);
			using val_t1 = std::decay_t<decltype(val1)>;
			using val_t2 = std::decay_t<decltype(val2)>;

			// Check signed / unsigned clamping
			{
				for (int8_t i = std::numeric_limits<int8_t>::lowest();
						i < std::numeric_limits<int8_t>::max(); ++i) {
					val_t1 v1 = clamp_cast<val_t1>(i);
					val_t2 v2 = clamp_cast<val_t2>(v1);
					if constexpr (std::is_unsigned_v<val_t1>) {
						if (i < 0) {
							EXPECT_EQ(v1, val_t1{ 0 });
						}
					} else {
						EXPECT_EQ(v1, static_cast<val_t1>(i));
					}

					if constexpr (std::is_unsigned_v<val_t2>) {
						if (v1 < val_t1{ 0 }) {
							EXPECT_EQ(v2, val_t2{ 0 });
						}
					} else {
						EXPECT_EQ(v2, static_cast<val_t2>(v1));
					}
				}
			}

			// Thorough checks
			{
				val_t1 v1_low = std::numeric_limits<val_t1>::lowest();
				val_t1 v1_hi = std::numeric_limits<val_t1>::max();
				val_t2 v2_low = std::numeric_limits<val_t2>::lowest();
				val_t2 v2_hi = std::numeric_limits<val_t2>::max();

				using namespace boost::multiprecision;
				using d128 = cpp_bin_float_quad;

				d128 v1_low128 = static_cast<d128>(v1_low);
				d128 v1_hi128 = static_cast<d128>(v1_hi);
				d128 v2_low128 = static_cast<d128>(v2_low);
				d128 v2_hi128 = static_cast<d128>(v2_hi);

				val_t1 v1 = clamp_cast<val_t1>(v2_low);
				if (v1_low128 <= v2_low128) {
					EXPECT_EQ(v1, static_cast<val_t1>(v2_low));
				} else {
					EXPECT_EQ(v1, v1_low);
				}
				v1 = clamp_cast<val_t1>(v2_hi);
				if (v1_hi128 >= v2_hi128) {
					EXPECT_EQ(v1, static_cast<val_t1>(v2_hi));
				} else {
					EXPECT_EQ(v1, v1_hi);
				}

				val_t2 v2 = clamp_cast<val_t2>(v1_low);
				if (v2_low128 <= v1_low128) {
					EXPECT_EQ(v2, static_cast<val_t2>(v1_low));
				} else {
					EXPECT_EQ(v2, v2_low);
				}
				v2 = clamp_cast<val_t2>(v1_hi);
				if (v2_hi128 >= v1_hi128) {
					EXPECT_EQ(v2, static_cast<val_t2>(v1_hi));
				} else {
					EXPECT_EQ(v2, v2_hi);
				}
			}
		});
	});
}

TEST(clamp_cast, edges) {
	std::tuple<uint64_t, uint32_t, uint16_t, uint8_t, int64_t, int32_t, int16_t,
			int8_t, long double, double, float>
			all_types{};

	for_each(all_types, [&](auto val1) {
		for_each(all_types, [&](auto val2) {
			unused(val1, val2);
			using val_t1 = std::decay_t<decltype(val1)>;
			using val_t2 = std::decay_t<decltype(val2)>;

			val_t1 v1_low = std::numeric_limits<val_t1>::lowest();
			val_t1 v1_hi = std::numeric_limits<val_t1>::max();
			val_t2 v2_low = std::numeric_limits<val_t2>::lowest();
			val_t2 v2_hi = std::numeric_limits<val_t2>::max();

			using namespace boost::multiprecision;
			using d128 = cpp_bin_float_quad;

			d128 v1_low128 = static_cast<d128>(v1_low);
			d128 v1_hi128 = static_cast<d128>(v1_hi);
			d128 v2_low128 = static_cast<d128>(v2_low);
			d128 v2_hi128 = static_cast<d128>(v2_hi);

			constexpr val_t1 edges{ 20 };

			if constexpr (std::is_floating_point_v<val_t1>) {
				val_t1 stop_iter = std::nextafter(v1_low, v1_hi);
				for (size_t i = 0; i < edges; ++i) {
					stop_iter = std::nextafter(stop_iter, v1_hi);
				}

				for (val_t1 i = v1_low; i < stop_iter;
						i = std::nextafter(i, v1_hi)) {
					val_t2 v2 = clamp_cast<val_t2>(i);
					if (v2_low128 <= v1_low128) {
						EXPECT_EQ(v2, static_cast<val_t2>(i));
					} else {
						EXPECT_EQ(v2, v2_low);
					}
				}

				stop_iter = std::nextafter(v1_hi, v1_low);
				for (size_t i = 0; i < edges; ++i) {
					stop_iter = std::nextafter(stop_iter, v1_low);
				}

				for (val_t1 i = v1_hi; i > stop_iter;
						i = std::nextafter(i, v1_low)) {
					val_t2 v2 = clamp_cast<val_t2>(i);
					if (v2_hi128 >= v1_hi128) {
						EXPECT_EQ(v2, static_cast<val_t2>(i));
					} else {
						EXPECT_EQ(v2, v2_hi);
					}
				}
			} else {
				for (val_t1 i = v1_low; i < v1_low + edges; ++i) {
					val_t2 v2 = clamp_cast<val_t2>(i);
					if (v2_low128 <= v1_low128) {
						EXPECT_EQ(v2, static_cast<val_t2>(i));
					} else {
						EXPECT_EQ(v2, v2_low);
					}
				}

				for (val_t1 i = v1_hi; i > v1_hi - edges; --i) {
					val_t2 v2 = clamp_cast<val_t2>(i);
					if (v2_hi128 >= v1_hi128) {
						EXPECT_EQ(v2, static_cast<val_t2>(i));
					} else {
						EXPECT_EQ(v2, v2_hi);
					}
				}
			}
		});
	});
}
} // namespace


int main(int argc, char** argv) {
	double ld = -42.0;
	unsigned char uc = clamp_cast<unsigned char>(ld);
	printf("%u\n", uc);

	float f = 500000.f;
	char c = clamp_cast<char>(f);
	printf("%d\n", c);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
