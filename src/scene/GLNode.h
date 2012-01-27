#ifndef GLNODE_H
#define GLNODE_H

#include "Node.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <utility>

class QImage;

class GLNode : public Node
{
public:
    GLNode( Node* parent );

    GLNode( Node* parent, const glm::vec3& position, const glm::quat& rotation );

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

    std::vector<glm::vec3> getVertices();

    void setColor( float r, float g, float b, float a );

    void setTexEnvMode( unsigned int mode )
    {
        mTexEnvMode = mode;
    }

    static void clearTextures();

protected:
    unsigned int mFaceCount, mTextureCount, *mTextureHandles;
    float *mVertices;
    float *mNormals;
    float *mUVs;
    std::string *mTextureFileNames;
    QImage **mTextures;

    bool mZPass;

    float *mColor;
    unsigned int mTexEnvMode;

    static std::map<std::string,std::pair<QImage*, unsigned int> > sTexturePool;
};

#endif // GLNODE_H
