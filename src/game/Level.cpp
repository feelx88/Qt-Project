#include "Level.h"

#include "Game.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

Level::Level( std::string fileName )
{
    mLevelModel = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mLevelModel, fileName );

    mLevelModel->setTag( Game::NODE_LEVEL );
}

Level::~Level()
{
    delete mLevelModel;
}

void Level::update()
{
}
