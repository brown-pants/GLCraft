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

//GLFrameBuffer
GLFrameBuffer::GLFrameBuffer() : GLObject() {}

void GLFrameBuffer::setDepthMap(const GLDepthMap &depthMap)
{
	this->bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.id(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	this->unbind();
}

void GLFrameBuffer::GLFrameBuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void GLFrameBuffer::GLFrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::create()
{
	glGenFramebuffers(1, &ID);
}

void GLFrameBuffer::deleteObj()
{
	glDeleteFramebuffers(1, &ID);
}