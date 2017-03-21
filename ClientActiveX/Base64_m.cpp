#include "StdAfx.h"
#include "Base64_m.h"
//#include <iostream>
//#include <ctype.h>

static const std::string base64_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


std::string Base64::base64_decode(unsigned char const* encoded_string, unsigned int in_len)
{
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;
	
	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			
			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}
	
	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;
		
		for (j = 0; j <4; j++)
			char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);
		
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	
	return ret;
}

std::string Base64::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	
	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			
			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	
	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';
		
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		
		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];
		
		while((i++ < 3))
			ret += '=';
		
	}
	
	return ret;
}

#ifndef CHARSET_EBCDIC
#define conv_bin2ascii(a)	(data_bin2ascii[(a)&0x3f])
#define conv_ascii2bin(a)	(data_ascii2bin[(a)&0x7f])
#else
/* We assume that PEM encoded files are EBCDIC files
* (i.e., printable text files). Convert them here while decoding.
* When encoding, output is EBCDIC (text) format again.
* (No need for conversion in the conv_bin2ascii macro, as the
* underlying textstring data_bin2ascii[] is already EBCDIC)
*/
#define conv_bin2ascii(a)	(data_bin2ascii[(a)&0x3f])
#define conv_ascii2bin(a)	(data_ascii2bin[os_toascii[a]&0x7f])
#endif

/* 64 char lines
* pad input with 0
* left over chars are set to =
* 1 byte  => xx==
* 2 bytes => xxx=
* 3 bytes => xxxx
*/
#define BIN_PER_LINE    (64/4*3)
#define CHUNKS_PER_LINE (64/4)
#define CHAR_PER_LINE   (64+1)

static unsigned char data_bin2ascii[65]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* 0xF0 is a EOLN
* 0xF1 is ignore but next needs to be 0xF0 (for \r\n processing).
* 0xF2 is EOF
* 0xE0 is ignore at start of line.
* 0xFF is error
*/

#define B64_EOLN		0xF0
#define B64_CR			0xF1
#define B64_EOF			0xF2
#define B64_WS			0xE0
#define B64_ERROR       	0xFF
#define B64_NOT_BASE64(a)	(((a)|0x13) == 0xF3)

static unsigned char data_ascii2bin[128]={
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xE0,0xF0,0xFF,0xFF,0xF1,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xE0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x3E,0xFF,0xF2,0xFF,0x3F,
	0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,
	0x3C,0x3D,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,
	0xFF,0x00,0x01,0x02,0x03,0x04,0x05,0x06,
	0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,
	0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
	0x31,0x32,0x33,0xFF,0xFF,0xFF,0xFF,0xFF,
};

void Base64::B64_EncodeInit()
{
	m_pctx = new B64_ENCODE_CTX;
	m_pctx->length=48;
	m_pctx->num=0;
	m_pctx->line_num=0;
	m_sData = "";
}

void Base64::B64_EncodeUpdate(/*unsigned char *out, int *outl,*/
					  const unsigned char *in, int inl)
{
	int i;
	//unsigned int total=0;

	//*outl=0;
	if (inl == 0) return;
	//OPENSSL_assert(m_pctx->length <= (int)sizeof(m_pctx->enc_data));
	if ((m_pctx->num+inl) < m_pctx->length)
	{
		memcpy(&(m_pctx->enc_data[m_pctx->num]),in,inl);
		m_pctx->num+=inl;
		return;
	}
	if (m_pctx->num != 0)
	{
		i=m_pctx->length-m_pctx->num;
		memcpy(&(m_pctx->enc_data[m_pctx->num]),in,i);
		in+=i;
		inl-=i;
		B64_EncodeBlock(/*out,*/m_pctx->enc_data,m_pctx->length);
		m_pctx->num=0;
//		out+=j;
// 		*(out++)='\n';
// 		*out='\0';
// 		total=j+1;
	}
	while (inl >= m_pctx->length)
	{
		B64_EncodeBlock(in,m_pctx->length);
		in+=m_pctx->length;
		inl-=m_pctx->length;
// 		out+=j;
// 		*(out++)='\n';
// 		*out='\0';
// 		total+=j+1;
	}
	if (inl != 0)
		memcpy(&(m_pctx->enc_data[0]),in,inl);
	m_pctx->num=inl;
	//*outl=total;
}

