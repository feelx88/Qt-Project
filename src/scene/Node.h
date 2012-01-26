#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>

#include "CollisionShape.h"

class Node
{
public:
    Node( Node* parent )
        : mPosition( glm::vec3() ), mScale( glm::vec3( 1.f ) ),
          mRotation( glm::quat() ), mParent( parent ), mCollisionShape( 0 ),
          mVisible( true ), mTag( 0 )
    {
        if( parent )
            parent->addChild( this );
    }

    Node( Node* parent, const glm::vec3& position, const glm::quat& rotation,
          const glm::vec3 scale = glm::vec3( 1.f ) )
        : mPosition( position ), mScale( scale ), mRotation( rotation ),
          mCollisionShape( 0 ), mVisible( true ), mTag( 0 )
    {
        if( parent )
            parent->addChild( this );
    }

    virtual ~Node()
    {
        if( mCollisionShape )
            delete mCollisionShape;
    }

    virtual void init(){}

    virtual void update()
    {
        for( std::vector<Node*>::iterator x = mChildren.begin();
             x != mChildren.end(); x++ )
            (*x)->update();
    }

    void setPosition( const glm::vec3& position )
    {
        mPosition = position;
    }

    void setScale( const glm::vec3& scale )
    {
        mScale = scale;
    }

    void setRotation( const glm::quat& rotation )
    {
        mRotation = rotation;
    }

    const glm::vec3& getPosition() const
    {
        return mPosition;
    }

    const glm::vec3& getScale() const
    {
        return mScale;
    }

    const glm::quat& getRotation() const
    {
        return mRotation;
    }

    void move( glm::vec3 delta )
    {
        delta = glm::gtx::quaternion::rotate( mRotation, delta );
        setPosition( getPosition() + delta );
    }

    void addChild( Node *child )
    {
        mChildren.push_back( child );
    }

    void hide()
    {
        mVisible = false;
    }

    void show()
    {
        mVisible = true;
    }

    void setVisible( bool visible )
    {
        mVisible = visible;
    }

    void setCollisionShape( CollisionShape *shape )
    {
        mCollisionShape = shape;
    }

    CollisionShape *getCollisionShape()
    {
        return mCollisionShape;
    }

    bool collidesWith( Node *other );

    void setTag( const unsigned int &tag )
    {
        mTag = tag;
    }

    const unsigned int &getTag()
    {
        return mTag;
    }

protected:
    glm::vec3 mPosition, mScale;
    glm::quat mRotation;

    Node *mParent;
    std::vector<Node*> mChildren;

    CollisionShape *mCollisionShape;

    bool mVisible;

    unsigned int mTag;
};

#endif // NODE_H
