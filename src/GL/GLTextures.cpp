#include "GLTextures.h"

//GLTexture
GLTexture::GLTexture(Type type) : GLObject()
{
	target = (type == Tex2d ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP);
}

void GLTexture::bind() const
{
	glBindTexture(target, ID);
}

void GLTexture::unbind() const
{
	glBindTexture(target, 0);
}

void GLTexture::create()
{
	glGenTextures(1, &ID);
}

void GLTexture::deleteObj()
{
	glDeleteTextures(1, &ID);
	ID = 0;
}

//GLTexture2D
GLTexture2D::GLTexture2D() : GLTexture(Tex2d)
{
	
}

void GLTexture2D::setData(GLubyte* data, GLsizei width, GLsizei height, GLboolean alpha)
{
	this->bind();
	GLenum format = (alpha == GL_FALSE ? GL_RGB : GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	this->unbind();
}

//GLDepthMap
GLDepthMap::GLDepthMap() : GLTexture(Tex2d)
{

}

void GLDepthMap::setData(GLsizei width, GLsizei height)
{
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	this->unbind();
}