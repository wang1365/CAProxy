/** \file Base64.h
 **	\date  2004-02-13
 **	\author grymse@alhem.net
**/
/*
Copyright (C) 2004-2006  Anders Hedstrom

This library is made available under the terms of the GNU GPL.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about 
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef _BASE64_H
#define _BASE64_H

#ifdef _WIN32
#pragma warning(disable:4514)
#endif

#include "StdAfx.h"
#include <stdio.h>
#include <string>

using namespace std;
/** \defgroup util Utilities */

/** Base64 encode/decode. 
	\ingroup util */

#define B64_ENCODE_LENGTH(l)	(((l+2)/3*4)+(l/48+1)*2+80)
#define B64_DECODE_LENGTH(l)	((l+3)/4*3+80)

class  Base64
{
private:

	typedef struct b64_Encode_Ctx_st
	{
		int num;	/* number saved in a partial encode/decode */
		int length;	/* The length is either the output line length
					* (in input bytes) or the shortest input line
					* length that is ok.  Once decoding begins,
					* the length is adjusted up each time a longer
					* line is decoded */
		unsigned char enc_data[80];	/* data to encode */
		int line_num;	/* number read on current line */
		int expect_nl;
	} B64_ENCODE_CTX;

	B64_ENCODE_CTX* m_pctx;
	std::string m_sData;

public:
	static inline bool is_base64(unsigned char c) {
		return (isalnum(c) || (c == '+') || (c == '/'));};
	std::string base64_encode(unsigned char const* , unsigned int len);
	inline std::string base64_encode(std::string const& s)
	{ return base64_encode((const unsigned char *)s.c_str(), (unsigned int)s.length());};
	inline std::string base64_decode(std::string const& s)
	{ return base64_decode((const unsigned char *)s.c_str(), (unsigned int)s.length());};

	std::string base64_decode(unsigned char const* , unsigned int len);


	//-----------------------

	void B64_EncodeInit();
	void B64_EncodeUpdate( /*unsigned char *out, int *outl,*/
		const unsigned char *in, int inl);
	std::string B64_EncodeFinal(/*unsigned char *out, int *outl*/);
	int B64_EncodeBlock(/*unsigned char *t, */const unsigned char *f, int dlen);
	void B64_DecodeInit();
	int B64_DecodeUpdate( /*unsigned char *out, int *outl,*/
		const unsigned char *in, int inl);
	int B64_DecodeBlock(/*unsigned char *t, */const unsigned char *f, int n);
	std::string B64_DecodeFinal(/*unsigned char *out, int *outl*/);

	std::string B64_Encode(const unsigned char *f, int dlen);
	std::string B64_Decode(const unsigned char *f, int dlen);


	Base64();
	~Base64();
};

#endif // _BASE64_H
