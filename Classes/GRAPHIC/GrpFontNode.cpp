#include "GrpFontNode.h"

#include "../COMMON/ScreenCommon.h"
#include "../COMMON/GlobalLang.h"
#include "../COMMON/GlobalLangFont.h"

#define GRP_FONT_CUSTOM_PIC2_PATH "extra_font_large.png"
#define GRP_FONT_CUSTOM_PIC2_WIDTH 96

GrpFontNode::GrpFontNode()
	:isWaitKey(false)
	,customchar(0)
	,pFont(NULL)
	,pCustom(NULL)
	,width(0)
{
	CCNode::autorelease();
}

void GrpFontNode::operator=(const GrpFontNode& val) {
	txt	= val.txt;
	x	= val.x;
	y	= val.y;
	delay	= val.delay;
	color	= val.color;
	fontSize			= val.fontSize;
	isAlignRight	= val.isAlignRight;
	isWaitKey	= val.isWaitKey;
	width		= val.width;
	customchar = val.customchar;

	pFont = NULL;
	pFontShadow = NULL;
	pCustom = NULL;
	pCustomShadow = NULL;
} 

GrpFontNode::~GrpFontNode()
{

}

int GrpFontNode::Init(GrpShaderFont *shader)
{
	bool result = false;

	if (!isWaitKey) {
		if (pFont != NULL) {
			this->removeChild(pFont);
			pFont = NULL;

			this->removeChild(pFontShadow);
			pFontShadow = NULL;
		}

		if (pCustom != NULL) {
			this->removeChild(pCustom);
			pCustom = NULL;

			this->removeChild(pCustomShadow);
			pCustomShadow = NULL;
		}

		if (customchar == 0) {
			pFont = CCLabelTTF::create(txt.getTextUTF8(), GlobalLangFont::getFontName(0), fontSize);
			shader->setShader(pFont);
			this->addChild(pFont, 1);

			pFontShadow = CCLabelTTF::create(txt.getTextUTF8(), GlobalLangFont::getFontName(0), fontSize);
			pFontShadow->setPosition(ccp(fontSize*0.15f, -fontSize*0.15f));
			pFontShadow->setColor(ccc3(0,0,0));
			this->addChild(pFontShadow, 0);


			width = pFont->getContentSize().width;
		} else {

			int w = GRP_FONT_CUSTOM_PIC2_WIDTH;
			
			float scale = fontSize / w;
			int x, y, index = customchar;

			if ('A' <= index && index <= 'Z')
			{
				x = ((index - 'A')%13)*w;
				y = ((int)((index - 'A')/13))*w;
			} else if ('a' <= index && index <= 'z')
			{
				x = ((index - 'a')%13)*w;
				y = ((int)((index - 'a')/13)+2)*w;
			}

			CCTexture2D *tex = CCTextureCache::sharedTextureCache()->addImage(GRP_FONT_CUSTOM_PIC2_PATH);
			tex->setAntiAliasTexParameters();					


			pCustom = CCSprite::createWithTexture(tex);
			pCustom->setTextureRect(CCRect(x,y,w,w));
			pCustom->setScale(scale);
			shader->setShader(pCustom);
			this->addChild(pCustom, 1);

			pCustomShadow = CCSprite::createWithTexture(tex);
			pCustomShadow->setTextureRect(CCRect(x,y,w,w));
			pCustomShadow->setPosition(ccp(fontSize*0.15f, -fontSize*0.15f));
			pCustomShadow->setColor(ccc3(0,0,0));
			pCustomShadow->setScale(scale);
			this->addChild(pCustomShadow, 0);


			width = pCustom->getContentSize().width * scale;
		}
	}
	return width;
}

int GrpFontNode::getWidth()
{
	return width;
}

void GrpFontNode::setOpacity( float val )
{
	if (pCustom != NULL)
		pCustom->setOpacity(val);

	if (pFont != NULL)
		pFont->setOpacity(val);

	if (pFontShadow != NULL)
		pFontShadow->setOpacity(val);

	if (pCustomShadow != NULL)
		pCustomShadow->setOpacity(val);
}
