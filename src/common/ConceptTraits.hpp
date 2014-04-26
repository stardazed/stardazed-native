// ------------------------------------------------------------------
// ConceptTraits - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONCEPTTRAITS_H
#define SD_CONCEPTTRAITS_H

namespace stardazed {


// A CRTP based trait to extend a type that has op== and op< to
// support all other comparison operators as well.

template <typename Derived>
struct FullyComparableTrait {
	// requires operator== and operator<
	template <typename U>
	constexpr bool operator!=(const U& other) const {
		return !static_cast<const Derived*>(this)->operator==(other);
	}

	template <typename U>
	constexpr bool operator<=(const U& other) const {
		return static_cast<const Derived*>(this)->operator<(other) || static_cast<const Derived*>(this)->operator==(other);
	}

	template <typename U>
	constexpr bool operator>(const U& other) const {
		return !(static_cast<const Derived*>(this)->operator<(other) || static_cast<const Derived*>(this)->operator==(other));
	}

	template <typename U>
	constexpr bool operator>=(const U& other) const {
		return !static_cast<const Derived*>(this)->operator<(other);
	}
};


} // ns stardazed

#endif
