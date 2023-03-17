/************************************************************/
#include "..\Inc\StdAfx.h"
#include "..\Inc\FtcUtil.h"
#include "..\Inc\md5.h"

#include <string.h>

const unsigned char md5::padding[64] = {
												  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
												  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  
												};
md5::md5(){

}
#define GET_ULONG_LE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ]       )        \
        | ( (unsigned long) (b)[(i) + 1] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 2] << 16 )        \
        | ( (unsigned long) (b)[(i) + 3] << 24 );       \
}

#define PUT_ULONG_LE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n)       );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 3] = (unsigned char) ( (n) >> 24 );       \
}


void md5::to_bytes(unsigned char* output, unsigned long input, unsigned int length ){
	for ( unsigned int j = 0; j < 4; j++ ) output[length + j] = (unsigned char)((input >> 8*j) & 0xff);  
	return;
}  

unsigned long md5::to_long_le(unsigned char * input, unsigned int length ){
	unsigned long rzt = (unsigned long)input[length];
	for( unsigned int i = 1; i < 4; i++ ) rzt |= (unsigned long) ((input)[(length) + i] <<  (8*i) );
	return rzt;
}  


void md5::init( MD5_CTX * ctx ){
	ctx->total[0] = 0;
	ctx->total[1] = 0;
	ctx->state[0] = md5::md5_init_state0;
	ctx->state[1] = md5::md5_init_state1;
	ctx->state[2] = md5::md5_init_state2;
	ctx->state[3] = md5::md5_init_state3;
	return;
}

