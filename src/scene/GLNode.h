#ifndef GLNODE_H
#define GLNODE_H

#include "Node.h"
#include <string>

class QImage;

class GLNode : public Node
{
public:
    GLNode( Node* parent )
        : Node( parent )
    {}

    GLNode( Node* parent, const glm::vec3& position, const glm::quat& rotation )
        : Node( parent, position, rotation )
    {}

    virtual ~GLNode();

    virtual void init();

    virtual void update( int deltaNSec );

    void setData( unsigned int faceCount, unsigned int textureCount,
                  float *vertices, float *normals, float *uvs,
                  std::string *textureFileNames );

protected:
    unsigned int mFaceCount, mTextureCount, *mTextureHandles;
    float *mVertices;
    float *mNormals;
    float *mUVs;
    std::string *mTextureFileNames;
    QImage **mTextures;
};

#endif // GLNODE_H