string Base64::B64_EncodeFinal(/*unsigned char *out, int *outl*/)
{
	//unsigned int ret=0;

	if (m_pctx->num != 0)
	{
		/*ret=*/B64_EncodeBlock(/*out,*/m_pctx->enc_data,m_pctx->num);
// 		out[ret++]='\n';
// 		out[ret]='\0';
		m_pctx->num=0;
	}
	//*outl=ret;

	delete m_pctx;
	m_pctx = NULL;

	return m_sData;
}

int Base64::B64_EncodeBlock(/*unsigned char *t, */const unsigned char *f, int dlen)
{
	int i,ret=0;
	unsigned long l;

	for (i=dlen; i > 0; i-=3)
	{
		if (i >= 3)
		{
			l=	(((unsigned long)f[0])<<16L)|
				(((unsigned long)f[1])<< 8L)|f[2];

			m_sData += conv_bin2ascii(l>>18L);
			m_sData += conv_bin2ascii(l>>12L);
			m_sData += conv_bin2ascii(l>> 6L);
			m_sData += conv_bin2ascii(l     );
// 			*(t++)=conv_bin2ascii(l>>18L);
// 			*(t++)=conv_bin2ascii(l>>12L);
// 			*(t++)=conv_bin2ascii(l>> 6L);
// 			*(t++)=conv_bin2ascii(l     );
		}
		else
		{
			l=((unsigned long)f[0])<<16L;
			if (i == 2) l|=((unsigned long)f[1]<<8L);

			m_sData += conv_bin2ascii(l>>18L);
			m_sData += conv_bin2ascii(l>>12L);
			//*(t++)=conv_bin2ascii(l>>18L);
			//*(t++)=conv_bin2ascii(l>>12L);
			m_sData += (i == 1)?'=':conv_bin2ascii(l>> 6L);
			//*(t++)=(i == 1)?'=':conv_bin2ascii(l>> 6L);
			m_sData += '=';
			//*(t++)='=';
		}
		ret+=4;
		f+=3;
	}

	//*t='\0';
	return(ret);
}

string Base64::B64_Encode(const unsigned char *f, int dlen)
{
	B64_EncodeInit();
	B64_EncodeUpdate(f,dlen);
	return B64_EncodeFinal();
}

void Base64::B64_DecodeInit()
{
	m_pctx = new B64_ENCODE_CTX;
	m_pctx->length=30;
	m_pctx->num=0;
	m_pctx->line_num=0;
	m_pctx->expect_nl=0;

	m_sData = "";
}

/* -1 for error
*  0 for last line
*  1 for full line
*/
int Base64::B64_DecodeUpdate(/*unsigned char *out, int *outl,*/
					 const unsigned char *in, int inl)
{
	int seof= -1,eof=0,rv= -1,ret=0,i,v,tmp,n,ln,tmp2,exp_nl;
	unsigned char *d;

	n=m_pctx->num;
	d=m_pctx->enc_data;
	ln=m_pctx->line_num;
	exp_nl=m_pctx->expect_nl;

	/* last line of input. */
	if ((inl == 0) || ((n == 0) && (conv_ascii2bin(in[0]) == B64_EOF)))
	{ rv=0; goto end; }

	/* We parse the input data */
	for (i=0; i<inl; i++)
	{
		/* If the current line is > 80 characters, scream alot */
		if (ln >= 80) { rv= -1; goto end; }

		/* Get char and put it into the buffer */
		tmp= *(in++);
		v=conv_ascii2bin(tmp);
		/* only save the good data :-) */
		if (!B64_NOT_BASE64(v))
		{
			//OPENSSL_assert(n < (int)sizeof(m_pctx->enc_data));
			d[n++]=tmp;
			ln++;
		}
		else if (v == B64_ERROR)
		{
			rv= -1;
			goto end;
		}

		/* have we seen a '=' which is 'definitly' the last
		* input line.  seof will point to the character that
		* holds it. and eof will hold how many characters to
		* chop off. */
		if (tmp == '=')
		{
			if (seof == -1) seof=n;
			eof++;
		}

		if (v == B64_CR)
		{
			ln = 0;
			if (exp_nl)
				continue;
		}

		/* eoln */
		if (v == B64_EOLN)
		{
			ln=0;
			if (exp_nl)
			{
				exp_nl=0;
				continue;
			}
		}
		exp_nl=0;

		/* If we are at the end of input and it looks like a
		* line, process it. */
		if (((i+1) == inl) && (((n&3) == 0) || eof))
		{
			v=B64_EOF;
			/* In case things were given us in really small
			records (so two '=' were given in separate
			updates), eof may contain the incorrect number
			of ending bytes to skip, so let's redo the count */
			eof = 0;
			if (d[n-1] == '=') eof++;
			if (d[n-2] == '=') eof++;
			/* There will never be more than two '=' */
		}

		if ((v == B64_EOF && (n&3) == 0) || (n >= 64))
		{
			/* This is needed to work correctly on 64 byte input
			* lines.  We process the line and then need to
			* accept the '\n' */
			if ((v != B64_EOF) && (n >= 64)) exp_nl=1;
			tmp2=v;
			if (n > 0)
			{
				v=B64_DecodeBlock(/*out,*/d,n);
				n=0;
				if (v < 0) { rv=0; goto end; }
				ret+=(v-eof);
			}
			else
			{
				eof=1;
				v=0;
			}

			/* This is the case where we have had a short
			* but valid input line */
			if ((v < m_pctx->length) && eof)
			{
				rv=0;
				goto end;
			}
			else
				m_pctx->length=v;

			if (seof >= 0) { rv=0; goto end; }
			//out+=v;
		}
	}
	rv=1;
end:
	//*outl=ret;
	m_pctx->num=n;
	m_pctx->line_num=ln;
	m_pctx->expect_nl=exp_nl;
	return(rv);
}

