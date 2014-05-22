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


// Standardized copy / move / none accessor traits


struct NoCopyBaseClassTrait {
	NoCopyBaseClassTrait(const NoCopyBaseClassTrait&) = delete;
	NoCopyBaseClassTrait& operator=(const NoCopyBaseClassTrait&) = delete;

public:
	virtual ~NoCopyBaseClassTrait() = default;

protected:
	// traits may not be created directly
	NoCopyBaseClassTrait() = default;
};



struct MoveOnlyBaseClassTrait {
	MoveOnlyBaseClassTrait(const MoveOnlyBaseClassTrait&) = delete;
	const MoveOnlyBaseClassTrait& operator=(const MoveOnlyBaseClassTrait&) = delete;
	
protected:
	// traits may not be created directly
	MoveOnlyBaseClassTrait() = default;

public:
	virtual ~MoveOnlyBaseClassTrait() = default;
	MoveOnlyBaseClassTrait(MoveOnlyBaseClassTrait&&) = default;
	MoveOnlyBaseClassTrait& operator=(MoveOnlyBaseClassTrait&&) = default;
};


} // ns stardazed

#endif
