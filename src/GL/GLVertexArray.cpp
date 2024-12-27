#include "GLVertexArray.h"

GLVertexArray::GLVertexArray() : GLObject()
{
}

void GLVertexArray::addVBO(GLuint index, GLVertexBuffer& vbo, GLint size, GLsizei stride, const void* pointer)
{
	this->bind();
	vbo.bind();
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
	glEnableVertexAttribArray(index);
	this->unbind();
	vbo.unbind();
}

void GLVertexArray::setEBO(GLElementBuffer& ebo)
{
	this->bind();
	ebo.bind();
	this->unbind();
	ebo.unbind();
}

void GLVertexArray::bind() const
{
	glBindVertexArray(ID);
}

void GLVertexArray::unbind() const
{
	glBindVertexArray(0);
}

void GLVertexArray::create()
{
	glGenVertexArrays(1, &ID);
}

void GLVertexArray::deleteObj()
{
	glDeleteVertexArrays(1, &ID);
	ID = 0;
}