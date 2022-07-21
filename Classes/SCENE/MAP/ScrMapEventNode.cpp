#include "ScrMapEventNode.h"

#include "ScrMapChipsetTile.h"

ScrMapEventNode::ScrMapEventNode()
	:pLayer(NULL)
	, m_imgbuf_id(-1)
	, m_event_id(0)
	, m_mirror_type(NeedMirror_None)
	, m_pTexture(NULL)
	, m_pTextureChipset(NULL)
	, m_last_visible(false)
{
	for(int i=0; i<4; i++)
		pSprite[i] = NULL;
}

ScrMapEventNode::~ScrMapEventNode()
{
	for(int i=0; i<4; i++)
		if (pSprite[i] != NULL)
			pLayer->removeChild(pSprite[i]);
}

void ScrMapEventNode::init( CCNode *layer, int index, NeedMirror mirror_type, int map_width, int map_height)
{
	pLayer = layer;
	m_event_id = index;
	m_mirror_type = mirror_type;

	m_map_width = map_width;
	m_map_height = map_height;
}

void ScrMapEventNode::init(CCTexture2D *textureChipset)
{
	if (m_pTextureChipset != textureChipset) {
		m_pTextureChipset = textureChipset;

		if (m_pic_name.isEmpty())
			check();
	}
}

void ScrMapEventNode::reloadShaders()
{
	m_shader.reload();
}

void ScrMapEventNode::check()
{
	m_pTexture = NULL;

	if (!m_pic_name.isEmpty()) {
		m_imgbuf_id = IBuffer::getInstance()->get(imgCharSet, m_pic_name);
		m_pTexture = IBuffer::getInstance()->get(imgCharSet, m_imgbuf_id);
	} else {
		m_pTexture = m_pTextureChipset;
	}

	if (m_pTexture != NULL) {
		for(int i=0; i<NeedMirror_NeedSprites[m_mirror_type]; i++) {
			if (pSprite[i] != NULL)
				pLayer->removeChild(pSprite[i]);

			pSprite[i] = CCSprite::createWithTexture(m_pTexture);
			pSprite[i]->setVisible(m_last_visible);
			m_shader.setShader(pSprite[i]);

			pLayer->addChild(pSprite[i], m_last_posZ);
		}
	}else
		unset();
}

void ScrMapEventNode::set( const TString &name )
{
	if (m_pic_name.isEmpty() || m_pic_name != name) {
		m_pic_name = name;
		check();
	}
	setVisible(true);
}

void ScrMapEventNode::unset()
{
	setVisible(false);
}

void ScrMapEventNode::setPos( float npos_x, float npos_y )
{
	if (m_pTexture != NULL) {
		pSprite[0]->setPosition(ccp(SCR_POS_X(npos_x), SCR_POS_Y(npos_y)));

		if (m_mirror_type == NeedMirror_Horizonal)
			pSprite[1]->setPosition(ccp(SCR_POS_X(npos_x), SCR_POS_Y(npos_y+_TILE(m_map_height))));
		else if (m_mirror_type == NeedMirror_Vertical)
			pSprite[1]->setPosition(ccp(SCR_POS_X(npos_x+_TILE(m_map_width)), SCR_POS_Y(npos_y)));
		else if (m_mirror_type == NeedMirror_Both) {
			pSprite[1]->setPosition(ccp(SCR_POS_X(npos_x+_TILE(m_map_width)), SCR_POS_Y(npos_y)));
			pSprite[2]->setPosition(ccp(SCR_POS_X(npos_x), SCR_POS_Y(npos_y+_TILE(m_map_height))));
			pSprite[3]->setPosition(ccp(SCR_POS_X(npos_x+_TILE(m_map_width)), SCR_POS_Y(npos_y+_TILE(m_map_height))));
		}
	}
}

void ScrMapEventNode::setZPos( int posZ )                           
{
	if (m_last_posZ != posZ && m_pTexture != NULL) {
		pLayer->reorderChild(pSprite[0], posZ);
		if (m_mirror_type == NeedMirror_Horizonal)
			pLayer->reorderChild(pSprite[1], posZ+GAME_SCREEN_HEIGHT);
		else if (m_mirror_type == NeedMirror_Vertical)
			pLayer->reorderChild(pSprite[1], posZ);
		else if (m_mirror_type == NeedMirror_Both) {
			pLayer->reorderChild(pSprite[1], posZ+GAME_SCREEN_HEIGHT);
			pLayer->reorderChild(pSprite[2], posZ);
			pLayer->reorderChild(pSprite[3], posZ+GAME_SCREEN_HEIGHT);
		}
		m_last_posZ = posZ;
	}
}

void ScrMapEventNode::setVisible(bool val) {

	if (m_last_visible != val && m_pTexture != NULL) {
		for(int i=0; i<NeedMirror_NeedSprites[m_mirror_type]; i++) {
			pSprite[i]->setVisible(val);
		}
		m_last_visible = val;
	}
}

void ScrMapEventNode::setTextureRect( int x, int y, int w, int h )
{
	if (m_pTexture != NULL) {
		CCRect rect = CCRect(x,y,w,h);
		for(int i=0; i<NeedMirror_NeedSprites[m_mirror_type]; i++) {
			pSprite[i]->setTextureRect(rect);
		}
	}
}

bool ScrMapEventNode::operator!=(const TString &str) {
	return (m_pic_name != str) || !m_last_visible;
}

void ScrMapEventNode::setTrans( int val )
{
	if (m_last_trans != val && m_pTexture != NULL) {
		m_last_trans = val;

		float per = (8-val)/8.0f;

		m_shader.setAlpha(per);
	}
}

void ScrMapEventNode::setColor( float r, float g, float b, float gr )
{
	m_shader.setColor(r, g, b, gr);
}
