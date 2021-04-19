#ifndef RANGE_H
#pragma once


Skip - набор диапазонов
Range - набор диапазонов

На кажом шагу формируем Range


struct Skip {
	Skip(const long l, const long r):l_(l),r_(r){}
	bool operator %(const long n) {
		return n == l_;
	}
	long SkipToZ(long &n) {
		return n = r_ + 1;
	}
	long l_;
	long r_;
};


#endif RANGE_H
