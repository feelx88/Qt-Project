#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

class Node;

class CollisionShape
{
public:
    enum ShapeType
    {
        COLLISION_SPHERE = 0,
        COLLISION_BOX,
        COLLISION_MESH
    };

    CollisionShape( ShapeType shape, Node *node );

    virtual ~CollisionShape();

    void setSphereRadius( float radius )
    {
        mSphereRadius = radius;
    }

    bool testCollision( CollisionShape *other );

    static CollisionShape *newSpehereShape( Node *node, float radius );

protected:
    Node *mNode;

    ShapeType mShapeType;

    float mSphereRadius;
};

#endif // COLLISIONSHAPE_H
