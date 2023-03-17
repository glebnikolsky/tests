/*!
*   \file md5.h
*/
/***************************************************************************
 *            md5.h
 *  Description: MD5 encryption algorythm
 *  Version: 0.1.1.2
 *  Author: Bliudov A.V.
 *  Wed Nov 2 13:37:14 2011
 *  Organization: (с) 2011 - 2014 Федеральный центр тестирования
 *
 *  Testing conditions:
 *  MD5("md5") has 1bc29b36 f623ba82 aaf6724f d3b16718
 *  MD5("md4") has c93d3bf7 a7c4afe9 4b64e30c 2ce39f4f
 *  MD5("")    has d41d8cd9 8f00b204 e9800998 ecf8427e
 *
 ****************************************************************************/
#ifndef _MD5_H_
#define _MD5_H_

#include <windows.h>
/*! \struct MD5_CTX*/
typedef struct{
    unsigned long total[2];     /*!< number of bytes processed  */
    unsigned long state[4];     /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */

    unsigned char ipad[64];     /*!< HMAC: inner padding        */
    unsigned char opad[64];     /*!< HMAC: outer padding        */
} MD5_CTX;

class md5{
    private:
        MD5_CTX * ctx;
	protected:
		static const unsigned long md5_init_state0 = 0x67452301;
		static const unsigned long md5_init_state1 = 0xefcdab89;
		static const unsigned long md5_init_state2 = 0x98badcfe;
		static const unsigned long md5_init_state3 = 0x10325476;
		static const unsigned char padding[64];
        /*! \brief ?
        *   \param output ?
        *   \param input ?
        *   \param length
        */
		void to_bytes(unsigned char* output, unsigned long input, unsigned int length );
		/*! \brief ?
        */
        /*! \brief to_long_le
        *   \param input ?
        *   \param length ?
        *   \returns unsigned long
        */
        unsigned long to_long_le(unsigned char * input, unsigned int length );
	public:
		/*! \brief ?
        *   \param ctx ?
        */
        void init(MD5_CTX * ctx);
		/*! \brief ?
        *   \param ctx ?
        *   \param data[64] ?
        */
        void transform( MD5_CTX *ctx, unsigned char data[64] );
        /*! \brief ?
        *   \param ctx ?
        *   \param buffer ?
        *   \param len ?
        */
        void update(MD5_CTX *ctx, const void *buffer, unsigned long len);
        /*! \brief ?
        *   \param ctx ?
        *   \param result ?
        */
        void final(MD5_CTX *ctx, char *result);
        /*! \brief ?
        *   \param buffer ?
        *   \param length ?
        *   \param md5_hash ?
        */
		void get_md5( char* buffer, unsigned long length, char* md5_hash );
		/*! \brief конструктор*/
        md5();
        /*! \brief деструктор*/
		~md5();
};

#endif /* _MD5_H_ */
