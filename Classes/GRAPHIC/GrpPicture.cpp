#include "GrpPicture.h"

GrpPicture::GrpPicture(GameMain *main )
{
	pGM = main;

	for(int i=0; i<GRP_PICTURE_MAX; i++) {
		pNodes[i] = new GrpPictureNode();
		this->addChild(pNodes[i], i);
	}

	lastFrame = pGM->lsd.System.frame_count;

	this->autorelease();
}

GrpPicture::~GrpPicture()
{
	for(int i=0; i<GRP_PICTURE_MAX; i++)
		this->removeChild(pNodes[i]);
}

void GrpPicture::draw(int framedelta)
{
	ArrayList<LsdPicture> &pics = pGM->lsd.Picture;

	for(int i=0; i<GRP_PICTURE_MAX; i++)
		pNodes[i]->Draw(framedelta, &(pics[i]));

}

void GrpPicture::setPic(GameMain *main,
			int id, int x, int y, 
			TString pic_name, bool fixMap, bool useBackcolor, 
			float scale, float upAlpha, float downAlpha, 
			int r, int g, int b, int gr, 
			int efftype, int effval )
{
	if (id <= 0 || id > GRP_PICTURE_MAX)
		return;

	LsdPicture &d = main->lsd.Picture[id-1];

	d.staX = d.finX = d.curX = x;
	d.staY = d.finY = d.curY = y;

	d.picName = pic_name;
	d.need_picname_check = true;

	d.is_fixed_to_map = fixMap;
	d.is_use_backcolor = useBackcolor;
	
	d.curS = d.finS = scale;
	d.curTA = d.finTA = upAlpha;
	d.curBA = d.finBA = downAlpha;

	d.curR = d.finR = r;
	d.curG = d.finG = g;
	d.curB = d.finB = b;
	d.curGr = d.finGr = gr;

	d.effect_type = efftype;

	if (efftype == 1)//rotation
		d.effect_cur_rotate_speed = d.effect_rotate_speed = effval;
	else if (efftype == 2)//wave
		d.effect_cur_wave_speed = d.effect_wave_speed = effval;
}

void GrpPicture::movePic(GameMain *main,
			int id, int x, int y, 
			float scale, float upAlpha, float downAlpha, 
			int r, int g, int b, int gr, 
			int efftype, int effval, 
			int runtime )
{
	if (id <= 0 || id > GRP_PICTURE_MAX)
		return;

	LsdPicture &d = main->lsd.Picture[id-1];

	d.staX = d.curX;
	d.staY = d.curY;
	d.finX = x;
	d.finY = y;

	d.finS = scale;
	d.finTA = upAlpha;
	d.finBA = downAlpha;

	d.finR = r;
	d.finG = g;
	d.finB = b;
	d.finGr = gr;

	d.effect_type = efftype;

	if (efftype == 1)//rotation
		d.effect_rotate_speed = effval;
	else if (efftype == 2)//wave
		d.effect_wave_speed = effval;

	if (runtime == 0)
		d.frame_left = 1;
	else
		d.frame_left = runtime * 6;


	//d.need_picname_check = true; //TODO ??? 왜해놓은거지
}

void GrpPicture::removePic(GameMain *main, int id)
{

	if (id <= 0 || id > GRP_PICTURE_MAX)
		return;

	LsdPicture &d = main->lsd.Picture[id-1];

	d.picName = "";
	d.need_picname_check = true;
}