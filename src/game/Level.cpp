#include "Level.h"

#include "Game.h"
#include "Enemy.h"
#include "Path.h"

#include "../ui/GLRenderer.h"
#include "../scene/GLNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include <QDomDocument>
#include <QFile>
#include <QDir>

Level::Level()
{
}

Level::~Level()
{
    for( std::vector<GLNode*>::iterator x = mLevelMeshes.begin();
         x != mLevelMeshes.end(); x++ )
        delete *x;
    for( std::vector<Enemy*>::iterator x = mEnemies.begin();
         x != mEnemies.end(); x++ )
        delete *x;
}

void Level::update()
{
    for( std::vector<Enemy*>::iterator x = mEnemies.begin();
         x != mEnemies.end(); x++ )
        (*x)->update();
}

void Level::loadLevel(std::string fileName)
{
    QDir dir( QString( fileName.c_str() ) );
    dir.cdUp();

    QDomDocument doc;
    doc.setContent( new QFile( fileName.c_str() ), false );

    QDomNodeList meshList = doc.elementsByTagName( "Mesh" );
    for( int x = 0; x < meshList.size(); x++ )
    {
        QDomNode meshNode = meshList.at( x );
        std::string file = meshNode.firstChildElement( "File" ).firstChild()
                .nodeValue().toStdString();

        GLNode *mesh = new GLNode( GLRenderer::getRootNode() );
        BMDImport::loadFromFile( mesh, dir.path().toStdString() + "/" + file );
        mLevelMeshes.push_back( mesh );

        glm::vec3 pos;

        pos.x = meshNode.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        pos.y = meshNode.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        pos.z = meshNode.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();

        mesh->setPosition( pos );

        CollisionShape *shape = CollisionShape::newMeshShape( mesh, mesh->getVertices() );
        mesh->setCollisionShape( shape );

        mesh->setTag( Game::NODE_LEVEL );
    }

    QDomNodeList playerPosition = doc.elementsByTagName( "Player" );
    if( !playerPosition.isEmpty() )
    {
        QDomNode pos = playerPosition.at( 0 );
        mPlayerStart.x = pos.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        mPlayerStart.y = pos.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        mPlayerStart.z = pos.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();
    }

    QDomNodeList enemies = doc.elementsByTagName( "Enemy" );
    for( int x = 0; x < enemies.size(); x++ )
    {
        QDomNode enemy = enemies.at( x );
        Path *path = new Path();

        QDomNodeList pathNodes = enemy.childNodes();
        for( int y = 0; y < pathNodes.size(); y++ )
        {
            QDomNode pathNode = pathNodes.at( y );
            if( pathNode.nodeName() != "PathNode" )
                continue;

            glm::vec3 pos;

            pos.x = pathNode.firstChildElement( "X" ).firstChild()
                    .nodeValue().toFloat();
            pos.y = pathNode.firstChildElement( "Y" ).firstChild()
                    .nodeValue().toFloat();
            pos.z = pathNode.firstChildElement( "Z" ).firstChild()
                    .nodeValue().toFloat();

            path->addNode( pos );
        }

        Enemy *e = new Enemy( enemy.firstChildElement( "Name" ).firstChild()
                    .nodeValue().toStdString() );
        e->setPath( path );

        mEnemies.push_back( e );
    }
}
