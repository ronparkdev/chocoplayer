#include "GrpPictureNode.h"

GrpPictureNode::GrpPictureNode() 
	:pTex(NULL) 
	,pSprUpper(NULL) 
	,pSprLower(NULL)
{
	isNeedCheck = true;

	curRot = curWave = 0.0f;

	this->autorelease();
}

GrpPictureNode::~GrpPictureNode() 
{
	if (pSprUpper != NULL)
		this->removeChild(pSprUpper);

	if (pSprLower != NULL)
		this->removeChild(pSprLower);
}

void GrpPictureNode::reloadShaders()
{
	shaderLower.reload();
	shaderUpper.reload();
}

void GrpPictureNode::Draw(int framedelta, LsdPicture *d)
{
	if (d->need_picname_check) {
		if (d->picName.isEmpty()) {
			if (pSprLower != NULL) {
				pSprUpper->setVisible(false);
				pSprLower->setVisible(false);
				return;
			}
		} else {
			if (texName == d->picName && isNoBackground == d->is_use_backcolor)
			{
				if (d->curTA < 100.0f)
					pSprUpper->setVisible(true);
				if (d->curBA < 100.0f)
					pSprLower->setVisible(true);
			} else {
				int texID;
				if (d->is_use_backcolor)
					texID = IBuffer::getInstance()->get(imgPicture, d->picName);
				else
					texID = IBuffer::getInstance()->get(imgPicture, IMAGE_BUFFER_NOALPHA_PRESTR + d->picName);

				if (texID != -1) {
					pTex = IBuffer::getInstance()->get(imgPicture, texID);
					int width = pTex->getContentSize().width;
					int height = pTex->getContentSize().height;

					if (pSprUpper != NULL) {
						this->removeChild(pSprUpper);
						this->removeChild(pSprLower);
					}

// 					m_pSpriteBatch = CCSpriteBatchNode::createWithTexture(m_pTexture, 2);
// 					this->addChild(m_pSpriteBatch);

					pSprUpper = CCSprite::createWithTexture(pTex);
					pSprUpper->setTextureRect(CCRect(0,0,width,height/2.0f));
					pSprUpper->setPosition(ccp(0,  height/4.0f));
					shaderUpper.setShader(pSprUpper);
					this->addChild(pSprUpper);

					pSprLower = CCSprite::createWithTexture(pTex);
					pSprLower->setTextureRect(CCRect(0,height/2.0f,width,height/2.0f));
					pSprLower->setPosition(ccp(0, -height/4.0f));
					shaderLower.setShader(pSprLower);
					this->addChild(pSprLower);
				}
			}
			isNeedCheck = true;
		}

		texName = d->picName;
		isNoBackground = d->is_use_backcolor;
		curWave = curRot = 0.0f;
		this->setRotation(0.0f);
	}

	if (d->is_fixed_to_map) {
		if (isNeedCheck) {
			lastScrX = ScreenCommon::game_screen_x;
			lastScrY = ScreenCommon::game_screen_y;
			isNeedCheck = false;
			d->need_picname_check = true;
		}

		float scr_def_x = ScreenCommon::game_screen_x - lastScrX;
		float scr_def_y = ScreenCommon::game_screen_y - lastScrY;

		if (scr_def_x != 0.0f || scr_def_y != 0.0f) {

			d->curX -= scr_def_x;
			d->curY -= scr_def_y;

			d->finX -= scr_def_x;
			d->finY -= scr_def_y;

			lastScrX = ScreenCommon::game_screen_x;
			lastScrY = ScreenCommon::game_screen_y;
			d->need_picname_check = true;
		}
	}

	if (d->frame_left > 0) {
		d->frame_left -= framedelta;
		if (d->frame_left <= 0) {
			d->curX		= d->finX;
			d->curY		= d->finY;
			
			d->curS		= d->finS;
			d->curTA	= d->finTA;
			d->curBA	= d->finBA;
			
			d->curR		= d->finR;
			d->curG		= d->finG;
			d->curB		= d->finB;
			d->curGr	= d->finGr;

			d->effect_cur_rotate_speed	= d->effect_rotate_speed;
			d->effect_cur_wave_speed	= d->effect_wave_speed;
		} else {
			d->curX		+= (d->finX - d->curX)		/ d->frame_left;
			d->curY		+= (d->finY - d->curY)		/ d->frame_left;
			
			d->curS		+= (d->finS - d->curS)		/ d->frame_left;
			d->curTA	+= (d->finTA - d->curTA)	/ d->frame_left;
			d->curBA	+= (d->finBA - d->curBA)	/ d->frame_left;
			
			d->curR		+= (d->finR - d->curR)		/ d->frame_left;
			d->curG		+= (d->finG - d->curG)		/ d->frame_left;
			d->curB		+= (d->finB - d->curB)		/ d->frame_left;
			d->curGr	+= (d->finGr - d->curGr)	/ d->frame_left;

			d->effect_cur_rotate_speed	+= (d->effect_rotate_speed	- d->effect_cur_rotate_speed)	/ d->frame_left;
			d->effect_cur_wave_speed	+= (d->effect_wave_speed	- d->effect_cur_wave_speed)		/ d->frame_left;
		}
		d->need_picname_check = true;


		//CCLOG((_ST("") + d->frame_left).getTextUTF8());
	}

	if (d->need_picname_check) {
		this->setPosition(ccp(SCR_POS_X(d->curX), SCR_POS_Y(d->curY)));
		this->setScale(d->curS / 100.0f);
		if (pSprUpper != NULL && pSprLower != NULL) {
			if (d->curTA == 100.0f) {
				pSprUpper->setVisible(false);
			} else {
				pSprUpper->setVisible(true);
				shaderUpper.setAlpha((100.0f - d->curTA)/100.0f);
				shaderUpper.setColor(
					(d->curR  - 100.0f)/100.0f, 
					(d->curG  - 100.0f)/100.0f, 
					(d->curB  - 100.0f)/100.0f, 
					(d->curGr - 100.0f)/100.0f 
					);
			}
			if (d->curBA == 100.0f) {
				pSprLower->setVisible(false);
			} else {
				pSprLower->setVisible(true);
				shaderLower.setAlpha((100.0f - d->curBA)/100.0f);
				shaderLower.setColor(
					(d->curR  - 100.0f)/100.0f, 
					(d->curG  - 100.0f)/100.0f, 
					(d->curB  - 100.0f)/100.0f, 
					(d->curGr - 100.0f)/100.0f 
					);
			}
		}

		d->need_picname_check = false;
	}

	if (d->effect_type == 1) {
		curWave = 0.0f;
	} else if (d->effect_type == 2) {
		if (curRot != 0.0f) {
			curRot = 0.0f;
			this->setRotation(0.0f);
		}
	}

	if (curRot > 0.0f || d->effect_type == 1) {
		curRot += d->effect_cur_rotate_speed / 250.0f;
		if (curRot > 1.0f) {
			if (d->effect_type == 0)
				curRot = 0.0f;
			else
				curRot = curRot - (int)curRot;
		}
		this->setRotation(curRot*360.0f);
	}
}

bool GrpPictureNode::isUsing()
{
	return (pSprLower != NULL);
}
