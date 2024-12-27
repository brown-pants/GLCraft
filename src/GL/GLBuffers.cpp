#include "GLBuffers.h"

//GLBuffer
GLBuffer::GLBuffer(Type type) : GLObject()
{
	target = (type == VBO ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
}

void GLBuffer::setData(GLsizeiptr size, const void* data)
{
	this->bind();
	glBufferData(target, size, data, GL_STATIC_DRAW);
	this->unbind();
}

void GLBuffer::bind() const
{
	glBindBuffer(target, ID);
}

void GLBuffer::unbind() const
{
	glBindBuffer(target, ID);
}

void GLBuffer::create()
{
	glGenBuffers(1, &ID);
}

void GLBuffer::deleteObj()
{
	glDeleteBuffers(1, &ID);
	ID = 0;
}

//GLVertexBuffer
GLVertexBuffer::GLVertexBuffer() : GLBuffer(VBO) {}

//GLElementBuffer
GLElementBuffer::GLElementBuffer() : GLBuffer(EBO) {}