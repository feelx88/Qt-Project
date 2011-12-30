#include "Level.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"

Level::Level( std::string fileName )
{
    mLevelModel = new GLNode( GLRenderer::getRootNode() );
    BMDImport::loadFromFile( mLevelModel, fileName );
}

Level::~Level()
{
    delete mLevelModel;
}

void Level::update()
{
}
