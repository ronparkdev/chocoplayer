#include "ConvWAV.h"

#include <strstream>

int adapt_ms_adpcm_table [] = { 
	230, 230, 230, 230, 307, 409, 512, 614, 
	768, 614, 512, 409, 307, 230, 230, 230 
} ;

int adapt_ms_adpcm_coeff0 [] = { 256, 512, 0, 192, 240, 460, 392 } ;
int adapt_ms_adpcm_coeff1 [] = { 0, -256, 0, 64, 0, -208, -232 } ;


ConvWAV::ConvWAV()
{

}

int CEIL(float num) {
	int inum = (int)num;
	if (num == (float)inum) {
		return inum;
	}
	return inum + 1;
}

bool ConvWAV::ChkAndFix(char *data, int datalen, const std::string &dstpath) {

	int i, sample;
	int channelID = 0;
	int frameID = 0;
	std::istrstream is(data, datalen);
	std::ofstream os;

	mFileSize = datalen;
	
	mOffset = 0;

	if (mFileSize < 128) {
		os.close();
		return false;
	}

	char header[12];
	is.read(header, 12);
	mOffset += 12;
	if (header[0] != 'R' ||
		header[1] != 'I' ||
		header[2] != 'F' ||
		header[3] != 'F' ||
		header[8] != 'W' ||
		header[9] != 'A' ||
		header[10] != 'V' ||
		header[11] != 'E') {
			return false;
	}

	mChannels = 0;
	mSampleRate = 0;

	while (mOffset + 8 <= mFileSize) {
		char chunkHeader[8];
		is.read(chunkHeader, 8);
		mOffset += 8;

		int chunkPos = 0;
		int chunkLen =
			((0xff & chunkHeader[7]) << 24) |
			((0xff & chunkHeader[6]) << 16) |
			((0xff & chunkHeader[5]) << 8) |
			((0xff & chunkHeader[4]));

		if (chunkHeader[0] == 'f' &&
			chunkHeader[1] == 'm' &&
			chunkHeader[2] == 't' &&
			chunkHeader[3] == ' ') {
				if (chunkLen < 16 || chunkLen > 1024) {
					return false;
				}

				char fmt[16];
				is.read(fmt, 16);
				if (chunkLen > 16)
					is.seekg(chunkLen-16, is.cur);

				mOffset += chunkLen;

				mFormat =
					((0xff & fmt[1]) << 8) |
					((0xff & fmt[0]));
				mChannels =
					((0xff & fmt[3]) << 8) |
					((0xff & fmt[2]));
				mSampleRate =
					((0xff & fmt[7]) << 24) |
					((0xff & fmt[6]) << 16) |
					((0xff & fmt[5]) << 8) |
					((0xff & fmt[4]));
				mBitsPerSample =
					((0xff & fmt[15]) << 8) |
					((0xff & fmt[14]));
				mBlockAlign =
					((0xff & fmt[13]) << 8) |
					((0xff & fmt[12]));

				mBytePerSample = mBitsPerSample>>3;

				if (mBytePerSample == 0)
					mBytePerSample = 1;

				if (mFormat != 2) {
					return false;
				}

		} else if (chunkHeader[0] == 'd' &&
			chunkHeader[1] == 'a' &&
			chunkHeader[2] == 't' &&
			chunkHeader[3] == 'a') {
				if (mChannels == 0 || mSampleRate == 0) {
					return false;
				}

				os.open(dstpath.c_str(), std::ifstream::binary | std::ifstream::ate);

				if (!os.is_open())
					return false;

				mOffset += chunkLen;

				char oneFrame[32];
				if (mFormat == 1) {

					WriteHeader(os, chunkLen / mBytePerSample);

					while(mOffset < mFileSize) {
						is.read(oneFrame, mBytePerSample);

						mOffset += mBytePerSample;

						if ((oneFrame[mBytePerSample-1] & 0x80) == 0x80)
							memset((char*)(&sample), 0xFF, 4);
						else
							memset((char*)(&sample), 0x00, 4);

						memcpy((char*)(&sample), oneFrame, mBytePerSample);

						os.write((char*)(&sample), 2);

						if (channelID < mChannels-1) {
							channelID++;
						} else {
							channelID = 0;
							frameID++;
						}
					}
				} else if (mFormat == 2) {

					int blockLen = (int)(CEIL((double)chunkLen / mBlockAlign));

					WriteHeader(os, (chunkLen - blockLen * 7 * mChannels) * 2);

					int predictor[2];
					short delta[2];
					short history0[2];
					short history1[2];
					int nibble;

					while(chunkPos < chunkLen) {
						if (chunkPos % mBlockAlign == 0) {
							is.read(oneFrame, 7 * mChannels);
							chunkPos += 7 * mChannels;
							for(i=0; i<mChannels; i++) {
								predictor[i] = (unsigned char)oneFrame[0+i];
								delta[i] = 
									((0xff & oneFrame[2+(mChannels-1)*1+i*2]) << 8) |
									((0xff & oneFrame[1+(mChannels-1)*1+i*2]));
								history0[i] = 
									((0xff & oneFrame[4+(mChannels-1)*3+i*2]) << 8) |
									((0xff & oneFrame[3+(mChannels-1)*3+i*2]));
								history1[i] = 
									((0xff & oneFrame[6+(mChannels-1)*5+i*2]) << 8) |
									((0xff & oneFrame[5+(mChannels-1)*5+i*2]));
							}
						}


						is.read(oneFrame, 1);
						chunkPos++;
						for(i=0; i<2; i++) {
							if (i == 0)
								nibble = (oneFrame[0] & 0xF0) >> 4;
							else
								nibble = (oneFrame[0] & 0x0F);

							sample = (	history0[channelID] * adapt_ms_adpcm_coeff0[predictor[channelID]] +
								history1[channelID] * adapt_ms_adpcm_coeff1[predictor[channelID]]) / 256;

							sample += ((nibble >= 8) ? (nibble-16) : nibble) * delta[channelID];

							if (sample > 32767)
								sample = 32767;

							if (sample < -32768)
								sample = -32768;

							history1[channelID] = history0[channelID];
							history0[channelID] = sample;

							delta[channelID] = (adapt_ms_adpcm_table[nibble] * delta[channelID]) / 256;

							if (delta[channelID] < 16)
								delta[channelID] = 16;

							os.write((char*)(&sample), 2);

							//printf("[%07d]\t%05d\n", frameID, sample);
							if (channelID < mChannels-1) {
								channelID++;
							} else {
								channelID = 0;
								frameID++;
							}
						}
					}
				}
				mFrameLen = frameID;

		} else {
			is.seekg(chunkLen, is.cur);
			mOffset += chunkLen;
		}
	}

	os.close();
	return true;
}

