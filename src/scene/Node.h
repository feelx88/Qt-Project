#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Node
{
public:
    Node()
        : mPosition( glm::vec3() ), mRotation( glm::quat() )
    {}

    Node( const glm::vec3& position, const glm::quat& rotation )
        : mPosition( position ), mRotation( rotation )
    {}

    virtual ~Node(){}

    virtual void update(){}

    void setPosition( const glm::vec3& position )
    {
        mPosition = position;
    }

    void setRotation( const glm::quat& rotation )
    {
        mRotation = rotation;
    }

    const glm::vec3& getPosition() const
    {
        return mPosition;
    }

    const glm::quat& getRotation() const
    {
        return mRotation;
    }

protected:
    glm::vec3 mPosition;
    glm::quat mRotation;
};

#endif // NODE_H
