// ------------------------------------------------------------------
// StringFormat.cpp - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#include "StringFormat.hpp"
#include <ostream>

namespace stardazed {


std::string toString(const bool b) {
	const static std::string TRUE_STRING { "true" }, FALSE_STRING { "false" };
	return b ? TRUE_STRING : FALSE_STRING;
}

std::string toString(const char* cs) {
	return cs;
}

std::string toString(std::string s) {
	return s;
}



//  _____          _   ____  _        _
// |  ___| __ ___ | |_/ ___|| |_ _ __(_)_ __   __ _
// | |_ | '_ ` _ \| __\___ \| __| '__| | '_ \ / _` |
// |  _|| | | | | | |_ ___) | |_| |  | | | | | (_| |
// |_|  |_| |_| |_|\__|____/ \__|_|  |_|_| |_|\__, |
//                                            |___/

FmtString::FmtString(std::string s) : fmt_{s} {
	scan_ = std::find(fmt_.begin(), fmt_.end(), '%');

	if (scan_ != fmt_.end()) {
		baked_.reserve(fmt_.size());
		std::copy(fmt_.cbegin(), scan_, std::back_inserter(baked_));
	}
	else
		baked_ = fmt_;
}


void FmtString::appendValue(const std::string& s) {
	if (scan_ == fmt_.end())
		return; // warning?
	
	auto ins = std::back_inserter(baked_);
	
	std::copy(s.cbegin(), s.cend(), ins);
	auto next = std::find(scan_ + 2, fmt_.cend(), '%');
	std::copy(scan_ + 2, next, ins);
	
	scan_ = next;
}


std::ostream& operator <<(std::ostream& os, const FmtString& fs) {
	os << fs.str();
	return os;
}



} // ns stardazed
