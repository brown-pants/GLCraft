#ifndef __GLBUFFERS_H__
#define __GLBUFFERS_H__
#include "GLObject.h"

class GLBuffer : public GLObject
{
public:
	enum Type
	{
		VBO,
		EBO
	};
	GLBuffer(Type type);
	void setData(GLsizeiptr size, const void* data);
	virtual void bind() const override;
	virtual void unbind() const override;
	virtual void create() override;
	virtual void deleteObj() override;

private:
	GLenum target;
};

class GLVertexBuffer : public GLBuffer
{
public:
	GLVertexBuffer();
};

class GLElementBuffer : public GLBuffer
{
public:
	GLElementBuffer();
};
#endif