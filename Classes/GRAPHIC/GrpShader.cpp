#include "GrpShader.h"

#include "../AppMulti.h"
#include "../COMMON/MultiOutter.h"

int g_grpshader_need_lite = -1; //1:lite, other:normal

int g_grpshader_count = 0;

void GrpShader::chkLite(){

	//g_grpshader_need_lite = 1;return;;//FORDEBUG

    if(g_grpshader_need_lite != -1)
        return;

    TString model = MultiOutter::getModel();
    
//    if(model.indexOf("iPhone2") != -1 ||
//       model.indexOf("iPhone3") != -1 ||
//       model.indexOf("iPhone4") != -1){
//        g_grpshader_need_lite = 1;
//    }else{
        g_grpshader_need_lite = 0;
//    }
}

bool GrpShader::isLite(){
	return (g_grpshader_need_lite == 1);
}

GrpShader::GrpShader()
{
	m_glProgram = NULL;

	m_last_r = m_last_g = m_last_b = m_last_gr = 0.0f;
	m_last_alpha = 1.0f;

	reload();

//	TString key;
//	key.setTextHEX((int)this);
//	CCShaderCache::sharedShaderCache()->addProgram(m_glProgram, key.getTextUTF8());
	
//	CCLog((_ST("Shader Count : ") + (++g_grpshader_count)).getTextUTF8());
}

GrpShader::~GrpShader()
{
	m_glProgram->release();

//	CCLog((_ST("Shader Count : ") + (--g_grpshader_count)).getTextUTF8());
}

void GrpShader::reload()
{
	char *filename;

	chkLite();

	if(isLite())
		filename = "color_filter_lite.fsh";
	else
		filename = "color_filter.fsh";

#if (SR_ANDROID == 1 || SR_IPHONE == 1)
	const char *path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename).c_str();
#else
	const char *path = filename;
#endif 

	CCString *data = CCString::createWithContentsOfFile(path);
	const GLchar * fragmentSource = (GLchar*) data->getCString();

	bool isFirst = 	(m_glProgram == NULL);

	if(isFirst)
		m_glProgram = new CCGLProgram();
	else
		m_glProgram->reset();
	
	m_glProgram->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, fragmentSource);
	m_glProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	m_glProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	m_glProgram->link();
	m_glProgram->updateUniforms();

	ul_AddR = glGetUniformLocation(m_glProgram->getProgram(), "u_AddR");
	ul_AddG = glGetUniformLocation(m_glProgram->getProgram(), "u_AddG");
	ul_AddB = glGetUniformLocation(m_glProgram->getProgram(), "u_AddB");
	ul_GrPer = glGetUniformLocation(m_glProgram->getProgram(), "u_GrPer");
	ul_Alpha = glGetUniformLocation(m_glProgram->getProgram(), "u_Alpha");

	m_glProgram->use();
	glUniform1f(ul_AddR,	m_last_r);
	glUniform1f(ul_AddG,	m_last_g);
	glUniform1f(ul_AddB,	m_last_b);
	glUniform1f(ul_GrPer,	m_last_gr);
	glUniform1f(ul_Alpha,	1.0f - m_last_alpha);


// 	TString key;
// 	key.setTextHEX((int)this);
// 	CCShaderCache::sharedShaderCache()->addProgram(m_glProgram, key.getTextUTF8());
}


void GrpShader::Init()
{

}

void GrpShader::setShader( CCNode *pNode )
{
	pNode->setShaderProgram(m_glProgram);
}

void GrpShader::setColor( float r, float g, float b, float gr )
{
	if(m_last_r == r && m_last_g == g && m_last_b == b && m_last_gr == gr)
		return;

	m_glProgram->use();
	glUniform1f(ul_AddR,	r);//TODO CHK
	glUniform1f(ul_AddG,	g);
	glUniform1f(ul_AddB,	b);
	glUniform1f(ul_GrPer,	gr);

	m_last_r = r;
	m_last_g = g;
	m_last_b = b;
	m_last_gr = gr;
}

void GrpShader::setAlpha( float alpha )
{
	if(m_last_alpha == alpha)
		return;

	m_glProgram->use();
	glUniform1f(ul_Alpha,	1.0f - alpha);

	m_last_alpha = alpha;
}
