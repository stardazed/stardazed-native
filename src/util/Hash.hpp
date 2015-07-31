// ------------------------------------------------------------------
// util::Hash - stardazed
// (c) 2015 by Arthur Langereis
// Parts lifted from or inspired by LLVM's libc++ std::hash implementation
// ------------------------------------------------------------------

#ifndef SD_UTIL_HASH_H
#define SD_UTIL_HASH_H

#include "system/Config.hpp"
#include "farmhash.h"

namespace stardazed {


/* ------------------------------------------------------------------

Usage
=====

to hash any object or scalar:
	auto h = hash(anything);

to hash a range of bytes:
	auto h = hashBlock(basePtr, sizeInBytes);

to specialize the hash function for a custom type:
	template <>
	struct Hash<MyObject> {
		static uint64 hash(const MyObject& obj {
			...
		}
	};

------------------------------------------------------------------ */


uint64 hashBlock(const void* data, size_t byteSize) {
	return util::Hash64(static_cast<const char*>(data), byteSize);
}


template <class T>
struct Hash {
	static uint64 hash(const T& t) {
		return hashBlock(&t, sizeof(t));
	}
};


// -- Some assumptions we make here
// If these do not hold then the Hash implementations will have to be expanded
// a bit for those types. Essentially expecting an LP/LLP 64-bit environment.
static_assert(sizeof(int) <= sizeof(uint64), "");
static_assert(sizeof(long) <= sizeof(uint64), "");
static_assert(sizeof(long long) <= sizeof(uint64), "");
static_assert(sizeof(double) <= sizeof(uint64), "");

static_assert(sizeof(void*) == sizeof(uint64), "");


#define HASH_SCALAR(Scalar) \
template<> struct Hash<Scalar> { constexpr static uint64 hash(Scalar s) { return static_cast<uint64>(s); } };

HASH_SCALAR(bool)
HASH_SCALAR(char)
HASH_SCALAR(signed char)
HASH_SCALAR(unsigned char)
HASH_SCALAR(char16_t)
HASH_SCALAR(char32_t)
HASH_SCALAR(wchar_t)
HASH_SCALAR(short)
HASH_SCALAR(unsigned short)
HASH_SCALAR(int)
HASH_SCALAR(unsigned int)
HASH_SCALAR(long)
HASH_SCALAR(long long)
HASH_SCALAR(unsigned long)
HASH_SCALAR(unsigned long long)

#undef HASH_SCALAR


template<>
struct Hash<float> {
	static uint64 hash(float f) {
		union {
			float f;
			uint64 i;
		} u;
		u.i = 0;
		u.f = f;
		return u.i;
	}
};


template<>
struct Hash<double> {
	static uint64 hash(double d) {
		union {
			double d;
			uint64 i;
		} u;
		u.i = 0;
		u.d = d;
		return u.i;
	}
};


// template<> struct Hash<long double>; -- Not Supported


template<class T>
struct Hash<T*> {
	constexpr static uint64 hash(T* tp) {
		return reinterpret_cast<uint64>(tp);
	}
};


template <typename T>
uint64 hash(T t) { return Hash<T>::hash(t); }


} // ns stardazed

#endif
