#ifndef __MESH_H__
#define __MESH_H__
#include "../GL/GLVertexArray.h"
#include "../GL/GLBuffers.h"
#include <vector>

struct Mesh
{
    void clear()
    {
        vao.deleteObj();
        vbo.deleteObj();
        ebo.deleteObj();
        for(auto obj : instance_vbos)
        {
            obj.deleteObj();
        }
    }
    
    GLVertexArray vao;
    GLVertexBuffer vbo;
    GLElementBuffer ebo;
    //instance arrays
    std::vector<GLVertexBuffer> instance_vbos;
};

#endif