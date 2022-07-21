#include "InfoFile.h"

InfoFile::InfoFile( TString path )
{
	fstream m_fs;
	char pathW[INFO_FILE_PATH_MAX];

	char *data;
	int datalen;
	int i, j;

	openedPath = path;

#if (SR_WIN == 1)
	path.getText_s(pathW,INFO_FILE_PATH_MAX);
#else
	strcpy(pathW, path.getTextUTF8());
#endif

	m_fs.open( pathW , ios::in | ios::binary);
	if (!m_fs.is_open())
		return;

	m_fs.seekg(0, ios::end);
	datalen = (int)m_fs.tellg();
	data = new char[datalen];

	m_fs.clear(); 
	m_fs.seekg(0, ios::beg);
	m_fs.read(data, datalen);
	m_fs.close();

	for(i=0, j=0; i<datalen; i++) {
		if (data[i] == '=') {
			data[i] = '\0';

			TString &name = datName.Add();
			name.setTextUTF8(data+j);
			j = i+1;
        } else if (data[i] == '\n' || data[i] == '\r') {
            if (datName.GetLength() > datValue.GetLength()) {
                TString &value = datValue.Add();
                if (j<i) {
                    data[i] = '\0';
                    
                    value.setTextUTF8(data+j);
                    
                } else {
                    value = "";
                }
            }
			j = i+1;
		}
	}
		
	delete []data;
}

InfoFile::InfoFile( char *data, int datasize )
{
	int i, j;

	for(i=0, j=0; i<datasize; i++) {
		if (data[i] == '=') {
			data[i] = '\0';

			TString &name = datName.Add();
			name.setTextUTF8(data+j);
			j = i+1;
        } else if ((data[i] == '\n') || (data[i] == '\r')) {
            if (datName.GetLength() > datValue.GetLength()) {
                TString &value = datValue.Add();
                if (j<i) {
                    data[i] = '\0';

                    value.setTextUTF8(data+j);
				
                } else {
                    value = "";
                }
            }
			j = i+1;
		}
	}
}

InfoFile::~InfoFile()
{

}

TString InfoFile::getValue( const TString &name )
{
	for(int i=0; i<datName.GetLength(); i++)
		if (datName[i] == name)
			return datValue[i];

	return "";
}

void InfoFile::setValue( const TString &name, const TString &value )
{
	for(int i=0; i<datName.GetLength(); i++)
		if (datName[i] == name) {
			datValue[i] = value;
			return;
		}

	datName.Add(name);
	datValue.Add(value);
}

void InfoFile::flush()
{
	fstream m_fs;
	char pathW[INFO_FILE_PATH_MAX];

#if (SR_WIN == 1)
	openedPath.getText_s(pathW,INFO_FILE_PATH_MAX);
#else
	strcpy(pathW, openedPath.getTextUTF8());
#endif

	m_fs.open( pathW , ios::out | ios::ate | ios::binary);
	if (!m_fs.is_open())
		return;

	for(int i=0; i<datName.GetLength(); i++) {
		if (!datName[i].isEmpty())
			m_fs.write(datName[i].getTextUTF8(), datName[i].getSize());

		m_fs.write("=", 1);

		if (!datValue[i].isEmpty())
			m_fs.write(datValue[i].getTextUTF8(), datValue[i].getSize());

		m_fs.write("\n", 1);
	}
	
	m_fs.close();
}
