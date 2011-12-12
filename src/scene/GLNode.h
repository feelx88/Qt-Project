#ifndef GLNODE_H
#define GLNODE_H

#include "Node.h"
#include <string>

class QImage;

class GLNode : public Node
{
public:
    GLNode();
    GLNode( const glm::vec3& position, const glm::quat& rotation );

    virtual ~GLNode();

    virtual void init();

    virtual void update();

    void setData( unsigned int vertexCount, unsigned int textureCount,
                  float *vertices, float *normals, float *uvs,
                  std::string *textureFileNames );

protected:
    unsigned int mVertexCount, mTextureCount, *mTextureHandles;
    float *mVertices;
    float *mNormals;
    float *mUVs;
    std::string *mTextureFileNames;
    QImage **mTextures;
};

#endif // GLNODE_H
