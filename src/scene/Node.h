#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <deque>

class Node
{
public:
    Node( Node* parent )
        : mPosition( glm::vec3() ), mRotation( glm::quat() ), mParent( parent ),
          mVisible( true )
    {
        if( parent )
            parent->addChild( this );
    }

    Node( Node* parent, const glm::vec3& position, const glm::quat& rotation )
        : mPosition( position ), mRotation( rotation ), mVisible( true )
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

protected:
    glm::vec3 mPosition;
    glm::quat mRotation;

    Node *mParent;
    std::deque<Node*> mChildren;

    bool mVisible;
};

#endif // NODE_H
