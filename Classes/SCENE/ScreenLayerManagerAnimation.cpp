#include "ScreenLayerManager.h"

#include "../COMMON/ScreenCommon.h"

#define BLOCK_WIDTH 4

void ScreenLayerManager::doReadyAni( int animation_type )
{
	lastGLSLFileName = NULL;

	char *filename;

	//Get File Name
	switch(animation_type) {
		case 19://19바로삭제
		case 20://19삭제안함 //TODO 만들어야됨
			changeFrameTotal = changeFrameLeft = 1;

		default:
		case 0://00페이드인
		case 16://16주인공확대
		case 18://18흐물흐물
			filename = "filter_scr_fade.fsh";break;

		case 1://01전체에 랜덤 블록*
		case 2://02위에서 랜덤 블록*
		case 3://03아래서 랜덤 블록*
			filename = "filter_scr_block.fsh";break;

		case 4://04블라인드 오픈
			filename = "filter_scr_blind.fsh";break;

		case 5://05상하에서 스트라이프
			filename = "filter_scr_stripe_h.fsh";break;

		case 6://06좌우에서 스트라이프
			filename = "filter_scr_stripe_v.fsh";break;

		case 7://07바깥쪽에서 중심에 a
			if (isScrHided)
				filename = "filter_scr_mid_in.fsh";
			else
				filename = "filter_scr_mid_out.fsh";
			break;

		case 8://08중심에서 바깥쪽에 a
			if (isScrHided)
				filename = "filter_scr_mid_out.fsh";
			else
				filename = "filter_scr_mid_in.fsh";
			break;

		case 9://09아래에서 스크롤
			if (isScrHided)
				filename = "filter_scr_scroll_down.fsh";
			else
				filename = "filter_scr_scroll_up.fsh";
			break;

		case 10://10위에서 스크롤
			if (isScrHided)
				filename = "filter_scr_scroll_up.fsh";
			else
				filename = "filter_scr_scroll_down.fsh";
			break;

		case 11://11오른쪽에서 스크롤
			if (isScrHided)
				filename = "filter_scr_scroll_right.fsh";
			else
				filename = "filter_scr_scroll_left.fsh";
			break;

		case 12://12왼쪽에서 스크롤
			if (isScrHided)
				filename = "filter_scr_scroll_left.fsh";
			else
				filename = "filter_scr_scroll_right.fsh";
			break;

		case 13://13상하에서 결합
			filename = "filter_scr_union_v.fsh";break;

		case 14://14좌우에서 결합
			filename = "filter_scr_union_h.fsh";break;

		case 15://15상하좌우에서 결합
			filename = "filter_scr_union_both.fsh";break;

		case 17://17모자이크
			filename = "filter_scr_mosaic.fsh";break;
	}

	//Get Moasic Pattern
	switch(animation_type) {
		case 1://01전체에 랜덤 블록*
			getMosaic(mosaicPattern, 40, 30);
			break;

		case 2://02위에서 랜덤 블록*
			getMosaicUp(mosaicPattern, 40, 30);
			break;

		case 3://03아래서 랜덤 블록*
			getMosaicDown(mosaicPattern, 40, 30);
			break;
	}

	lastGLSLFileName = NULL;

	//Load Shader Data
	if (lastGLSLFileName == NULL || strcmp(lastGLSLFileName, filename) != 0) {
		

		if (pGlProg != NULL) {
			pGlProg->release();
			pGlProg = NULL;
		}

#if (SR_ANDROID == 1)
		char *path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename).c_str();
#elif (SR_IPHONE == 1)
        const char *path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename).c_str();
#else
        const char *path = filename;
#endif
        lastGLSLFileName = filename;

		CCString *data = CCString::createWithContentsOfFile(path);
		const GLchar * fragmentSource = (GLchar*) data->getCString();
		
		pGlProg = new CCGLProgram();

		pGlProg->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, fragmentSource);
		pGlProg->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		pGlProg->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		if (!pGlProg->link() && animation_type != 0) {
			doReadyAni(0);
			return;
		}
		pGlProg->updateUniforms();

		glProgCtl = glGetUniformLocation(pGlProg->getProgram(), "u_Value");
	}


	pCurScreenSpr->setVisible(true);
	pCurScreenSpr->setFlipY(true);
	pCurScreenSpr->setShaderProgram(pGlProg);
	pCurScreenSpr->setScale(ScreenCommon::game_s);

	pCurScreenSpr->setPosition(ccp(
		ScreenCommon::game_x + ScreenCommon::game_w * ScreenCommon::game_s * 0.5f, 
		ScreenCommon::game_y + ScreenCommon::game_h * ScreenCommon::game_s * 0.5f));

	lastAniType = animation_type;
}

