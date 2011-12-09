#ifndef GLNODE_H
#define GLNODE_H

#include "Node.h"
#include <string>

class GLNode : public Node
{
public:
    GLNode();
    GLNode( const glm::vec3& position, const glm::quat& rotation );

    virtual ~GLNode();

    virtual void update();

    void setData( int vertexCount, int textureCount, float *vertices,
                  float *normals, float *uvs, std::string *textureFileNames );

protected:
    int mVertexCount;
    float *mVertices;
    float *mNormals;
    float *mUVs;
    std::string *mTextureFileNames;
};

#endif // GLNODE_H
