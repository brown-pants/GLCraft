#include "GLObject.h"

GLObject::GLObject() : ID(0) {}

GLObject::GLObject(GLObject&& obj) noexcept
{
	this->ID = obj.ID;
	obj.ID = 0;
}

GLObject::GLObject(const GLObject& obj) noexcept
{
	this->ID = obj.ID;
}

GLObject& GLObject::operator=(GLObject&& obj) noexcept
{
	this->ID = obj.ID;
	obj.ID = 0;
	return *this;
}

GLObject& GLObject::operator=(const GLObject& obj) noexcept
{
	this->ID = obj.ID;
	return *this;
}
