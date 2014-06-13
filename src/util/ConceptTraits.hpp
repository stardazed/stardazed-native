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

protected:
	// traits may not be created directly
	FullyComparableTrait() = default;
};


// Standardized copy / move / none accessor macros

#define SD_NOCOPYORMOVE_CLASS(Class)     \
Class(const Class&) = delete;            \
Class& operator=(const Class&) = delete;


#define SD_DEFAULT_MOVE_OPS(Class)   \
Class(Class&&) = default;            \
Class& operator=(Class&&) = default;


// Explicitly unused parameter
#define SD_UNUSED_PARAM_HELPER(p) #p
#define SD_UNUSED_PARAM(param) _Pragma(SD_UNUSED_PARAM_HELPER(unused(param)))


} // ns stardazed

#endif
