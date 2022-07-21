#include "ImageBuffer.h"

IBuffer *IBuffer::m_pInstance = NULL;

IBuffer * IBuffer::getInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new IBuffer();

	return m_pInstance;
}


void IBuffer::removeInstance()
{
	if (m_pInstance != NULL) {
		for(int i=0; i<IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE; i++) {
			IBufferNode &node = m_pInstance->m_nodes[i];

			node.isUsed = false;

		}
		
		
		delete m_pInstance;

	}

	m_pInstance = NULL;
}

IBuffer::IBuffer()
{
	m_pInstance = this;
	for(int i=0; i<IMAGE_BUFFER_TYPES; i++) {
		m_nodelen[i] = 0;
	}
}

void IBuffer::setFileCTL( FileCTL *filectl )
{
	for(int i=0; i<IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE; i++)
		m_nodes[i].setFileCTL(filectl);
}

int IBuffer::get( Image_Type_Name type, const TString &name )
{
	int type_id = (int)type;
	int node_checked = 0;
	int node_len = m_nodelen[type_id];

	if (name.isEmpty())
		return get( type, IMAGE_BUFFER_NOFILE);

	for(int i=0; i<IMAGE_BUFFER_PER_TYPE && node_checked < node_len; i++) {
		IBufferNode &node = m_nodes[(type_id<<IMAGE_BUFFER_PER_TYPE_SHIFT) | i];
		if (node.isUsed) {
			if (node.texName == name) {
				node.isMapChanged = false;
				return i;		//Find Image in Buffer
			}

			node_checked++;
		}
	}

	//Cant fin Image in Buffer, load from sdcard
	for(int i=0; i<IMAGE_BUFFER_PER_TYPE; i++) {
		IBufferNode &node = m_nodes[(type_id<<IMAGE_BUFFER_PER_TYPE_SHIFT) | i];
		if (!node.isUsed) {
			if (node.Set(type, name)) {
				m_nodelen[type_id]++;
				return i;
			}
			break;
		}
	}

	return get( type, IMAGE_BUFFER_NOFILE);
}

CCTexture2D* IBuffer::get( Image_Type_Name type, int index )
{
	IBufferNode &node = m_nodes[(int)type<<IMAGE_BUFFER_PER_TYPE_SHIFT | index];
	node.isMapChanged = false;
	if (node.isUsed) {
		return node.pTex;
	} else {
		CCLOG("IBuffer::get_texture failed");
		return NULL;
	}
}

void IBuffer::mapChanging()
{
	for(int i=0; i<IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE; i++)
		if (m_nodes[i].isUsed)
			m_nodes[i].isMapChanged = true;
}

void IBuffer::mapChanged()
{
	for(int i=0; i<IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE; i++)
		if (m_nodes[i].isUsed && m_nodes[i].isMapChanged)
			m_nodes[i].Unset();
}

void IBuffer::reload(const char *key) {
	int i, pos, type;

	TString tkey, ttype, tname;

	tkey.setTextUTF8(key);

	CCLOG("Reload Try : 0x%s", tkey.getTextUTF8());

	if ((pos = tkey.indexOf('|')) < 0)
		return;

	ttype = tkey.substring(0, pos);
	tname = tkey.substring(pos+1);

	type = -1;
	for(i=0; i<IMAGE_BUFFER_TYPES; i++)
		if (ttype == Image_Type_Folder[i])
			type = i;

	if (type == -1)
		return;

	for(i=type << IMAGE_BUFFER_PER_TYPE_SHIFT; 
		i<(type + 1) << IMAGE_BUFFER_PER_TYPE_SHIFT; i++)
	{
		if (m_nodes[i].texName == tname) {
			m_nodes[i].Reload();
			return;
		}
	}
}

void IBuffer::release()
{
	for(int i=0; i<IMAGE_BUFFER_TYPES*IMAGE_BUFFER_PER_TYPE; i++)
		m_nodes[i].Unset();
}
