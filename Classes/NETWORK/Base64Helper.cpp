#include "Base64Helper.h"

struct BASE64_DEC_TABLE {
	signed char n[256];

	BASE64_DEC_TABLE() {
		for(int i=0; i<256; ++i)    n[i] = -1;
		for(unsigned char i='0'; i<='9'; ++i) n[i] = 52+i-'0';
		for(unsigned char i='A'; i<='Z'; ++i) n[i] = i-'A';
		for(unsigned char i='a'; i<='z'; ++i) n[i] = 26+i-'a';
		n['+'] = 62;
		n['/'] = 63;
	}
	int operator [] (unsigned char i) const { return n[i]; }
};

static const BASE64_DEC_TABLE b64DecTable;
static const char b64EncTable[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

int Base64DecodeGetRequiredLength(int n) {
	return  n * 4 / 3;
}

int Base64Decode(const char* src, const int src_len, void* dst, int *dst_len) {
	if (!dst_len) return 0;
	int bc=0, a=0;
	char* const pstart = static_cast<char*>(dst);
	char* pd = pstart;
	char* const pend = pd + *dst_len;
	for(int i=0; i<src_len; ++i) {
		const int n = b64DecTable[src[i]];
		if (n == -1) continue;
		a |= (n & 63) << (18 - bc);
		if ((bc += 6) > 18) {
			*pd = a >> 16; if (++pd >= pend) return pd - pstart;
			*pd = a >> 8;  if (++pd >= pend) return pd - pstart;
			*pd = a;       if (++pd >= pend) return pd - pstart;
			bc = a = 0;
		}    }
	if (bc >= 8) {
		*pd = a >> 16; if (++pd >= pend) return pd - pstart;
		if (bc >= 16) *(pd++) = a >> 8;
	}

	int len = pd - pstart;

	*dst_len = len;

	return len;
}

int Base64EncodeGetRequiredLength(int n) {
	return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int Base64Encode(unsigned char const* src, int src_len, char* dst, int *dst_len) {
	int i = 0;
	int j = 0;
	int dst_pos = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (src_len--) {
		char_array_3[i++] = *(src++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				dst[dst_pos++] = b64EncTable[char_array_4[i]];
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
			dst[dst_pos++] = b64EncTable[char_array_4[j]];

		while((i++ < 3))
			dst[dst_pos++] = '=';

	}

	*dst_len = dst_pos;

	return dst_pos;
}