void md5::transform( MD5_CTX *ctx, unsigned char data[64] ){
    unsigned long X[16];//, A, B, C, D;

	for (int i = 0; i < 64; i+= 4 ){
		X[ i/4] = this->to_long_le( data,  i );
	}

	class math_data{
	private:
		unsigned long A;
		unsigned long B;
		unsigned long C;
		unsigned long D;
		unsigned long * X;
	public:
		math_data(unsigned long x[16], unsigned long a, unsigned long b, unsigned long c, unsigned long d){
			this->X = x;
			this->A = a;
			this->B = b;
			this->C = c;
			this->D = d;
		}

		unsigned long Sxn( unsigned long x, unsigned long n ){
			return ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)));
		}
		unsigned long F1( unsigned long x, unsigned long y, unsigned long z ){
			return (z ^ (x & (y ^ z)));
//			return (((x) & (y)) | ((~x) & (z)));
		}
		unsigned long P1(	unsigned long a,
							unsigned long b,
							unsigned long c,
							unsigned long d,
							unsigned long pos,
							unsigned long s,
							unsigned long cnst ){
			unsigned long ret = a;
			ret += F1(b,c,d) + X[pos] + cnst;
			ret = Sxn(ret, s) + b;
			return ret;
		}
		void Round1(){
			this->A = this->P1( this->A, this->B, this->C, this->D,  0,  7, 0xD76AA478 );
			this->D = this->P1( this->D, this->A, this->B, this->C,  1, 12, 0xE8C7B756 );
			this->C = this->P1( this->C, this->D, this->A, this->B,  2, 17, 0x242070DB );
			this->B = this->P1( this->B, this->C, this->D, this->A,  3, 22, 0xC1BDCEEE );
			this->A = this->P1( this->A, this->B, this->C, this->D,  4,  7, 0xF57C0FAF );
			this->D = this->P1( this->D, this->A, this->B, this->C,  5, 12, 0x4787C62A );
			this->C = this->P1( this->C, this->D, this->A, this->B,  6, 17, 0xA8304613 );
			this->B = this->P1( this->B, this->C, this->D, this->A,  7, 22, 0xFD469501 );
			this->A = this->P1( this->A, this->B, this->C, this->D,  8,  7, 0x698098D8 );
			this->D = this->P1( this->D, this->A, this->B, this->C,  9, 12, 0x8B44F7AF );
			this->C = this->P1( this->C, this->D, this->A, this->B, 10, 17, 0xFFFF5BB1 );
			this->B = this->P1( this->B, this->C, this->D, this->A, 11, 22, 0x895CD7BE );
			this->A = this->P1( this->A, this->B, this->C, this->D, 12,  7, 0x6B901122 );
			this->D = this->P1( this->D, this->A, this->B, this->C, 13, 12, 0xFD987193 );
			this->C = this->P1( this->C, this->D, this->A, this->B, 14, 17, 0xA679438E );
			this->B = this->P1( this->B, this->C, this->D, this->A, 15, 22, 0x49B40821 );
			unsigned long a = this->A;
			unsigned long b = this->B;
			unsigned long c = this->C;
			unsigned long d = this->D;
			return;
		}
		unsigned long F2( unsigned long x, unsigned long y, unsigned long z ){
			return (y ^ (z & (x ^ y)));
//			return (((x) & (z)) | ((y) & (~z)));
		}
		unsigned long P2(	unsigned long a,
							unsigned long b,
							unsigned long c,
							unsigned long d,
							unsigned long pos,
							unsigned long s,
							unsigned long cnst ){
			unsigned long ret = a;
			ret += F2(b,c,d) + X[pos] + cnst;
			ret = Sxn(ret, s) + b;
			return ret;
		}

		void Round2(){
			this->A = P2( A, B, C, D,  1,  5, 0xF61E2562 );
			this->D = P2( D, A, B, C,  6,  9, 0xC040B340 );
			this->C = P2( C, D, A, B, 11, 14, 0x265E5A51 );
			this->B = P2( B, C, D, A,  0, 20, 0xE9B6C7AA );
			this->A = P2( A, B, C, D,  5,  5, 0xD62F105D );
			this->D = P2( D, A, B, C, 10,  9, 0x02441453 );
			this->C = P2( C, D, A, B, 15, 14, 0xD8A1E681 );
			this->B = P2( B, C, D, A,  4, 20, 0xE7D3FBC8 );
			this->A = P2( A, B, C, D,  9,  5, 0x21E1CDE6 );
			this->D = P2( D, A, B, C, 14,  9, 0xC33707D6 );
			this->C = P2( C, D, A, B,  3, 14, 0xF4D50D87 );
			this->B = P2( B, C, D, A,  8, 20, 0x455A14ED );
			this->A = P2( A, B, C, D, 13,  5, 0xA9E3E905 );
			this->D = P2( D, A, B, C,  2,  9, 0xFCEFA3F8 );
			this->C = P2( C, D, A, B,  7, 14, 0x676F02D9 );
			this->B = P2( B, C, D, A, 12, 20, 0x8D2A4C8A );
		}
		unsigned long F3( unsigned long x, unsigned long y, unsigned long z ){
			return (x ^ y ^ z);
//			return ((x) ^ (y) ^ (z));
		}
		unsigned long P3(	unsigned long a,
							unsigned long b,
							unsigned long c,
							unsigned long d,
							unsigned long pos,
							unsigned long s,
							unsigned long cnst ){
			unsigned long ret = a;
			ret += F3(b,c,d) + X[pos] + cnst;
			ret = Sxn(ret, s) + b;
			return ret;
		}

		void Round3(){
			this->A = P3( A, B, C, D,  5,  4, 0xFFFA3942 );
			this->D = P3( D, A, B, C,  8, 11, 0x8771F681 );
			this->C = P3( C, D, A, B, 11, 16, 0x6D9D6122 );
			this->B = P3( B, C, D, A, 14, 23, 0xFDE5380C );
			this->A = P3( A, B, C, D,  1,  4, 0xA4BEEA44 );
			this->D = P3( D, A, B, C,  4, 11, 0x4BDECFA9 );
			this->C = P3( C, D, A, B,  7, 16, 0xF6BB4B60 );
			this->B = P3( B, C, D, A, 10, 23, 0xBEBFBC70 );
			this->A = P3( A, B, C, D, 13,  4, 0x289B7EC6 );
			this->D = P3( D, A, B, C,  0, 11, 0xEAA127FA );
			this->C = P3( C, D, A, B,  3, 16, 0xD4EF3085 );
			this->B = P3( B, C, D, A,  6, 23, 0x04881D05 );
			this->A = P3( A, B, C, D,  9,  4, 0xD9D4D039 );
			this->D = P3( D, A, B, C, 12, 11, 0xE6DB99E5 );
			this->C = P3( C, D, A, B, 15, 16, 0x1FA27CF8 );
			this->B = P3( B, C, D, A,  2, 23, 0xC4AC5665 );
		}
		unsigned long F4( unsigned long x, unsigned long y, unsigned long z ){
			return (y ^ (x | ~z));
//			return ((y) ^ ((x) | (~z)));
		}
		unsigned long P4(	unsigned long a,
							unsigned long b,
							unsigned long c,
							unsigned long d,
							unsigned long pos,
							unsigned long s,
							unsigned long cnst ){
			unsigned long ret = a;
			ret += F4(b,c,d) + X[pos] + cnst;
			ret = Sxn(ret, s) + b;
			return ret;
		}

		void Round4(){
			this->A = P4( A, B, C, D,  0,  6, 0xF4292244 );
			this->D = P4( D, A, B, C,  7, 10, 0x432AFF97 );
			this->C = P4( C, D, A, B, 14, 15, 0xAB9423A7 );
			this->B = P4( B, C, D, A,  5, 21, 0xFC93A039 );
			this->A = P4( A, B, C, D, 12,  6, 0x655B59C3 );
			this->D = P4( D, A, B, C,  3, 10, 0x8F0CCC92 );
			this->C = P4( C, D, A, B, 10, 15, 0xFFEFF47D );
			this->B = P4( B, C, D, A,  1, 21, 0x85845DD1 );
			this->A = P4( A, B, C, D,  8,  6, 0x6FA87E4F );
			this->D = P4( D, A, B, C, 15, 10, 0xFE2CE6E0 );
			this->C = P4( C, D, A, B,  6, 15, 0xA3014314 );
			this->B = P4( B, C, D, A, 13, 21, 0x4E0811A1 );
			this->A = P4( A, B, C, D,  4,  6, 0xF7537E82 );
			this->D = P4( D, A, B, C, 11, 10, 0xBD3AF235 );
			this->C = P4( C, D, A, B,  2, 15, 0x2AD7D2BB );
			this->B = P4( B, C, D, A,  9, 21, 0xEB86D391 );
		}
		void calculate(){
			this->Round1();
			this->Round2();
			this->Round3();
			this->Round4();
		}
		unsigned long getA(){ return this->A; }
		unsigned long getB(){ return this->B; }
		unsigned long getC(){ return this->C; }
		unsigned long getD(){ return this->D; }
	};

	math_data mt( X, ctx->state[0], ctx->state[1], ctx->state[2], ctx->state[3]);
	mt.calculate();

    ctx->state[0] += mt.getA();
    ctx->state[1] += mt.getB();
    ctx->state[2] += mt.getC();
    ctx->state[3] += mt.getD();
	return;
}

