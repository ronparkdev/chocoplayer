#include "GrpFont.h"

#include "../COMMON/ScreenCommon.h"
#include "../COMMON/MgrSetting.h"

GrpFont::GrpFont( GameMain *main )
	:msgStartTime(0)
	,isWaittingKey(false)
{
	pGM = main;
	fontScale = ScreenCommon::game_s*1.5f;
	fontSize = GRP_FONT_SIZE*fontScale;
	lineHeight = GRP_NEXTLINE_SIZE*fontScale;
	lastSysTex = NULL;

	for(int i=0; i<20; i++) {
		pFontShaders[i] = NULL;
		pFontExtraShaders[i] = NULL;
	}

	this->setScale(1.0f/fontScale);
	this->autorelease();
}

GrpFont::~GrpFont()
{
	removeText();

	for(int i=0; i<20; i++) {
		if (pFontShaders[i] != NULL)
			delete pFontShaders[i];

		if (pFontExtraShaders[i] != NULL)
			delete pFontExtraShaders[i];
	}
}

void GrpFont::reloadShaders()
{
	for(int i=0; i<20; i++) {
		if (pFontShaders[i] != NULL)
			pFontShaders[i]->reload();

		if (pFontExtraShaders[i] != NULL)
			pFontExtraShaders[i]->reload();
	}
}

void GrpFont::removeText() {
	for(int i=0; i<pFontNodes.GetLength(); i++)
		this->removeChild(pFontNodes[i]);

	pFontNodes.RemoveAll();
}

void GrpFont::setText(const TString &str)
{
	int texID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	if (pTex == NULL)
		return;

	setText(str, pTex);
}

