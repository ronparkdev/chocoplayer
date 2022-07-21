/*
 * Copyright (C) 2008 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * ConvWAV represents a standard 16-bit WAV file, splitting it into
 * artificial frames of 20 ms and taking the maximum of each frame to
 * get an approximation of the waveform contour.
 */

#ifndef __R2K_FILEIO_CONVERTER_WAV__
#define __R2K_FILEIO_CONVERTER_WAV__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAXFRAME 51200

class ConvWAV {
    // Member variables containing frame info
    int mNumFrames;
	int mFrameBytes;
	int mFrameLen;
	int mFileSize;
	int mFormat;
	int mSampleRate;
	int mBitsPerSample;
	int mBytePerSample;
	int mBlockAlign;
    int mChannels;
    int mOffset;

public:
	ConvWAV();

	bool ChkAndFix(char *data, int datalen, const std::string &dstpath);

private:
	void WriteHeader(std::ofstream &os, int expectFrameLen);
};
#endif