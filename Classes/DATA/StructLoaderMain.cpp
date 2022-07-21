#include "StructLoader.h"

#include "../AppMulti.h"

#include <cocos2d.h>
USING_NS_CC;

StructLoader::StructLoader()
	:opened(false)
	,mode(StructLoaderModeNone)
	,pData(NULL)
{
}

StructLoader::~StructLoader()
{
	close();
}

bool StructLoader::open( bool isRead, const char *path )
{
	if (opened == true) {
		close();
	}

	mode = isRead ? StructLoaderModeRead : StructLoaderModeWrite;

    opened = false;
    strncpy(openedPath, path, sizeof(openedPath));

	if (mode == StructLoaderModeRead) {
		m_fs.open( openedPath , ios::in | ios::binary);

		if (m_fs.is_open() == true) {
			m_fs.seekg(0, ios::end);
			dataLength = (int)m_fs.tellg();

			if (pData != NULL)
				delete(pData);

			memoryDoNotDealloc = false;
			pData = new char[dataLength];

			m_fs.clear(); 
			m_fs.seekg(0, ios::beg);
		

			int readed = 0;
			m_fs.read(pData, dataLength);
			readed = (int)m_fs.gcount();

			m_fs.close();

			offset = 0;
			opened = true;
		}
	} else {
		dataLength = STRUCT_LOADER_BUFFER_MIN;
		pData = new char[dataLength];
		offset = 0;

		opened = true;
		memoryDoNotDealloc = false;
	}

	return opened;
}

bool StructLoader::open( char *data, int datalen, bool useSharedMemory /*= false */ )
{
	if (data == NULL) {
		return false;
	} else {
		if (opened == true) {
			close();
		}

		opened = true;
		mode = StructLoaderModeRead;

		memoryDoNotDealloc = useSharedMemory;
		dataLength = datalen;
		pData = data;
		offset = 0;

		return true;
	}
}

void StructLoader::stopOpening()
{
	if (opened) {
		opened = false;
	}
}

bool StructLoader::close()
{
	if (!opened) {
		return false;
	} else {
		bool result = true;

		if (mode == StructLoaderModeWrite) {
			m_fs.open( openedPath , ios::out | ios::ate | ios::binary);

			if (m_fs.is_open()) {
				m_fs.write(pData, offset);
				m_fs.close();

			} else {
				result = false;
			}
		}

		if (pData != NULL && !memoryDoNotDealloc) {
			delete []pData;
		}
		pData = NULL;

		opened = false;

		return result;
	}
}

void StructLoader::allocForWrite( int size ) {
	if (mode == StructLoaderModeWrite) {
		int requiredDataLength = offset + size;

		if (dataLength < requiredDataLength) {
			int lastDataLength = dataLength;
			int newDataLength = dataLength;

			do {
				newDataLength <<= 1;
			} while (requiredDataLength > newDataLength && newDataLength > 0);

			if (newDataLength < 0) {
				throw StructLoaderException_WrongStruct();
			} else {
				char *pLastData = pData;
				char *pNewData = new char[newDataLength];

                memcpy(pNewData, pLastData, lastDataLength);

				pData = pNewData;
				dataLength = newDataLength;

				delete []pLastData;
			}
		}
	}
}

bool StructLoader::isSizeReadable( int size )
{
	return ((offset + size) < dataLength);
}

void StructLoader::writeStructSize( int startOffsetOfStruct )
{
	int lastOffset, structSize , lengthOfSize;
	structSize = offset - startOffsetOfStruct;
	if (structSize < 0) {
		throw StructLoaderException_WrongStruct();
	} else {
		lastOffset = offset;

		//predict offset for write size
		*this << structSize;
		lengthOfSize = offset - lastOffset;

		allocForWrite(lengthOfSize);
		memmove(pData + startOffsetOfStruct + lengthOfSize, pData + startOffsetOfStruct, sizeof(char)*structSize);
	
		offset = startOffsetOfStruct;
		*this << structSize;
		offset = startOffsetOfStruct + lengthOfSize + structSize;
	}
}

bool StructLoader::isOpened()	{	return opened;	}

void StructLoader::intoCode()
{
 	int type;
 	*this >> type;
}

void StructLoader::finishStruct()
{
	if (mode == StructLoaderModeInit) {

	} else if (mode == StructLoaderModeRead) {
		int lastpos, lasttype = -1, type, size;
		while (offset < dataLength) {
			*this >> type;
			if (type == CODE_END_OF_STRUCT) {
				break;
			}

			//R2K ChocoPlayer 1.x Savefile Bug.... :(
			if (lasttype != -1 && lasttype >= type) {
				setOffset(lastpos);
				break;
			}

			if (offset >= dataLength) { 
				break;
			} 
	
			*this >> size;
			setOffset(offset+size);

			lastpos = offset;
			lasttype = type;
		}
	} else {
		*this << CODE_END_OF_STRUCT;
	}
}

bool StructLoader::isNextCode(int find)
{
	int code, structSize, lastOffset;
	while (offset < dataLength) {
		lastOffset = offset; 
		*this >> code;
		if (code == CODE_END_OF_STRUCT) {
			setOffset(lastOffset);
			break;
		} else if (code == find) {
			setOffset(lastOffset);
			return true;
		} else if (code > find) {
			setOffset(lastOffset);
			break;
		} else if (offset >= dataLength) {
			break;
		}
		*this >> structSize;
		setOffset(offset + structSize);
	}

	return false;
}

int StructLoader::getOffset()
{
	return offset;
}

void StructLoader::setOffset( int newOffset )
{
	if (newOffset < 0) {
		offset = 0;
	} else if (newOffset > dataLength) {
		if (mode == StructLoaderModeWrite) {
			allocForWrite(newOffset - dataLength);
			offset = newOffset;
		} else {
			offset = dataLength;
		}
	} else {
		offset = newOffset;
	}
}

int StructLoader::getLength()
{
	return dataLength;
}

char* StructLoader::getRawData()
{
	return pData;
}
