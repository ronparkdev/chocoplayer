#include "GrpShaderFont.h"

#include "../AppMulti.h"
#include "../COMMON/MultiOutter.h"

GrpShaderFont::GrpShaderFont(bool isExtra)
{
	this->isExtra = isExtra;
	glProg = NULL;
	pLastTexBg = NULL;
	reload();
}

GrpShaderFont::~GrpShaderFont()
{
	glProg->release();
}

void GrpShaderFont::reload()
{
	const char *filename = isExtra?"filter_font_extra.fsh":"filter_font.fsh";

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

	m_ul_bgx = glGetUniformLocation(glProg->getProgram(), "u_bg_x");
	m_ul_bgy = glGetUniformLocation(glProg->getProgram(), "u_bg_y");
	m_uMask  = glGetUniformLocation( glProg->getProgram(), "u_background");

	glProg->use();

	if (pLastTexBg != NULL) {
		glUniform1f(m_ul_bgx, lastBgX);
		glUniform1f(m_ul_bgy, lastBgY);
		glActiveTexture(GL_TEXTURE1);
		ccGLBindTexture2DN(1, pLastTexBg->getName());
		glUniform1i(m_uMask, 1);
	}
}

void GrpShaderFont::setShader( CCNode *pNode )
{
	pNode->setShaderProgram(glProg);
}

void GrpShaderFont::setPos( int id, CCTexture2D * texBG )
{
	lastBgX = (id % 10) * 16.0f / 160.0f;
	lastBgY = (id / 10 + 3) * 16.0f / 80.0f;
	pLastTexBg = texBG;

	if (pLastTexBg != NULL) {
		glUniform1f(m_ul_bgx, lastBgX);
		glUniform1f(m_ul_bgy, lastBgY);
		glActiveTexture(GL_TEXTURE1);
		ccGLBindTexture2DN(1, pLastTexBg->getName());
		glUniform1i(m_uMask, 1);
	}
}
