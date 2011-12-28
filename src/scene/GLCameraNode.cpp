#include "GLCameraNode.h"
#include <QtOpenGL>

#include <GL/glu.h>

GLCameraNode::GLCameraNode( Node *parent )
    : Node( parent ), mLookAt( glm::vec3( 0.f, 0.f, 0.f ) ),
      mUpVector( glm::vec3( 0.f, 0.f, 0.f ) )
{
}

GLCameraNode::GLCameraNode( Node *parent, const glm::core::type::vec3 &position,
                            const glm::gtc::quaternion::quat &rotation )
    : Node( parent, position, rotation ), mLookAt( glm::vec3( 0.f, 0.f, 0.f ) ),
      mUpVector( glm::vec3( 0.f, 1.f, 0.f ) )
{
}

GLCameraNode::~GLCameraNode()
{
}

void GLCameraNode::update()
{
    Node::update();

    gluLookAt( mPosition.x, mPosition.y, mPosition.z,
               mLookAt.x, mLookAt.y, mLookAt.z,
               mUpVector.x, mUpVector.y, mUpVector.z);
}
