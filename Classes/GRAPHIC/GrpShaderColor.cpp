#include "GrpShaderColor.h"

#include "../AppMulti.h"
#include "../COMMON/MultiOutter.h"

int GrpShaderColor::g_grpshader_need_lite = -1; //1:lite, other:normal

int GrpShaderColor::g_grpshader_count = 0;

void GrpShaderColor::chkLite() {

	//g_grpshader_need_lite = 1;return;;//FORDEBUG

    if (g_grpshader_need_lite != -1)
        return;

    TString model = MultiOutter::getModel();
    
//    if (model.indexOf("iPhone2") != -1 ||
//       model.indexOf("iPhone3") != -1 ||
//       model.indexOf("iPhone4") != -1) {
//        g_grpshader_need_lite = 1;
//    } else {
        g_grpshader_need_lite = 0;
//    }
}

bool GrpShaderColor::isLite() {
	return (g_grpshader_need_lite == 1);
}

GrpShaderColor::GrpShaderColor()
{
	glProg = NULL;

	lastR = lastG = lastB = lastGray = 0.0f;
	lastA = 1.0f;

	reload();

//	TString key;
//	key.setTextHEX((int)this);
//	CCShaderCache::sharedShaderCache()->addProgram(m_glProgram, key.getTextUTF8());
	
//	CCLog((_ST("Shader Count : ") + (++g_grpshader_count)).getTextUTF8());
}

GrpShaderColor::~GrpShaderColor()
{
	glProg->release();

//	CCLog((_ST("Shader Count : ") + (--g_grpshader_count)).getTextUTF8());
}

void GrpShaderColor::reload()
{
	char *filename;

	chkLite();

	if (isLite())
		filename = "filter_color_lite.fsh";
	else
		filename = "filter_color.fsh";

#if (SR_ANDROID == 1 || SR_IPHONE == 1)
	const char *path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename).c_str();
#else
	const char *path = filename;
#endif 

	CCString *data = CCString::createWithContentsOfFile(path);
	const GLchar * fragmentSource = (GLchar*) data->getCString();

	bool isFirst = 	(glProg == NULL);

	if (isFirst)
		glProg = new CCGLProgram();
	else
		glProg->reset();
	
	glProg->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, fragmentSource);
	glProg->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	glProg->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	glProg->link();
	glProg->updateUniforms();

	ul_AddR = glGetUniformLocation(glProg->getProgram(), "u_AddR");
	ul_AddG = glGetUniformLocation(glProg->getProgram(), "u_AddG");
	ul_AddB = glGetUniformLocation(glProg->getProgram(), "u_AddB");
	ul_GrPer = glGetUniformLocation(glProg->getProgram(), "u_GrPer");
	ul_Alpha = glGetUniformLocation(glProg->getProgram(), "u_Alpha");

	glProg->use();
	glUniform1f(ul_AddR,	lastR);
	glUniform1f(ul_AddG,	lastG);
	glUniform1f(ul_AddB,	lastB);
	glUniform1f(ul_GrPer,	lastGray);
	glUniform1f(ul_Alpha,	1.0f - lastA);


// 	TString key;
// 	key.setTextHEX((int)this);
// 	CCShaderCache::sharedShaderCache()->addProgram(m_glProgram, key.getTextUTF8());
}


void GrpShaderColor::Init()
{

}

void GrpShaderColor::setShader( CCNode *pNode )
{
	pNode->setShaderProgram(glProg);
}

void GrpShaderColor::setColor( float r, float g, float b, float gr )
{
	if (lastR == r && lastG == g && lastB == b && lastGray == gr)
		return;

	glProg->use();
	glUniform1f(ul_AddR,	r);//TODO CHK
	glUniform1f(ul_AddG,	g);
	glUniform1f(ul_AddB,	b);
	glUniform1f(ul_GrPer,	gr);

	lastR = r;
	lastG = g;
	lastB = b;
	lastGray = gr;
}

void GrpShaderColor::setAlpha( float alpha )
{
	if (lastA == alpha)
		return;

	glProg->use();
	glUniform1f(ul_Alpha,	1.0f - alpha);

	lastA = alpha;
}
