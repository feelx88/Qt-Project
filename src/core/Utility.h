#ifndef UTILITY_H
#define UTILITY_H

#include <glm/glm.hpp>

struct Utility{
    static glm::quat directionToQuat( glm::vec3 direction )
    {
        direction *= -1;
        glm::vec3 left = glm::cross( direction, glm::vec3( 0.f, -1.f, 0.f ) );
        glm::vec3 up = glm::cross( direction, left );

        direction = glm::normalize( direction );
        up = glm::normalize( up );
        left = glm::normalize( left );

        glm::mat3x3 directionMatrix( left.x, up.x, direction.x,
                                     left.y, up.y, direction.y,
                                     left.z, up.z, direction.z );

        glm::quat q = glm::toQuat( directionMatrix );
        return glm::inverse( q );
    }
};

#endif // UTILITY_H
