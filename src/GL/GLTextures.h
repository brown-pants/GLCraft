#ifndef __GLTEXTURES_H__
#define __GLTEXTURES_H__
#include "GLObject.h"

class GLTexture : public GLObject
{
public:
	enum Type
	{
		Tex2d,
		TexCubMap
	};
	GLTexture(Type type);
	virtual void bind() const override;
	virtual void unbind() const override;
	virtual void create() override;
	virtual void deleteObj() override;

private:
	GLenum target;
};

class GLTexture2D : public GLTexture
{
public:
	GLTexture2D();
	void setData(GLubyte* data, GLsizei width, GLsizei height, GLboolean alpha);
};
#endif