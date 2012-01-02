#include "Node.h"
#include "CollisionShape.h"

bool Node::collidesWith( Node *other )
{
    if( !mCollisionShape )
        return false;
    return mCollisionShape->testCollision( other->mCollisionShape );
}
