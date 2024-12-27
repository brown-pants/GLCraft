#include "GLShader.h"

GLShader::GLShader() : GLObject() 
{
    
}

void GLShader::compile(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource)
{
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void GLShader::setBool(const std::string& name, bool value) const
{
    bind();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    unbind();
}

void GLShader::setInt(const std::string& name, int value) const
{
    bind();
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    unbind();
}

void GLShader::setFloat(const std::string& name, float value) const
{
    bind();
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    unbind();
}

void GLShader::setVec2(const std::string& name, const glm::vec2& value) const
{
    bind();
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    unbind();
}

void GLShader::setVec2(const std::string& name, float x, float y) const
{
    bind();
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    unbind();
}

void GLShader::setVec3(const std::string& name, const glm::vec3& value) const
{
    bind();
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    unbind();
}

void GLShader::setVec3(const std::string& name, float x, float y, float z) const
{
    bind();
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    unbind();
}

void GLShader::setVec4(const std::string& name, const glm::vec4& value) const
{
    bind();
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    unbind();
}

void GLShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    bind();
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    unbind();
}

void GLShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    bind();
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    unbind();
}

void GLShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    bind();
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    unbind();
}

void GLShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    bind();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    unbind();
}

void GLShader::bind() const
{
    glUseProgram(ID);
}

void GLShader::unbind() const
{
    glUseProgram(0);
}

void GLShader::create()
{
    ID = glCreateProgram();
}

void GLShader::deleteObj()
{
    glDeleteProgram(ID);
    ID = 0;
}

void GLShader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}