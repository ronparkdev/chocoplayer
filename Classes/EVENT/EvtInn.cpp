#include "EvtMain.h"

void EvtMain::doInnEvent(int type,int cost) {
	GlbEvent evt;
	GlbEventNode *node;
	ArrayList<GlbEventNode> &e = evt.data;
	LdbVoca &voca = pGM->ldb.dVoca;

	bool enoughMoney = pGM->lsd.State.money >= cost;

	node = &(e.Add());
	node->code=10110;//MessageLine1
	node->string="\\$" + voca.inn_a_greeting1[type] + " " + cost + voca.gold +  voca.inn_a_greeting2[type] ;
	
	node = &(e.Add());
	node->code=20110;//MessageLine2
	node->string=voca.inn_a_greeting3[type];

	node = &(e.Add());
	node->code=10140;//ShowChoice
	node->parameters.SetLength(1);
	node->parameters[0] = 2;
	node->string = voca.yes + "/" + voca.no;
	
	node = &(e.Add());
	node->code=20140;//SelectAccept
	node->parameters.SetLength(1);
	node->parameters[0] = 0;
	if (enoughMoney)
		node->string=voca.inn_a_accept[type];
	else
		node->string="\\C[1]"+voca.inn_a_accept[type];

	if (enoughMoney) {
		node = &(e.Add());
		node->code=11530;//Memory BGM
		
		GlbMusic &mus = pGM->lsd.System.music_inn;
		node = &(e.Add());
		node->code=11510;//Play Music
		node->indent = 1;
		node->string=mus.name;
		node->parameters.SetLength(4);
		node->parameters[0] = mus.fadein;
		node->parameters[1] = mus.volume;
		node->parameters[2] = mus.tempo;
		node->parameters[3] = mus.balance;
		
		node = &(e.Add());
		node->code=11010;//EraseScr
		node->indent = 1;
		
		node = &(e.Add());
		node->code=11410;//Wait 5 second
		node->indent = 1;
		node->parameters.SetLength(2);
		node->parameters[0] = 50;
		node->parameters[1] = 0;
		
		node = &(e.Add());
		node->code=10490;//Heal All
		node->indent = 1;
		
		node = &(e.Add());
		node->code = 10310;
		node->parameters.SetLength(3);
		node->parameters[0] = 1;
		node->parameters[1] = 0;
		node->parameters[2] = cost;
		
		node = &(e.Add());
		node->code=11020;//ShowScr
		node->indent = 1;
		
		node = &(e.Add());
		node->code=11540;//play Memorized BGM 
		
	}
	node = &(e.Add());
	node->code=10;//End
	node->indent = 1;
	
	node = &(e.Add());
	node->code=20140;//SelectCancel
	node->parameters.SetLength(1);
	node->parameters[0] = 1;
	node->string = voca.inn_a_cancel[type];

	node = &(e.Add());
	node->code=20141;
	node->indent = 0;
	
	node = &(e.Add());
	node->code=0;

	evt.data.ChangeMode(ArrayMode);

	doEvent(evt);
}