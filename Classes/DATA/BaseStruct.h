#ifndef __R2K_DATA__STRUCT_DEFINE__
#define __R2K_DATA__STRUCT_DEFINE__
#include "../COMMON/DebugingCode.h"

#include "ArrayList.h"
class StructLoader;
class TString;

#define STRUCT_DEFINE_STRING_BUFFER 512

using namespace std;

class BaseStructException_WrongStruct{};

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char ubyte;

class BaseStruct{
public:
	static bool showdebug;
	
	BaseStruct();

	virtual void init();
	virtual void serialize(StructLoader &sl);

	static int size;

	int getIndex();
	void setIndex( int id );
protected:
    static TString default_string;
    int m_index;
    
    void serialize(StructLoader *sl, int code, double &val);
    void serialize(StructLoader *sl, int code, double &val, const double &defval);
    void serialize(StructLoader *sl, int code, ArrayList<double> &vals);
    
    void serialize(StructLoader *sl, int code, float &val);
    void serialize(StructLoader *sl, int code, float &val, const float &defval);
    void serialize(StructLoader *sl, int code, ArrayList<float> &vals);
    
    void serialize(StructLoader *sl, int code, int &val);
    void serialize(StructLoader *sl, int code, int &val, const int &defval);
    void serialize(StructLoader *sl, int code, ArrayList<int> &vals);
    
    void serialize(StructLoader *sl, int code, uint &val);
    void serialize(StructLoader *sl, int code, uint &val, const uint &defval);
    void serialize(StructLoader *sl, int code, ArrayList<uint> &vals);
    
    void serialize(StructLoader *sl, int code, ushort &val);
    void serialize(StructLoader *sl, int code, ushort &val, const ushort &defval);
    void serialize(StructLoader *sl, int code, ArrayList<ushort> &vals);
    
    void serialize(StructLoader *sl, int code, ubyte &val);
    void serialize(StructLoader *sl, int code, ubyte &val, const ubyte &defval);
    void serialize(StructLoader *sl, int code, ArrayList<ubyte> &vals);
    
    void serialize(StructLoader *sl, int code, bool &val);
    void serialize(StructLoader *sl, int code, bool &val, const bool &defval);
    void serialize(StructLoader *sl, int code, ArrayList<bool> &vals);
    
    void serialize(StructLoader *sl, int code, BaseStruct &val);
    void serialize(StructLoader *sl, int code, ArrayList<BaseStruct> &val);
    
    void serialize(StructLoader *sl, int code, TString &val);
    void serialize(StructLoader *sl, int code, TString &str, const TString &defstr, bool useNullStr = false, bool usePublicNullStr = false);
    
	void serializeEmpty(StructLoader &sl, int code);
    
private:
    template <typename T>
    void serializeT(StructLoader &sl, T &val);
    template <typename T>
    void serializeT(StructLoader &sl, int code, T &val);
  
    template <typename T>
    void serializeT(StructLoader &sl, T &val, const T &defval);
    template <typename T>
    void serializeT(StructLoader &sl, int code, T &val, const T &defval);
    
    template <typename T>
    void serializeT(StructLoader &sl, ArrayList<T> &val);
    template <typename T>
    void serializeT(StructLoader &sl, int code, ArrayList<T> &val);
};

#include "BaseStruct.hpp"

#endif
