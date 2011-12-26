#ifndef GLCAMERANODE_H
#define GLCAMERANODE_H

#include "Node.h"

class GLCameraNode : public Node
{
public:
    GLCameraNode( Node* parent );

    GLCameraNode( Node* parent, const glm::vec3& position,
                  const glm::quat& rotation );

    virtual ~GLCameraNode();

    virtual void update( int deltaNSec );

    void setLookAt( const glm::vec3 &lookAt )
    {
        mLookAt = lookAt;
    }

    void setUpVector( const glm::vec3 &upVector )
    {
        mUpVector = upVector;
    }

    const glm::vec3& getLookAt()
    {
        return mLookAt;
    }

    const glm::vec3& getUpVector()
    {
        return mUpVector;
    }

protected:
    glm::vec3 mLookAt, mUpVector;
};

#endif // GLCAMERANODE_H
