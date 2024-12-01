// tESTS.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <set>
#include <tuple>
#include <vector>
#include <memory>
#include <boost/crc.hpp>


// Абстрактный базовый класс Shape
class Shape {
public:
	virtual ~Shape() = 0; // Чисто виртуальный деструктор
	virtual double Area() const = 0;
	virtual double Perimeter() const = 0;
};

Shape::~Shape() {}

// Класс Circle, наследующий от Shape
class Circle: public Shape {
public:
	Circle( double radius ): radius_( radius ) {}
	virtual double Area() const override {
		return 3.14 * radius_ * radius_;
	}
	virtual double Perimeter() const override {
		return 2 * 3.14 * radius_;
	}
private:
	double radius_;
};

// Класс Rectangle, наследующий от Shape
class Rectangle: public Shape {
public:
	Rectangle( double width, double height ): width_( width ), height_( height ) {}
	virtual double Area() const override {
		return width_ * height_;
	}
	virtual double Perimeter() const override {
		return 2 * (width_ + height_);
	}
private:
	double width_, height_;
};

// Класс Triangle, наследующий от Shape
class Triangle: public Shape {
public:
	Triangle( double a, double b, double c ): a_( a ), b_( b ), c_( c ) {}
	virtual double Area() const override {
		double s = Perimeter() / 2;
		return std::sqrt( s * (s - a_) * (s - b_) * (s - c_) );
	}
	virtual double Perimeter() const override {
		return a_ + b_ + c_;
	}
private:
	double a_, b_, c_;
};

// Функция для вывода информации о фигуре
void PrintShapeInfo( const Shape* shape ) {
	std::cout << "Area: " << shape->Area() << std::endl;
	std::cout << "Perimeter: " << shape->Perimeter() << std::endl;
}

std::string CreateBar( int year, int type, long long no )
{
	std::string res( 13, '0' );
	res[0] += 2;
	res[1] += year;
	res[2] += type;
	int crc{2 + 3*year + type};
	for ( int i{0}, j{11}; i < 9 && no; ++i, --j, no /= 10 ) {
		int rest = no % 10;
		res[j] += rest;
		crc += i % 2 ? rest : 3 * rest;
	}
	res[12] += crc % 10 ? 10 - (crc % 10 ) : 0;
	return res;
}


long long CreateBarll( int year, int type, long long no )
{
	int crc{2 + 3 * year + type};
	long long barcode = 2 * 1'000'000'000'000ll + year * 100'000'000'000ll + type * 10'000'000'000ll + no * 10ll;
	for ( int i{0}; i < 9 && no; ++i, no /= 10 ) crc += i % 2 ? no % 10 : 3 * (no % 10);
	barcode += crc % 10 ? 10 - (crc % 10) : 0;
	return barcode;
}

unsigned long MakeSeed( std::string &&d, std::string &&b )
{
	std::string seed = d + b;
	boost::crc_32_type  computer;
	computer.reset();
	computer.process_bytes( seed.c_str(), seed.length() );
	return computer.checksum();
}



bool AdjustNo( long &no, std::set<std::tuple<long, long>> &exc )
{
	using namespace std;
	auto f = exc.begin();
	long shift{0};
	if ( get<0>( *f ) == 1 ){
		no += get<1>( *f ) - get<0>( *f ) + 1;
	}
	else {
		exc.insert( make_tuple( 0l, 0l ) );
		f = exc.begin();
	}
	exc.insert( make_tuple( 100l, 100l ) );
	auto s{f};
	++s;
	while ( s != exc.end() && !(no > get<1>( *f ) && no < get<0>( *s )) ){
		++f;
		++s;
		no += get<1>( *f ) - get<0>( *f ) + 1;
	}
	return s != exc.end();
}


// Главная функция
int main() {
	using namespace std;
	//
	
	
	
	
	
	
	
	
	
	
	std::set<std::tuple<long, long>> exc{{11,20},{31,40}}, exc1{{1,10},{21,30},{41,50}};
	for ( auto n : {5,11,21,35,71} ) {
		long no = n;
		cout << no << '\t';
		cout << AdjustNo( no, exc ) << '\t';
		cout << no << '\n';
	}
	exc.swap( exc1 );
	for ( auto n : {5,11,21,35,71} ) {
		long no = n;
		cout << no << '\t';
		cout << AdjustNo( no, exc ) << '\t';
		cout << no << '\n';
	}

	cout << MakeSeed( _T( "ERBD_MAIN_2024" ), _T( "BARCODE_R" ) ) << '\t' << MakeSeed( _T( "ERBD_MAIN_2024" ), _T( "BARCODE_AB" ) ) << '\t' << MakeSeed( _T( "ERBD_MAIN_2024" ), _T( "BARCODE_C" ) ) << '\t' << MakeSeed( _T( "ERBD_MAIN_2024" ), _T( "BARCODE_C2" ) ) << '\n';
	std::mt19937 gen;
	gen.seed( 1 );
	std::vector<long>v( 10 );
	std::iota( v.begin(), v.end(), 0 );
	std::copy( v.begin(), v.end(), ostream_iterator<long>( cout, "\t" ) );
	std::cout << '\n';
	shuffle( v.begin(), v.end(), gen );
	std::copy( v.begin(), v.end(), ostream_iterator<long>( cout, "\t" ) );
	std::cout << '\n';
	std::iota( v.begin(), v.end(), 0 );
	gen.seed( 1 );
	std::copy( v.begin(), v.end(), ostream_iterator<long>( cout, "\t" ) );
	std::cout << '\n';
	shuffle( v.begin(), v.end(), gen );
	std::copy( v.begin(), v.end(), ostream_iterator<long>( cout, "\t" ) );
	std::cout << '\n';




	std::vector<std::unique_ptr<Shape>> shapes;
	shapes.push_back( std::make_unique<Circle>( 5.0 ) );
	shapes.push_back( std::make_unique<Rectangle>( 4.0, 6.0 ) );
	shapes.push_back( std::make_unique<Triangle>( 3.0, 4.0, 5.0 ) );
	std::cout << 2430004131314 << ' ' << CreateBar( 4, 3, 413131 ) << ' ' << CreateBarll( 4, 3, 413131 ) << '\n';
	std::cout << 2410003828293 << ' ' << CreateBar( 4, 1, 382829 ) << ' ' << CreateBarll( 4, 1, 382829 ) << '\n';
	std::cout << 2420003827141 << ' ' << CreateBar( 4, 2, 382714 ) << ' ' << CreateBarll( 4, 2, 382714 ) << '\n';
	std::cout << 2440001174498 << ' ' << CreateBar( 4, 4, 117449 ) << ' ' << CreateBarll( 4, 4, 117449 ) << '\n';
	std::cout << (2+3*4 +4 +3*9 +9 + 3*9 +9 +3*9 + 9+ 3*9 + 9 +3*9) %10 << ' ' << CreateBar( 4, 4, 999999999 ) << ' ' << CreateBarll( 4, 4, 999999999 ) << '\n';
	for ( const auto& shape : shapes ) {
		PrintShapeInfo( shape.get() );
	}

	return 0;
}
