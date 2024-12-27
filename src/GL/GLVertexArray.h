#ifndef __GLVERTEXARRAY_H__
#define __GLVERTEXARRAY_H__
#include "GLBuffers.h"

class GLVertexArray : public GLObject
{
public:
	GLVertexArray();
	void addVBO(GLuint index, GLVertexBuffer& vbo, GLint size, GLsizei stride, const void* pointer);
	void setEBO(GLElementBuffer& ebo);
	virtual void bind() const override;
	virtual void unbind() const override;
	virtual void create() override;
	virtual void deleteObj() override;
};
#endif