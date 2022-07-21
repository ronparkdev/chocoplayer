#include "GrpFontL.h"

#include "../COMMON/ScreenCommon.h"

GrpFontL::GrpFontL( GameMain *main /*= NULL*/ )
{
	m_main = main;
	fontScale = ScreenCommon::game_s;
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

GrpFontL::~GrpFontL()
{
	removeText();

	for(int i=0; i<20; i++) {
		if (pFontShaders[i] != NULL)
			delete pFontShaders[i];

		if (pFontExtraShaders[i] != NULL)
			delete pFontExtraShaders[i];
	}
}

void GrpFontL::reloadShaders()
{
	for(int i=0; i<20; i++) {
		if (pFontShaders[i] != NULL)
			pFontShaders[i]->reload();

		if (pFontExtraShaders[i] != NULL)
			pFontExtraShaders[i]->reload();
	}
}

void GrpFontL::setText( const TString &str)
{
	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name);
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	if (pTex == NULL)
		return;

	setText(str, pTex);
}

void GrpFontL::setText( const TString &str, CCTexture2D *pSysTex)
{
	if (oriText == str)
		return;

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

	//CCLOG(str.getTextUTF8());

	//초기화
	removeText();

	oriText = str;

	if (str.isEmpty())
		return;

	text = "";

	int offset = 0;
	int curPosX = 0;
	int curPosY = -lineHeight/2;
	int curColor = 0;
	bool curMsgSkip = false;

	int startright = -1;
	int startrightpos = -1;

	//전처리 : 문장에 쓰이는 변수를 대입함
	bool find_sys = false;

	bool find_setforce = false;

	ArrayList<int> setforce_x;
	ArrayList<int> setforce_strpos;
	ArrayList<int> setforce_strposf;
	ArrayList<int> setforce_strnpos;
	ArrayList<int> setforce_strnposf;
	ArrayList<bool> setforce_isright;
	int setforce_len = 0;

	ArrayList<int> setcolor_id;
	ArrayList<int> setcolor_pos;
	ArrayList<int> setcolor_npos;
	int setcolor_len = 0;

	int lastpos = 0;
	for(int i=0; i<str.getLength(); i++) {
		int c = str.charAt(i);

		if (!find_sys) {
			switch(c) {
			case '\\':
			case 0xA5: //YEN(FOR JAPANESE) //TODO
				{
					text = text + str.substring(lastpos, i);
					lastpos = i+1;

					find_sys = true;
					break;
				}
			case '>':
				{
					if (find_setforce) {
						text = text + str.substring(lastpos, i);
						lastpos = i+1;

						int k = setforce_len;
						setforce_strposf[k] = text.getLength(); 
						setforce_len++;
						find_setforce = false;
					}
					break;
				}
			}
		} else {
			//toupper
			if ('a' <= c && c <= 'z') c += 'A' - 'a';

			switch(c) {
			case '\\':
			case 0xA5: //YEN(FOR JAPANESE) //TODO
				{
					text = text + str.substring(lastpos, i-1) 
						+ "\\";

					lastpos = i+1;
					break;
				}
			case 'L'://Start Left Font (ONLY CHOCOPLAYER)
			case 'R'://Start Right Font (ONLY CHOCOPLAYER)
			case 'N'://CharacterName
			case 'V'://Print Variance
			case 'C'://Color
			case 'S'://SetPosForce
				{
					int sf = str.indexOf('[', i+1);
					if (sf == -1)break;
					int st = str.indexOf(']', sf+1);
					if (st == -1)break;

					int val = str.substring(sf+1, st).getInt();

					switch(c) {
					case 'N'://CharacterName
						{
							if (m_main != NULL)
								text = text + str.substring(lastpos, i-1) 
								+ m_main->lsd.Character[val-1].name;
							break;
						}
					case 'V'://Print Variance
						{
							if (m_main != NULL)
								text = text + str.substring(lastpos, i-1) 
								+ m_main->lsd.System.variables[val-1];
							break;
						}

					case 'C'://MsgColor
						{
							setcolor_id.Add(val);
							setcolor_pos.Add(text.getLength());
							setcolor_len++;
							break;
						}


					case 'L'://Start Left Font (ONLY CHOCOPLAYER)
					case 'R'://Start Right Font (ONLY CHOCOPLAYER)
						{
							if (find_setforce) {
								text = text + str.substring(lastpos, i);
								lastpos = i+1;

								int k = setforce_len;
								setforce_strposf[k] = text.getLength(); 
								setforce_len++;
								find_setforce = false;
							}

							int k = setforce_len;

							if (setforce_x.GetLength() >= k) {
								setforce_x		.Add();
								setforce_isright.Add();
								setforce_strpos	.Add();
								setforce_strposf.Add();
							}

							setforce_x[k]		= str.substring(sf+1, st).getInt() * fontScale;
							setforce_isright[k]	= (c == 'R');
							setforce_strpos[k]	= text.getLength();

							find_setforce = true;

							break;
						}
					}

					i = st;
					lastpos = st+1;
					break;
				}

			default:
				{
					text = text + str.substring(lastpos, i);
					lastpos = i+1;
					break;
				}
			}
			find_sys = false;
		}
	}

	text = text + str.substring(lastpos, str.getLength());

	if (find_setforce) {
		int k = setforce_len;
		setforce_strposf[k] = text.getLength(); 
		setforce_len++;
		find_setforce = false;
	}

	setforce_strnpos	.SetLength(setforce_len);
	setforce_strnposf	.SetLength(setforce_len);
	setcolor_npos		.SetLength(setcolor_len);


	//FOR DEBUGING
	// 	CCLOG(m_str.getTextUTF8());
	// 	for(int i=0; i<setforce_len; i++)
	// 		CCLOG((_ST("setforce") + setforce_strpos[i] + "->" + setforce_strposf[i] + "(" + (setforce_isright[i]?"R":"L") + setforce_x[i] + ")").getTextUTF8());
	// 	for(int i=0; i<setcolor_len; i++)
	// 		CCLOG((_ST("setcolor") + setcolor_pos[i] + "~(" + setcolor_id[i] + ")").getTextUTF8());


	for(int i=0; i<setforce_len; i++) {
		setforce_strnpos[i] = -1;
		setforce_strnposf[i] = -1;
	}

	for(int i=0; i<setcolor_len; i++)
		setcolor_npos[i] = -1;

	int m_strlen = text.getLength();
	lastpos = 0;

	bool isSpecialFont = false;
	for(int i=0; i<m_strlen; i++) {
		int c = text.charAt(i);

		if (!isSpecialFont) {
			if (c == '$') {
				isSpecialFont = true;

				if (lastpos < i) {
					GrpFontNode *n = new GrpFontNode();
					n->txt = text.substring(lastpos, i);
					n->customchar = 0;
					this->addChild(n);
					pFontNodes.Add(n);

					lastpos = i;
					offset++;
				}
			}
		} else {
			isSpecialFont = false;

			GrpFontNode *n = new GrpFontNode();
			n->customchar = c;
			this->addChild(n);
			pFontNodes.Add(n);

			lastpos = i+1;
			offset++;
		}

		for(int j=0; j<setcolor_len; j++)
			if (setcolor_pos[j] == i && setcolor_npos[j] == -1) {
				setcolor_npos[j] = offset + ((lastpos < i)?1:0);

				if (lastpos < i) {
					GrpFontNode *n = new GrpFontNode();
					n->txt = text.substring(lastpos, i);
					n->customchar = 0;
					this->addChild(n);
					pFontNodes.Add(n);

					lastpos = i;
					offset++;
				}
			}

			for(int j=0; j<setforce_len; j++) {
				bool fixed = false;
				if (setforce_strpos[j] == i && setforce_strnpos[j] == -1) {
					setforce_strnpos[j] = offset + ((lastpos < i)?1:0);
					fixed = true;
				}

				if (setforce_strposf[j] == i && setforce_strnposf[j] == -1) {
					setforce_strnposf[j] = offset + ((lastpos < i)?1:0);
					fixed = true;
				}

				if (fixed && lastpos < i) {
					GrpFontNode *n = new GrpFontNode();
					n->txt = text.substring(lastpos, i);
					n->customchar = 0;
					this->addChild(n);
					pFontNodes.Add(n);

					lastpos = i;
					offset++;
				}

			}
	}

	if (lastpos < m_strlen) {
		GrpFontNode *n = new GrpFontNode();
		n->txt = text.substring(lastpos, m_strlen);
		n->customchar = 0;
		this->addChild(n);
		pFontNodes.Add(n);

		offset++;
	}

	for(int i=0; i<pFontNodes.GetLength(); i++) {
		pFontNodes[i]->y = curPosY;
		pFontNodes[i]->fontSize = fontSize;
		pFontNodes[i]->color = 0;
	}

	for(int j=0; j<setforce_len; j++) {
		if (setforce_strposf[j] == text.getLength() && setforce_strnposf[j] == -1)
			setforce_strnposf[j] = offset;
	}


	//FOR DEBUGING
	// 	TString tempstr;
	// 	for(int i=0; i<m_data.GetLength(); i++)
	// 		tempstr = tempstr + "@" + m_data[i].m_char;
	// 	CCLOG(tempstr.getTextUTF8());
	// 	for(int i=0; i<setforce_len; i++)
	// 		CCLOG((_ST("setforce") + setforce_strnpos[i] + "->" + setforce_strnposf[i] + "(" + (setforce_isright[i]?"R":"L") + setforce_x[i] + ")").getTextUTF8());
	// 	for(int i=0; i<setcolor_len; i++)
	// 		CCLOG((_ST("setcolor") + setcolor_npos[i] + "~(" + setcolor_id[i] + ")").getTextUTF8());


	pFontNodes.ChangeMode(ArrayMode);

	int lastWidth = 0, curWidth;
	curPosX = lastWidth = 0;
	for(int i=0; i<pFontNodes.GetLength(); i++) {
		GrpFontNode *n = pFontNodes[i];

		for(int j=setcolor_len-1; j>=0; j--)
			if (setcolor_npos[j] <= i && setcolor_npos[j] != -1) {
				n->color = setcolor_id[j];
				break;
			}

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

		curPosX += (curWidth + lastWidth)/2;
		lastWidth = curWidth;

		n->x = curPosX;

		n->setPosition( ccp(n->x, n->y) );
		n->setVisible(true);
	}

	for(int i=0; i<setforce_len; i++) {
		if (setforce_strnpos[i] < 0 || setforce_strnposf[i] < 0)
			continue;

		curPosX = setforce_x[i];
		lastWidth = 0;
		if (setforce_isright[i]) {
			for(int j=setforce_strnpos[i]; j<setforce_strnposf[i]; j++) {
				GrpFontNode *n = pFontNodes[j];

				curWidth = n->getWidth();
				curPosX += (curWidth + lastWidth)/2;
				lastWidth = curWidth;

				n->x = curPosX;

				n->setPosition( ccp(n->x, n->y));
			}
		} else {
			for(int j=setforce_strnposf[i]-1; j>=setforce_strnpos[i]; j--) {
				GrpFontNode *n = pFontNodes[j];

				curWidth = n->getWidth();
				curPosX -= (curWidth + lastWidth)/2;
				lastWidth = curWidth;

				n->x = curPosX;

				n->setPosition( ccp(n->x, n->y));
			}
		}
	}
}

void GrpFontL::removeText()
{
	for(int i=0; i<pFontNodes.GetLength(); i++)
		this->removeChild(pFontNodes[i]);

	pFontNodes.RemoveAll();

	oriText = "";
}

void GrpFontL::setOpacity( float val )
{
	for(int i=0; i<pFontNodes.GetLength(); i++)
		pFontNodes[i]->setOpacity(val);
}

int GrpFontL::getWidth()
{
	int width = 0;

	for(int i=0; i<pFontNodes.GetLength(); i++)
		width += pFontNodes[i]->getWidth();

	return width;
}




