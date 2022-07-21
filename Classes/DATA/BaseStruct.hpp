
template <typename T>
void BaseStruct::serialize( StructLoader &sl, T &val )
{
    if (typeid(val) == typeid(BaseStruct)) {
        ((BaseStruct &)val).serialize(sl);
    } else {
        if (sl.getMode() == StructLoaderModeInit) {

        } else if (sl.isOpened()) {
            if (sl.getMode() == StructLoaderModeRead)
                sl >> val;
            else
                sl << val;
        }
    }
}

template <typename T>
void BaseStruct::serialize( StructLoader &sl, int code, T &val )
{
	if (sl.getMode() == StructLoaderModeInit) {

	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(code)) {
				sl.intoCode();
				sl >> size;
				int newpos = sl.getOffset() + size;
				sl >> val;
				sl.setOffset(newpos);
			}
		} else {
			sl << code;
			int startpos = sl.getOffset();
			sl << val;
			sl.writeStructSize(startpos);
		}
	}
}

template <typename T>
void BaseStruct::serialize( StructLoader &sl, T &val, const T &defval )
{
	if (sl.getMode() == StructLoaderModeInit) {
		val = defval;
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			sl >> BaseStruct::size;
			sl >> val;
		} else {
			int startpos = sl.getOffset();
			sl << val;
			sl.writeStructSize(startpos);

		}
	}
}

template <typename T>
void BaseStruct::serialize( StructLoader &sl, int code, T &val, const T &defval )
{
	if (sl.getMode() == StructLoaderModeInit) {
		val = defval;
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(code)) {
				sl.intoCode();
				sl >> size;
				int newpos = sl.getOffset() + size;
				sl >> val;
				sl.setOffset(newpos);

// 				TString str;
// 				str.setTextHEX(code);
// 				str = str + "\t";
// 				cout<<str.getTextUTF8();
// 				cout<<val<<endl;
			}
		} else {
			if (val != defval) {
				sl << code;
				int startpos = sl.getOffset();
				sl << val;
				sl.writeStructSize(startpos);
			}
		}
	}
}

template <typename T>
void BaseStruct::serialize( StructLoader &sl, ArrayList<T> &val )
{
    if (sl.getMode() == StructLoaderModeInit) {
        val.RemoveAll();
    } else if (sl.isOpened()) {
        if (sl.getMode() == StructLoaderModeRead) {
            int size;
            sl >> size;
            size /= sizeof(T);

            val.RemoveAll();
            val.ChangeMode(ArrayMode);
            val.SetLength(size);
            for(int i=0; i<size; i++)
                sl >> val[i];

        } else {
            int startpos = sl.getOffset();
            for(int i=0; i<val.GetLength(); i++)
                sl << val[i];
            sl.writeStructSize(startpos);
        }
    }
}

template <typename T>
void BaseStruct::serialize( StructLoader &sl, int code, ArrayList<T> &val )
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

//template<typename T>
//void BaseStruct::serialize( StructLoader &sl, int code, ArrayList<T> &val )
//{
//	if (sl.getMode() == StructLoaderModeInit) {
//		serialize(sl, val);
//	} else if (sl.isOpened()) {
//		if (sl.getMode() == StructLoaderModeRead) {
//			if (sl.isNextCode(code)) {
//				sl.intoCode();
//				serialize(sl, val);
//			} 
//		} else {
//			sl << code;
//			serialize(sl, val);
//		}
//	}
//}