void GrpFont::setText(const TString &str, CCTexture2D *pSysTex)
{
	//초기화
	removeText();

	if (pSysTex != lastSysTex) {
		for(int i=0; i<20; i++) {
			if (pFontShaders[i] != NULL) {
				delete pFontShaders[i];
				pFontShaders[i] = NULL;
			}

			if (pFontExtraShaders[i] != NULL) {
				delete pFontExtraShaders[i];
				pFontExtraShaders[i] = NULL;
			}
		}

		lastSysTex = pSysTex;
	}

	text = "";
	
	isAutoMsg = false;
	showMoneyPos = -1;

	if (pGM != NULL)
		keyLastEnter = pGM->ctl->getKey(rkc_enter);

	int offset = 0;
	int curPosX = 0;
	int curPosY = -lineHeight/2;
	int curColor = 0;
	int curSpeed = 1;
	int curDelay = 0;
	bool curMsgSkip = false;

	//전처리 : 문장에 쓰이는 변수를 대입함
	bool find_sys = false;
	
	int lastpos = 0;
	for(int i=0; i<str.getLength(); i++) {
		int c = str.charAt(i);

		if (!find_sys) {
			switch(c) {
			case '\\':
			case 0xA5: //YEN(FOR JAPANESE) //TODO
				{
					find_sys = true;
					break;
				}
			}
		} else {
			//toupper
			if ('a' <= c && c <= 'z') c += 'A' - 'a';

			switch(c) {
			case '\\':
			case 0xA5: //YEN(FOR JAPANESE)
				{
					text = text + str.substring(lastpos, i-1) 
							+ "\\";

					lastpos = i;
					break;
				}

			case 'N'://CharacterName
			case 'V'://Print Variance
				{
					int sf = str.indexOf('[', i+1);
					if (sf == -1)break;
					int st = str.indexOf(']', sf+1);
					if (st == -1)break;

					int val = str.substring(sf+1, st).getInt();

					switch(c) {
						case 'N'://CharacterName
							{
								if (pGM != NULL)
									text = text + str.substring(lastpos, i-1) 
										+ pGM->lsd.Character[val-1].name;
								break;
							}
						case 'V'://Print Variance
							{
								if (pGM != NULL)
									text = text + str.substring(lastpos, i-1) 
										+ pGM->lsd.System.variables[val-1];
								break;
							}
					}

					i = lastpos = st+1;
					break;
				}
			case '^'://Auto Continue
				{
					isAutoMsg = true;
					text = text + str.substring(lastpos, i-1);
					lastpos = i+1;
					break;
				}
			}
			find_sys = false;
		}
	}
	text = text + str.substring(lastpos, str.getLength());

	int m_strlen = text.getLength();

	for(int i=0; i<m_strlen; i++) {
		int c = text.charAt(i);

		if (!find_sys) {
			switch(c) {
			case '\\':
			case 0xA5: //YEN(FOR JAPANESE)
				{
					find_sys = true;
					break;
				}

			case '$':
				{
					if (i+1 < m_strlen) {
						char c2 = str.charAt(i+1);

						GrpFontNode *n = new GrpFontNode();
						n->customchar = c2;
						n->color = curColor;
						n->delay = curMsgSkip?0:curDelay + curSpeed;
						n->y = curPosY;
						n->fontSize = fontSize;
						this->addChild(n);
						pFontNodes.Add(n);

						curDelay = 0;
						
						i += 1;
						offset++;
					}
					break;
				}

			case '\n':
				{
					curPosY -= lineHeight;

					break;
				}

			default:
				{
					GrpFontNode *n = new GrpFontNode();
					n->isWaitKey = false;
					n->txt.setChar(c);
					n->customchar = false;
					n->color = curColor;
					n->delay = curMsgSkip?0:curDelay + curSpeed;
					n->y = curPosY;
					n->fontSize = fontSize;
					this->addChild(n);
					pFontNodes.Add(n);

					curDelay = 0;
					offset++;
				}
			}
		} else {
			//toupper
			if ('a' <= c && c <= 'z') c += 'A' - 'a';

			switch(c) {
			case 'C'://MsgColor
			case 'S'://MsgSpeed
				{
					int sf = str.indexOf('[', i+1);
					if (sf == -1)break;
					int st = str.indexOf(']', sf+1);
					if (st == -1)break;

					int val = str.substring(sf+1, st).getInt();

					switch(c) {
					case 'C'://MsgColor
						{
							curColor = val;
							break;
						}
					case 'S'://MsgSpeed
						{
							curSpeed = val;
							break;
						}
					}

					i = st; 
					lastpos = st+1;
					break;
				}
			case '$':
				{//Print Money
					showMoneyPos = offset;
					break;
				}
			case '.':
				{//Wait 250ms
					curDelay  = 15;//TODO Check!
					break;
				}
			case '|':
				{//Wait 1000ms
					curDelay  = 60;
					break;
				}
			case '!'://WaitKey
				{
					GrpFontNode *n = new GrpFontNode();
					n->isWaitKey = true;
					n->y = curPosY;
					n->width = 0;
					n->delay = 0;
					n->color = curColor;
					this->addChild(n);
					pFontNodes.Add(n);

					offset++;
					break;
				}
			case '>':
				{//Fast Print start
					curMsgSkip = true;
					break;
				}
			case '<':
				{//Fast Print end
					curMsgSkip = false;
					break;
				}
			case '_':
				{//Half space
					break;
				}
			}
			find_sys = false;
		}
	}

	lastMsgDelay = curDelay;

	pFontNodes.ChangeMode(ArrayMode);
	int lastWidth = 0, curWidth;
	float lastposy = -1.0f;
	for(int i=0; i<pFontNodes.GetLength(); i++) {
		GrpFontNode *n = pFontNodes[i];

		GrpShaderFont *shader = NULL;

		if (n->customchar == 0) {
			if (pFontShaders[n->color] == NULL) {
				pFontShaders[n->color] = new GrpShaderFont(false);
				pFontShaders[n->color]->setPos(n->color, pSysTex);

				CCLOG("Create Font Shader %d", n->color);
			}

			shader = pFontShaders[n->color];
		} else {
			if (pFontExtraShaders[n->color] == NULL) {
				pFontExtraShaders[n->color] = new GrpShaderFont(true);
				pFontExtraShaders[n->color]->setPos(n->color, pSysTex);

				CCLOG("Create Extra Font Shader %d", n->color);
			}

			shader = pFontExtraShaders[n->color];
		}


		curWidth = n->Init(shader);
		this->reorderChild(n, i);

		if (n->txt == " ")
			curWidth = (int)(18*ScreenCommon::game_s/2.0f);

		if (lastposy != n->y || lastposy == -1.0f) {
			curPosX = 0;
			lastWidth = 0;
		}

		curPosX += (curWidth + lastWidth)/2;
		lastWidth = curWidth;

		n->x = curPosX;
		lastposy = n->y;

		if (!n->isWaitKey) {
			n->setPosition( ccp(n->x, n->y) );
			n->setVisible(false);
		}
	}
}

void GrpFont::showAll()
{
	msgStartTime = 0;
	for(int i=0; i<pFontNodes.GetLength(); i++)
		pFontNodes[i]->setVisible(true);
}

int GrpFont::showStart(int curtime)
{
	msgStartTime = curtime;
	pos = 0;
	int sumtime = 0;
	for(int i=0; i<pFontNodes.GetLength(); i++)
		sumtime += pFontNodes[i]->delay;

	return sumtime;
}

bool GrpFont::refresh(int curtime, bool allowAutoMsg)
{
	if (msgStartTime == 0) {
		isWaittingKey = false;
		return true;
	}

	while(pos < pFontNodes.GetLength()) {
		GrpFontNode *n = pFontNodes[pos];

		if (n->isWaitKey && pGM != NULL) {
			int key_enter = pGM->ctl->getKey(rkc_enter);

			
			MgrSetting *s = MgrSetting::getInstance();

			if (	(s->getBool(RST_ETC_AUTOMSG) && key_enter > 0 && allowAutoMsg) || 
				(keyLastEnter != key_enter && key_enter > 0))
					pos++;
			keyLastEnter = key_enter;
			isWaittingKey = true;
			return false;
		} else {
			if (msgStartTime + n->delay < curtime) {
				n->setVisible(true);
				msgStartTime += n->delay;
				pos++;
			} else {
				return false;
			}
			isWaittingKey = false;
		}
	}

	if (msgStartTime + lastMsgDelay >= curtime) {
		return false;
	} else {
		isWaittingKey = false;
		return true;
	}

}

bool GrpFont::isWaitKey()
{
	return isWaittingKey;
}
