// ------------------------------------------------------------------
// container::FlagSet - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_FLAGSET_H
#define SD_CONTAINER_FLAGSET_H

#include "system/Config.hpp"
#include <limits>

namespace stardazed {
namespace container {


namespace detail {

template <typename EnumType, typename DataType>
// requires Enum<EnumType> && Unsigned<DataType>
class FlagSet {
	static_assert(std::is_enum<EnumType>::value, "EnumType must be an enum (class)");
	static_assert(std::is_integral<DataType>::value && std::is_unsigned<DataType>::value, "DataType must be an unsigned integral type");
	
	using EnumValueType = std::underlying_type_t<EnumType>;

	DataType data_ = 0;
	
	constexpr DataType EnumToMask(EnumType e) const {
		return DataType{1} << static_cast<EnumValueType>(e);
	}
	
public:
	constexpr FlagSet() = default;
	constexpr explicit FlagSet(DataType flags) : data_{flags} {}
	
	constexpr bool test(EnumType e) const {
		return data_ & EnumToMask(e);
	}
	
	constexpr FlagSet& set(EnumType e) {
		data_ |= EnumToMask(e);
		return *this;
	}
	constexpr FlagSet& reset(EnumType e) {
		data_ &= ~EnumToMask(e);
		return *this;
	}
	constexpr FlagSet& flip(EnumType e) {
		data_ ^= EnumToMask(e);
		return *this;
	}
	
	constexpr bool any() const {
		return data_ != 0;
	}
	constexpr bool all() const {
		return data_ == std::numeric_limits<DataType>::max();
	}
	constexpr bool none() const {
		return data_ == 0;
	}
	
	constexpr FlagSet& operator &=(const FlagSet& other) {
		data_ &= other.data_;
		return *this;
	}
	constexpr FlagSet& operator |=(const FlagSet& other) {
		data_ |= other.data_;
		return *this;
	}
	constexpr FlagSet& operator ^=(const FlagSet& other) {
		data_ ^= other.data_;
		return *this;
	}
	
	constexpr FlagSet operator ~() const {
		return FlagSet{ ~data_ };
	}
	
	constexpr bool operator ==(const FlagSet& other) const {
		return data_ == other.data_;
	}
	constexpr bool operator !=(const FlagSet& other) const {
		return data_ != other.data_;
	}
	
	constexpr DataType data() const {
		return data_;
	}
};


template <typename EnumType, typename DataType>
constexpr FlagSet<EnumType, DataType> operator &(const FlagSet<EnumType, DataType>& a, const FlagSet<EnumType, DataType>& b) {
	return FlagSet<EnumType, DataType>{ a.data() & b.data() };
}
template <typename EnumType, typename DataType>
constexpr FlagSet<EnumType, DataType> operator |(const FlagSet<EnumType, DataType>& a, const FlagSet<EnumType, DataType>& b) {
	return FlagSet<EnumType, DataType>{ a.data() | b.data() };
}
template <typename EnumType, typename DataType>
constexpr FlagSet<EnumType, DataType> operator ^(const FlagSet<EnumType, DataType>& a, const FlagSet<EnumType, DataType>& b) {
	return FlagSet<EnumType, DataType>{ a.data() ^ b.data() };
}

} // ns detail


template <typename EnumType>
using FlagSet32 = detail::FlagSet<EnumType, uint32_t>;

template <typename EnumType>
using FlagSet64 = detail::FlagSet<EnumType, uint64_t>;


} // ns container
} // ns stardazed

#endif