int Base64::B64_DecodeBlock(/*unsigned char *t, */const unsigned char *f, int n)
{
	int i,ret=0,a,b,c,d;
	unsigned long l;

	/* trim white space from the start of the line. */
	while ((conv_ascii2bin(*f) == B64_WS) && (n > 0))
	{
		f++;
		n--;
	}

	/* strip off stuff at the end of the line
	* ascii2bin values B64_WS, B64_EOLN, B64_EOLN and B64_EOF */
	while ((n > 3) && (B64_NOT_BASE64(conv_ascii2bin(f[n-1]))))
		n--;

	if (n%4 != 0) return(-1);

	for (i=0; i<n; i+=4)
	{
		a=conv_ascii2bin(*(f++));
		b=conv_ascii2bin(*(f++));
		c=conv_ascii2bin(*(f++));
		d=conv_ascii2bin(*(f++));
		if (	(a & 0x80) || (b & 0x80) ||
			(c & 0x80) || (d & 0x80))
			return(-1);
		l=(	(((unsigned long)a)<<18L)|
			(((unsigned long)b)<<12L)|
			(((unsigned long)c)<< 6L)|
			(((unsigned long)d)     ));
		m_sData += (char)(l>>16L)&0xff;
		m_sData += (char)(l>> 8L)&0xff;
		m_sData += (char)(l     )&0xff;
// 		*(t++)=(unsigned char)(l>>16L)&0xff;
// 		*(t++)=(unsigned char)(l>> 8L)&0xff;
// 		*(t++)=(unsigned char)(l     )&0xff;
		ret+=3;
	}
	return(ret);
}

string Base64::B64_DecodeFinal(/*unsigned char *out, int *outl*/)
{
	int i;

	//*outl=0;
	if (m_pctx->num != 0)
	{
		i = B64_DecodeBlock(/*out,*/m_pctx->enc_data,m_pctx->num);
		if (i < 0) return "";
		m_pctx->num=0;
		//*outl=i;
		delete m_pctx;
		m_pctx = NULL;
		return m_sData;
	}
	else
	{
		delete m_pctx;
		m_pctx = NULL;
		return m_sData;
	}
}

string Base64::B64_Decode(const unsigned char *f, int dlen)
{
	B64_DecodeInit();
	int i = B64_DecodeBlock(f,dlen);
	//B64_DecodeUpdate(f,dlen);
	//if(i == -1)
	if(i <= 0)
		return "";
	return m_sData;//B64_DecodeFinal();
}

Base64::Base64()
{
	m_sData = "";
	m_pctx = NULL;
}

Base64::~Base64()
{
	if(m_pctx)
		delete m_pctx;
}
