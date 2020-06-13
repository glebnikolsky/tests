// TestCRC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/crc.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <vector>


using namespace std;

boost::random::mt19937 gen;
const int k8 = 1024*8;
unsigned char halfbyte[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
unsigned char bitbyte[256];
unsigned long bitmask[32];





struct Bitcount{
	void Init(){
		for(int i =0; i< 256; ++i) bitbyte[i] = halfbyte[(i>>4) & 0xf] + halfbyte[i & 0xf];
		for(int i=0; i < 32; ++i) bitmask[i] = 1<<i;
	}
	size_t Bits(void *buf, size_t bsize){
		unsigned char *ch = (unsigned char *)buf;
		size_t bcount = 0;
		while(bsize--) bcount += bitbyte[*ch++];
		return bcount;
	}
};

struct CountColl{
	CountColl() { 
		bits_ = new unsigned long[2048];
		if ( bits_) for( int i =0; i < 2048; ++i) bits_[i] = 0;
	}

	~CountColl() {
		if ( bits_ ) delete[] bits_;
	}

	bool Valid() {
		return bits_ != NULL;
	}

	void operator()(unsigned long pos){
		bits_[pos >> 5] |= bitmask[pos & 0x1f];
	}

	operator unsigned char*()
	{ 
		return (unsigned char*)bits_; 
	}

	unsigned long *bits_;
};


template<typename T>struct TestData{
	TestData(T n)
	{	
		vector<T> tmp(256/sizeof(T),n); 
		swap(data_,tmp);
	}

	TestData()
	{	
		vector<T> tmp(256/sizeof(T)); 
		long long n = ((long long)1)<<(sizeof(T)*8);
	    boost::random::uniform_int_distribution<long long> dist(0, n);
		for(size_t i = 0; i < tmp.size(); ++i) tmp[i] = (T)dist(gen);

		swap(data_,tmp);
	}
	operator unsigned char*()
	{ 
		return (unsigned char*)&data_[0]; 
	}

	vector<T> data_;

};

union L2S{
	unsigned long l_;
	unsigned short s_[2];
};

int _tmain(int argc, _TCHAR* argv[])
{
	{
		boost::crc_32_type  computer;
		computer.reset();
		computer.process_bytes("1",1);
		std::cout << computer.checksum();
	}


	Bitcount b;
	b.Init();
	{
		CountColl crc16;
		for(unsigned long i = 0; i < 0x10000; ++i){
			TestData<unsigned short> dt((unsigned short)i);
			boost::crc_ccitt_type computer;
			computer.reset();
			computer.process_bytes((unsigned char*)dt, 256);
			crc16( computer.checksum());
		}
		unsigned int n16 = b.Bits((unsigned char*)crc16, 2048*4);
		cout<<"crc16 linear\t"<<n16<<'\t'<<0x10000-n16<<endl;
	}
	{
		CountColl crc16;
		for(unsigned long i = 0; i < ((unsigned long)1)<<16; ++i){
			TestData<unsigned short> dt;
			boost::crc_ccitt_type computer;
			computer.reset();
			computer.process_bytes((unsigned char*)dt, 256);
			crc16( computer.checksum());
		}
		unsigned int n16 = b.Bits((unsigned char*)crc16, 2048*4);
		cout<<"crc16 random\t"<<n16<<'\t'<<0x10000-n16<<endl;
	}
	{
		vector<CountColl> crc32(0x10000);
		int bad=0;
		for(int i=0; i < 0x10000; ++i)	bad += crc32[i].Valid() ? 0 : 1;
		cout<<bad<<endl;
		for(long long bg =0, e = 0x100000; bg < 0x100000000; bg+= 0x100000, e+= 0x100000){
			for(long long i = bg; i <e; ++i){
				TestData<unsigned long> dt((unsigned long)i);
				boost::crc_32_type  computer;
				computer.reset();
				computer.process_bytes((unsigned char*)dt, 256);
				L2S n;
				n.l_ = computer.checksum();
				crc32[n.s_[1]](n.s_[0]);
			}
			cerr<<'\r'<<hex<<e;
		}
		unsigned long coll=0;
		for(int i = 0; i < 0x10000; ++i)
			coll += b.Bits((unsigned char*)crc32[i],2048*4);
		cout<<"\nlinear crc32\t"<<coll<<'\t'<<0x100000000-coll<<endl;
	}
	{
		vector<CountColl> crc32(0x10000);
		int bad=0;
		for(int i=0; i < 0x10000; ++i)	bad += crc32[i].Valid() ? 0 : 1;
		cout<<bad<<endl;

		for(long long bg =0, e = 0x100000; bg < 0x100000000; bg+= 0x100000, e+= 0x100000){
			for(long long i = bg; i <e; ++i){
				TestData<unsigned long> dt;
				boost::crc_32_type  computer;
				computer.reset();
				computer.process_bytes((unsigned char*)dt, 256);
				L2S n;
				n.l_ = computer.checksum();
				crc32[n.s_[1]](n.s_[0]);
			}
			cerr<<'\r'<<hex<<e;
		}
		unsigned long coll=0;
		for(int i = 0; i < 0x10000; ++i)
			coll += b.Bits((unsigned char*)crc32[i],2048*4);
		cout<<"\nrandom crc32\t"<<coll<<'\t'<<0x100000000-coll<<endl;
	}
	return 0;
}

