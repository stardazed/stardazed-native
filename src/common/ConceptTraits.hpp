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


// Types that should not be moved or copied, mostly used for global
// instances of system classes. Note: not the same as a Singleton
// There can still be multiple instances of these classes

struct NoCopyAllowedTrait {
	NoCopyAllowedTrait(const NoCopyAllowedTrait&) = delete;
	const NoCopyAllowedTrait& operator=(const NoCopyAllowedTrait&) = delete;

protected:
	// traits may not be created directly
	NoCopyAllowedTrait() = default;
};

} // ns stardazed

#endif
