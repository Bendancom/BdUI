export module math.number;

import <concepts>;

namespace bd {
	export template<typename T>
	concept unsigned_integral = requires(T x, T y) {
		{x + y} -> std::convertible_to<T>;
		{x - y}  -> std::convertible_to<T>;
		{x * y} -> std::convertible_to<T>;
		{x / y} -> std::convertible_to<T>;
		{x++} -> std::convertible_to<T>;
		{x--} -> std::convertible_to<T>;
		{x >> 0} -> std::convertible_to<T>;
		{x << 0} -> std::convertible_to<T>;
		{x & 0} -> std::convertible_to<T>;
		{x | 0} -> std::convertible_to<T>;
		{x ^ 0} -> std::convertible_to<T>;
		x = y;
		requires T(3) / T(2) == T(1);
		requires T(-1) > T(0);
		requires std::totally_ordered<T>;
	};

	export template<typename T>
	concept integral = requires(T x, T y) {
		{x + y} -> std::convertible_to<T>;
		{x - y}  -> std::convertible_to<T>;
		{x * y} -> std::convertible_to<T>;
		{x / y} -> std::convertible_to<T>;
		{x++} -> std::convertible_to<T>;
		{x--} -> std::convertible_to<T>;
		{-x} -> std::convertible_to<T>;
		{x >> 0} -> std::convertible_to<T>;
		{x << 0} -> std::convertible_to<T>;
		{x & 0} -> std::convertible_to<T>;
		{x | 0} -> std::convertible_to<T>;
		{x ^ 0} -> std::convertible_to<T>;
		x = y;
		requires T(3) / T(2) == T(1);
		requires T(-1) < T(0);
		requires std::totally_ordered<T>;
		
	};

	export template<typename T>
	concept rational = requires (T x, T y) {
		{x + y} -> std::convertible_to<T>;
		{x - y}  -> std::convertible_to<T>;
		{x* y} -> std::convertible_to<T>;
		{x / y} -> std::convertible_to<T>;
		{x++} -> std::same_as<T>;
		{x--} -> std::same_as<T>;
		{-x} -> std::same_as<T>;
		x = y;
		requires T(3) / T(2) == T(1.5);
		requires std::totally_ordered<T>;
	};
	export template<typename T>
	constexpr std::size_t size(const T& n) { return sizeof(n); }
	export template<typename T> requires requires {  {T().size()} -> std::convertible_to<std::size_t>; }
	constexpr std::size_t size(const T& n) { return n.size(); }
}