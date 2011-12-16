#include "Game.h"

#include "../ui/GLRenderer.h"

#include "../scene/Node.h"
#include "../scene/GLCameraNode.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

Game::Game()
{
}

GLNode *node;

void Game::init()
{
    new GLCameraNode( GLRenderer::getRootNode(), glm::vec3( 0, 10, 10 ),
                      glm::quat() );

    node = new GLNode( GLRenderer::getRootNode(), glm::vec3( 0, 0, 0 ),
                               glm::gtx::quaternion::angleAxis(
                                   45.f, 0.f, 1.f, 0.f ) );
    BMDImport::loadFromFile( node, "raw/ship.bmd" );
}

void Game::run()
{
    glm::quat rot = node->getRotation();
    float angle = glm::gtx::quaternion::angle( rot ) + 0.1f;
    angle = angle > 360.f ? 0.f : angle;
    rot = glm::gtx::quaternion::angleAxis( angle,
                                           glm::vec3( 0, 1, 0 ) );
    node->setRotation( rot );
}
