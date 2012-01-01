#ifndef GLNODE_H
#define GLNODE_H

#include "Node.h"
#include <string>

class QImage;

class GLNode : public Node
{
public:
    GLNode( Node* parent )
        : Node( parent ), mZPass( false )
    {}

    GLNode( Node* parent, const glm::vec3& position, const glm::quat& rotation )
        : Node( parent, position, rotation ), mZPass( false )
    {}

    virtual ~GLNode();

    virtual void init();

    virtual void update();

    void setData( unsigned int faceCount, unsigned int textureCount,
                  float *vertices, float *normals, float *uvs,
                  std::string *textureFileNames );

    void setAlwasDrawToFront( bool on )
    {
        mZPass = on;
    }

protected:
    unsigned int mFaceCount, mTextureCount, *mTextureHandles;
    float *mVertices;
    float *mNormals;
    float *mUVs;
    std::string *mTextureFileNames;
    QImage **mTextures;

    bool mZPass;
};

#endif // GLNODE_H
