#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <deque>

class Node
{
public:
    enum CollisionShape
    {
        COLLISION_DISABLED = 0,
        COLLSISION_SPHERE,
        COLLISION_BOX
    };

    Node( Node* parent )
        : mPosition( glm::vec3() ), mScale( glm::vec3( 1.f ) ),
          mRotation( glm::quat() ), mParent( parent ), mVisible( true ),
          mCollisionShape( COLLISION_DISABLED )
    {
        if( parent )
            parent->addChild( this );
    }

    Node( Node* parent, const glm::vec3& position, const glm::quat& rotation,
          const glm::vec3 scale = glm::vec3( 1.f ) )
        : mPosition( position ), mScale( scale ), mRotation( rotation ),
          mVisible( true )
    {
        if( parent )
            parent->addChild( this );
    }

    virtual ~Node(){}

    virtual void init(){}

    virtual void update()
    {
        for( std::deque<Node*>::iterator x = mChildren.begin();
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
        delta = glm::gtx::vector_angle::rotate( mRotation, delta );
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

    void setCollisionShape( const CollisionShape &shape )
    {
        mCollisionShape = shape;
    }

    const CollisionShape& getCollisionShape()
    {
        return mCollisionShape;
    }

    bool collidesWith( Node *other );

protected:
    glm::vec3 mPosition, mScale;
    glm::quat mRotation;

    Node *mParent;
    std::deque<Node*> mChildren;

    CollisionShape mCollisionShape;

    bool mVisible;
};

#endif // NODE_H
