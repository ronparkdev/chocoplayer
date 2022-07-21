#ifndef __R2K_DATA__GLB_EVENT__
#define __R2K_DATA__GLB_EVENT__

#include <iostream>
#include "../BaseStruct.h"


class GlbEventNode{
public:
	int		code;
	int		indent;
	TString	string;
	ArrayList<int> parameters;

	GlbEventNode() {
		code = 0;
		indent = 0;
		string = "";
	}
};

class GlbEvent : public BaseStruct
{
public:
	ArrayList<GlbEventNode> data;

public:
	GlbEvent() {

	}

	void init() {
		data.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeInit) {
		} else if (sl.getMode() == StructLoaderModeRead) {
			init();

			data.ChangeMode(ListMode);
			int datasize = BaseStruct::size;
			int poslimit = min(sl.getOffset() + datasize, sl.getLength());
			while(sl.getOffset() < poslimit) {
				GlbEventNode node;
				BaseStruct::serialize (sl, node.code);
				BaseStruct::serialize (sl, node.indent);
				BaseStruct::serialize (sl, node.string);

				int param_length;
				sl >> param_length;
				node.parameters.ChangeMode(ArrayMode);
				node.parameters.SetLength(param_length);
				for(int i=0; i<param_length; i++)
					sl >> node.parameters[i];
				data.Add(node);
			}
			data.ChangeMode(ArrayMode);
		} else {
			for(int i=0; i<data.GetLength(); i++) {
				GlbEventNode &node = data[i];

				BaseStruct::serialize (sl, node.code);
				BaseStruct::serialize (sl, node.indent);
				BaseStruct::serialize (sl, node.string);
				
				sl << node.parameters.GetLength();
				for(int j=0; j<node.parameters.GetLength(); j++)
					sl << node.parameters[j];
			}
		}
	}
};


#endif