void ConvWAV::WriteHeader(std::ofstream &os, int expectFrameLen) {

	long longSampleRate = mSampleRate;
	long byteRate = mSampleRate * 2 * mChannels;

	long totalAudioLen = mChannels * expectFrameLen * 2;
	long totalDataLen = totalAudioLen + 36;

	char header[44];
	header[0] = 'R';  // RIFF/WAVE header
	header[1] = 'I';
	header[2] = 'F';
	header[3] = 'F';
	header[4] = (char) (totalDataLen & 0xff);
	header[5] = (char) ((totalDataLen >> 8) & 0xff);
	header[6] = (char) ((totalDataLen >> 16) & 0xff);
	header[7] = (char) ((totalDataLen >> 24) & 0xff);
	header[8] = 'W';
	header[9] = 'A';
	header[10] = 'V';
	header[11] = 'E';
	header[12] = 'f';  // 'fmt ' chunk
	header[13] = 'm';
	header[14] = 't';
	header[15] = ' ';
	header[16] = 16;  // 4 chars: size of 'fmt ' chunk
	header[17] = 0;
	header[18] = 0;
	header[19] = 0;
	header[20] = 1;  // format = 1
	header[21] = 0;
	header[22] = (char) mChannels;
	header[23] = 0;
	header[24] = (char) (longSampleRate & 0xff);
	header[25] = (char) ((longSampleRate >> 8) & 0xff);
	header[26] = (char) ((longSampleRate >> 16) & 0xff);
	header[27] = (char) ((longSampleRate >> 24) & 0xff);
	header[28] = (char) (byteRate & 0xff);
	header[29] = (char) ((byteRate >> 8) & 0xff);
	header[30] = (char) ((byteRate >> 16) & 0xff);
	header[31] = (char) ((byteRate >> 24) & 0xff);
	header[32] = (char) (2 * mChannels);  // block align
	header[33] = 0;
	header[34] = 16;  // bits per sample
	header[35] = 0;
	header[36] = 'd';
	header[37] = 'a';
	header[38] = 't';
	header[39] = 'a';
	header[40] = (char) (totalAudioLen & 0xff);
	header[41] = (char) ((totalAudioLen >> 8) & 0xff);
	header[42] = (char) ((totalAudioLen >> 16) & 0xff);
	header[43] = (char) ((totalAudioLen >> 24) & 0xff);

	os.write(header, 44);
}
