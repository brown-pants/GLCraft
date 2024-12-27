#ifndef __GLOBJECT_H__
#define __GLOBJECT_H__
#include <glad/glad.h>

class GLObject
{
public:
	GLObject();

	GLObject(const GLObject& obj) noexcept;
	GLObject(GLObject&& obj) noexcept;

	GLObject& operator= (const GLObject& obj) noexcept;
	GLObject& operator= (GLObject&& obj) noexcept;

	inline GLuint id() const { return ID; }

	virtual void bind() const = 0;
	virtual void unbind() const = 0;
	virtual void create() = 0;
	virtual void deleteObj() = 0;

protected:
	GLuint ID;
};
#endif