void ScreenLayerManager::doFinAni()
{
	if (pCurScreenSpr != NULL) {
		this->removeChild(pCurScreenSpr);
		pCurScreenSpr = NULL;
	}
}

void ScreenLayerManager::doRunAni(float per) {

	if (pGlProg == NULL)
		return;

	if (per > 1.0f)
		per = 1.0f;

	if (per < 0.0f)
		per = 0.0f;

	pGlProg->use();
	switch(lastAniType) {
	case 0://00페이드인
	case 4://04블라인드 오픈
	case 7://07바깥쪽에서 중심에 a
	case 8://08중심에서 바깥쪽에 a
	case 9://09아래에서 스크롤
	case 10://10위에서 스크롤
	case 11://11오른쪽에서 스크롤
	case 12://12왼쪽에서 스크롤
	case 13://13상하에서 결합
	case 14://14좌우에서 결합
	case 15://15상하좌우에서 결합
	case 17://17모자이크
	case 16://16주인공확대
	case 18://18흐물흐물
	case 19://19바로삭제
	case 20://19바로삭제
		{
			glUniform1f(glProgCtl, per);
			break;
		}

	case 1://01전체에 랜덤 블록*
	case 2://02위에서 랜덤 블록*
	case 3://03아래서 랜덤 블록*
		{
			memset(mosaicBuffer, 0, sizeof(bool)*1200);
			
			int i, j, id, val, tper = (int)(per*1200);
			for(i=0; i<tper; i++)
				mosaicBuffer[mosaicPattern[i]] = true;

			id = 0;
			for(i=0; i<150; i++) {
				val = 0;
				for(j=0; j<8; j++) {
					if (mosaicBuffer[id++])
						val = (val << 1) | 0x1;
					else
						val = val << 1;
				}
				mosaicDataIn[i] = val;
			}

			glUniform1iv(glProgCtl, 150, mosaicDataIn);
 			break;
		}


	case 5://05상하에서 스트라이프
		{
			glUniform1i(glProgCtl, (int)(per*60));
			break;
		}

	case 6://06좌우에서 스트라이프
		{
			glUniform1i(glProgCtl, (int)(per*80));
			break;
		}
	}
}

void ScreenLayerManager::getMosaic(int *d, int x, int y) {
	bool *mosaic;

	mosaic = new bool[x*y];
	memset(mosaic, 0, sizeof(bool)*x*y);

	int tcnt = x * y;
	int c = 0, p, n;

	for(int i=0; i<tcnt; i++) {
		n = 0;
		p = (int)(rand() % (tcnt-c));

		for(int j=0; j<tcnt; j++) {
			if (!mosaic[j]) {
				if (p == n) {
					mosaic[j] = true;
					d[c] = j;
					c++;
					break;
				}
				n++;
			}
		}
	}

	delete []mosaic;
}

void ScreenLayerManager::getMosaicUp(int *d, int x, int y) {
	bool *mosaic;

	mosaic = new bool[x*y];
	memset(mosaic, 0, sizeof(bool)*x*y);

	int tcnt = x * y;
	int c = 0, p, tp, n;

	for(int i=0; i<tcnt; i++) {
		n = 0;

		tp = (int)(y*i/tcnt+y/6) * x;
		while(-1) {
			p = (int)(rand() % tp);
			if (!mosaic[p] && p < tcnt) {
				mosaic[p] = true;
				d[c] = p;
				c++;
				break;
			}
		}
	}

	delete []mosaic;
}

void ScreenLayerManager::getMosaicDown(int *d, int x, int y) {
	bool *mosaic;

	mosaic = new bool[x*y];
	memset(mosaic, 0, sizeof(bool)*x*y);

	int tcnt = x * y;
	int c = 0, p, tp, n;

	for(int i=0; i<tcnt; i++) {
		n = 0;

		tp = (int)(y*i/tcnt+y/6) * x;
		while(-1) {
			p = tcnt - (int)(rand() % tp);
			if (!mosaic[p] && p >= 0) {
				mosaic[p] = true;
				d[c] = p;
				c++;
				break;
			}
		}
	}

	delete []mosaic;
}