void md5::update(MD5_CTX *ctx, const void *buffer, unsigned long len){
    unsigned int fill;
    unsigned long left;
	unsigned char * input = (unsigned char *)buffer;

	if( len > 0 ){
		left = ctx->total[0] & 0x3F;
		fill = 64 - left;

		ctx->total[0] += len;
		ctx->total[0] &= 0xFFFFFFFF;

		if( ctx->total[0] < (unsigned long) len ) ctx->total[1]++;

		if( left && len >= fill ){
			memcpy( (void *) (ctx->buffer + left),
					(void *) buffer, fill );
			this->transform( ctx, ctx->buffer );
			input += fill;
			len  -= fill;
			left = 0;
		}

		while( len >= 64 )
		{
			this->transform( ctx, input );
			input += 64;
			len  -= 64;
		}

		if( len > 0 ){
			memcpy( (void *) (ctx->buffer + left),
					(void *) buffer, len );
		}
	}
	return;
}

void md5::final( MD5_CTX *ctx, char * out_md5_hash ){
    unsigned long last, padn;
    unsigned long high, low;
    unsigned char msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

	this->to_bytes( msglen, low, 0 );
	this->to_bytes( msglen, high, 4 );


    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

	this->update( ctx, (unsigned char *) md5::padding, padn );
    this->update( ctx, msglen, 8 );
	for( unsigned int i = 0; i < 4; i++ ){
		this->to_bytes( (unsigned char *)out_md5_hash,  ctx->state[i], 4*i );
	}
}

md5::~md5(){

}
/************************************************************/
