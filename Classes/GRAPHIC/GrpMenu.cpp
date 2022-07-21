#include "GrpMenu.h"

GrpMenu::GrpMenu( GameMain *main, int row, int col, int width, bool autowidth )
{
	this->pGM = main;
	this->row = row;
	this->col = col;
	this->width = width;
	this->isAutoWidth = autowidth;
	
	pCtl = main->ctl;
	height = (row+1)*16;

	curId = curPage = 0;
	nodesPerPage = row * col;

	int texID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
	CCTexture2D *pTexSystem = IBuffer::getInstance()->get(imgSystem, texID);
	
	this->pTexSystem = pTexSystem;

	isInControl_ = false;

	pMsgbox = new GrpMsgbox(pGM, width, height);
	this->addChild(pMsgbox, 0);

	pSelection = new GrpSelection(pTexSystem, (width-8)/col, 16);
	pSelection->setVisible(false);
	this->addChild(pSelection,1);

	pFonts = new GrpFontL*[nodesPerPage];
	for(int offset=0; offset<nodesPerPage; offset++) {
		pFonts[offset] = new GrpFontL();

		int c = offset % col;
		int r = offset / col;
		float posX = ((8+c*(width-8)/col) - width/2);
		float posY = (height/2 - (8+r*16));


		pFonts[offset]->setPosition(ccp(posX,posY));
		this->addChild(pFonts[offset], 2);
	}
	widthPerOne = (width-16)/col;

	pSysUp = new GrpSysUp(pTexSystem);
	pSysUp	->setPosition(ccp(0,  ((height-4)/2)));
	this->addChild(pSysUp,3);

	pSysDown = new GrpSysDown(pTexSystem);
	pSysDown	->setPosition(ccp(0, (-(height-4)/2)));
	this->addChild(pSysDown,3);

	refreshPos();
	refreshPage();

	CCNode::autorelease();
}

GrpMenu::~GrpMenu()
{
	for(int i=0; i<nodesPerPage; i++)
		this->removeChild(pFonts[i]);

	delete []pFonts;

	this->removeChild(pMsgbox);
	this->removeChild(pSelection);
	this->removeChild(pSysUp);
	this->removeChild(pSysDown);
}

void GrpMenu::reloadShaders()
{
	for(int i=0; i<nodesPerPage; i++)
		pFonts[i]->reloadShaders();
}

void GrpMenu::setInControl( bool val )
{
	isInControl_ = val;
	pSelection->setVisible(val);
	refreshPos();

	initInput();
}

bool GrpMenu::isInControl()
{
	return isInControl_;
}

void GrpMenu::refreshNodes( const ArrayList<TString> &nodes )
{
	this->nodes = nodes;
	if (0 < nodes.GetLength() && nodes.GetLength() <= curId) {
		curId = nodes.GetLength() - 1;
		curPage = curId / nodesPerPage;
	}
	refreshPage();
}

void GrpMenu::refreshPos()
{
	int offset = curId % nodesPerPage;
	int c = offset % col;
	int r = offset / col;

	float posX = (4+c*(width-8)/col - width/2 + (width-8)/col/2);
	float posY = (height/2 -(16+r*16));

	pSelection->setPosition(ccp(posX,posY));
}

void GrpMenu::refreshPage()
{
	int offset = 0;
	int maxwidth = 0;
	for(int i = curPage*nodesPerPage ; i<_MIN((curPage+1)*nodesPerPage, nodes.GetLength()); i++) {
		pFonts[offset]->setText(nodes[i], pTexSystem);
		maxwidth = _MAX(pFonts[offset]->getWidth(), maxwidth);
		offset++;
	}
	if (isAutoWidth) {
		maxwidth = (maxwidth / ScreenCommon::game_s) + 16;

		this->removeChild(pMsgbox);
		pMsgbox = new GrpMsgbox(pGM, maxwidth, height);
		this->addChild(pMsgbox, 0);

		GrpSelection *tmp = new GrpSelection(pTexSystem, (maxwidth-8)/col, 16);
		tmp->setVisible(pSelection->isVisible());
		tmp->setPosition(pSelection->getPosition());
		this->removeChild(pSelection);
		this->addChild(tmp,1);
		pSelection = tmp;
	}


	for(; offset <nodesPerPage; offset++) {
		pFonts[offset]->setText("", pTexSystem);
	}

	if (nodes.GetLength() > nodesPerPage) {
		pSysUp->setVisible(curPage != 0);
		pSysDown->setVisible(curPage != nodes.GetLength()/nodesPerPage);
	} else {
		pSysUp->setVisible(false);
		pSysDown->setVisible(false);
	}
}

void GrpMenu::initInput()
{
	keyLastEnter	= pCtl->getKey(rkc_enter);
	keyLastEsc		= pCtl->getKey(rkc_esc);
	keyLastArrow	= pCtl->getKey(rkc_arrow);
}

int GrpMenu::getInput()
{
	if (!isInControl_)
		return GRP_MENU_NOTWORK;

	int key_enter	= pCtl->getKey(rkc_enter);
	int key_esc		= pCtl->getKey(rkc_esc);
	int key_arrow	= pCtl->getKey(rkc_arrow);

	if (keyLastEnter != key_enter) {
		keyLastEnter = key_enter;

		if (key_enter > 0) {
			pGM->snd.PlaySound(pGM->lsd.System.se_decision);
			return curId;
		}
	}

	if (keyLastEsc != key_esc) {
		keyLastEsc = key_esc;

		if (key_esc > 0) {
			pGM->snd.PlaySound(pGM->lsd.System.se_cancel);
			return GRP_MENU_CANCEL;
		}
	}

	if (keyLastArrow != key_arrow) {
		keyLastArrow = key_arrow;

		if (nodes.GetLength() == 0)
			return GRP_MENU_NOTWORK;

		int newindex = curId;
		if (key_arrow == rkca_up) {
			if (nodes.GetLength() > col) {
				newindex -= col;
				if (newindex < 0)
					newindex += nodes.GetLength();
			}
		} else if (key_arrow == rkca_down) {
			if (nodes.GetLength() > col) {
				newindex += col;
				if (newindex >= nodes.GetLength())
					newindex -= nodes.GetLength();
			}
		} else if (key_arrow == rkca_left && col > 1) {
			if (nodes.GetLength() > 1) {
				newindex -= 1;
				if (newindex < 0)
					newindex += nodes.GetLength();
			}
		} else if (key_arrow == rkca_right && col > 1) {
			if (nodes.GetLength() > 1) {
				newindex += 1;
				if (newindex >= nodes.GetLength())
					newindex -= nodes.GetLength();
			}
		} else {
			return GRP_MENU_NOTWORK;
		}

		if (curId != newindex)
			pGM->snd.PlaySound(pGM->lsd.System.se_cursor);

		curId = newindex;

		int newpage = newindex / nodesPerPage;
		if (newpage != curPage) {
			curPage = newpage;
			refreshPage();
		}

		refreshPos();
	}

	return GRP_MENU_NOTWORK;
}

int GrpMenu::getIndex()
{
	return curId;
}

void GrpMenu::setIndex( int val )
{
	if (val < 0 || val >= nodes.GetLength())
		return;

	curId = val;
	int newpage = val / nodesPerPage;
	if (newpage != curPage) {
		curPage = newpage;
		refreshPage();
	}
	refreshPos();
}
