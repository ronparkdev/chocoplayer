#include "BaseStruct.h"

#include "StructLoader.h"
#include "ArrayList.h"

bool BaseStruct::showdebug = false;
int BaseStruct::size;
//int BaseStruct::index;//FOR TEST

TString BaseStruct::default_string;

BaseStruct::BaseStruct() 
{
	if (default_string.isEmpty()) {
		default_string.setChar(0x1);
	}
}

void BaseStruct::init()
{
	StructLoader initStructLoader;
	initStructLoader.setInitMode();

	serialize(initStructLoader);
}

void BaseStruct::serialize( StructLoader &sl )
{

}


//BaseStruct

void BaseStruct::serialize( StructLoader &sl, BaseStruct &val )
{
    val.serialize(sl);
}

void BaseStruct::serialize( StructLoader &sl, int code, BaseStruct &val )
{
    if (sl.getMode() == StructLoaderModeInit) {
        val.serialize(sl);
    } else if (sl.isOpened()) {
        if (sl.getMode() == StructLoaderModeRead) {
            if (sl.isNextCode(code)) {
                sl.intoCode();
                sl >> size;
                int newpos = sl.getOffset() + size;
                val.serialize(sl);
                sl.setOffset(newpos);
            }
        } else {
            sl << code;
            int startpos = sl.getOffset();
            val.serialize(sl);
            sl.writeStructSize(startpos);
        }
    }
}

void BaseStruct::serialize( StructLoader &sl, int code, ArrayList<BaseStruct> &val )
{
    if (sl.getMode() == StructLoaderModeInit) {
        for(int i=0; i<val.GetLength(); i++) {
            m_index = i;
            val[i].serialize(sl);
        }
    } else if (sl.isOpened()) {
        if (sl.getMode() == StructLoaderModeRead) {
            val.ChangeMode(ArrayMode);
            
            int totalpage, arraysize, curpage;
            sl >> arraysize;
            int newpos = sl.getOffset() + arraysize;
            sl >> totalpage;
            
            int lastpage = val.GetLength();
            if (totalpage != lastpage)
                val.SetLength(totalpage);
            
            StructLoader sl_init;
            sl_init.setInitMode();
            for(int i=lastpage; i<totalpage; i++) {
                val[i].serialize(sl_init);
            }
            
            for(int i=0; i<totalpage; i++) {
                sl >> curpage;
                val[i].setIndex(curpage);
                val[i].serialize(sl);
            }
            sl.setOffset(newpos);
        } else {
            int size = val.GetSize();
            int startpos = sl.getOffset();
            sl << val.GetLength();
            for(int i=0; i<val.GetLength(); i++) {
                sl << val[i].getIndex();
                val[i].serialize(sl);
            }
            sl.writeStructSize(startpos);
        }
    }
}

//TString

void BaseStruct::serialize( StructLoader &sl, TString &str )
{
    
}

void BaseStruct::serialize( StructLoader &sl, int code, TString &str )
{
    
}

void BaseStruct::serialize( StructLoader &sl, int code, TString &str, const TString &defstr, bool useNullStr, bool usePublicNullStr )
{
	if (sl.getMode() == StructLoaderModeInit) {
		str = defstr;
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(code)) {
				sl.intoCode();
				serialize(sl, str);
				if (useNullStr) {
					if (usePublicNullStr) {
						if (str == default_string) {
							str = defstr;
						}
					} else {
						if (str.isEmpty()) {
							str = defstr;
						}
					}
				}
			}
		} else {
			if (useNullStr) {
				if (str == defstr) {
					sl << code;
					if (usePublicNullStr)
						serialize(sl, default_string);
					else
						sl << 0;

				} else {
					sl << code;
					serialize(sl, str);
				}
			} else {
				if (str != defstr) {
					sl << code;
					serialize(sl, str);
				}
			}
		}
	}
}

/*
void BaseStruct::serialize( StructLoader &sl, ArrayList<TString> &val )
{
	if (sl.getMode() == StructLoaderModeInit) {
		val.RemoveAll();
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			val.RemoveAll();
			val.ChangeMode(ArrayMode);

			int totalpage, arraysize, curpage;
			sl >> arraysize;
			sl >> totalpage;

			val.SetLength(totalpage);

			for(int i=1; i<=totalpage; i++) {
				sl >> curpage;
				if (curpage == i) {
					val.Add();
					BaseStruct::serialize(sl, val[i-1]);
				} else {
					throw BaseStructException_WrongStruct();
				}
			}	

		} else {
			int size = val.GetSize();
			sl << val.GetLength();
			int startpos = sl.getOffset();
			for(int i=1; i<=val.GetLength(); i++) {
				sl << i;
				BaseStruct::serialize(sl, val[i-1]);
			}
			sl.writeStructSize(startpos);
		}
	}
}

void BaseStruct::serialize( StructLoader &sl, int code, ArrayList<TString> &val )
{
	if (sl.getMode() == StructLoaderModeInit) {
		val.RemoveAll();
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(code)) {
				sl.intoCode();
				serialize(sl, val);
			} 
		} else {
			sl << code;
			serialize(sl, val);
		}
	}
}
*/

//Etc

void BaseStruct::serializeEmpty( StructLoader &sl, int code )
{
	if (sl.getMode() == StructLoaderModeInit) {

	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(code)) {
				sl.intoCode();
				sl >> size;
				sl.setOffset(sl.getOffset()+size);
			} 
		} else if (sl.getMode() == StructLoaderModeRead) {
			sl << code;
			sl << CODE_END_OF_STRUCT;
		}
	}
}

int BaseStruct::getIndex()
{
	return m_index;
}

void BaseStruct::setIndex( int id ) 
{
	m_index = id;
}
