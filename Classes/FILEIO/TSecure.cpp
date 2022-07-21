#include "TSecure.h"

#include "md5/md5.h"

TString TSequre::getMD5( const char *data, int datalen )
{
	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16*2 + 1];
	int di;

	md5_init(&state);
	md5_append(&state, (md5_byte_t*) data, datalen);
	md5_finish(&state, digest);

	for (di = 0; di < 16; ++di)
		sprintf(hex_output + di * 2, "%02x", digest[di]);

	return hex_output;
}

TString TSequre::getMD5( const TString &data )
{
	return getMD5(data.getTextUTF8(), data.getSize());
}

