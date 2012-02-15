#include "Level.h"

#include "Game.h"
#include "PlayerShip.h"
#include "Enemy.h"
#include "Path.h"

#include "../ui/GLRenderer.h"
#include "../ui/MainWindow.h"

#include "../scene/GLNode.h"
#include "../scene/GLCameraNode.h"
#include "../scene/BMDImport.h"
#include "../scene/CollisionShape.h"

#include <QDomDocument>
#include <QFile>
#include <QDir>

Level::Level( GLCameraNode *camera )
    : mPlayerShip( 0 ), mCamera( camera )
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

    delete mPlayerShip;
}

void Level::update()
{
    for( std::vector<Enemy*>::iterator x = mEnemies.begin();
         x != mEnemies.end(); x++ )
        (*x)->update();
    mPlayerShip->update();

    for( unsigned int x = 0; x < mDirectionChangers.size(); x++ )
    {
        DirectionChanger dC = mDirectionChangers.at( x );
        if( dC.enabled && CollisionShape::collisionOccured(
                    mPlayerShip->getNode()->getCollisionShape(),
                    dC.node->getCollisionShape() ) )
        {
            mPlayerShip->setDirection( dC.direction );

            dC.enabled = false;

            mPlayerShip->setFlightMode( dC.mode );
        }
    }

    std::vector<CollisionShape*> shapes = CollisionShape::shapesCollidingWith(
                mPlayerShip->getNode()->getCollisionShape() );
    for( unsigned int x = 0; x < shapes.size(); x++ )
    {
        Node *node = shapes.at( x )->getNode();
        if( node->getTag() == Game::NODE_ENEMY ||node->getTag() == Game::NODE_LEVEL  )
            GLRenderer::sMainWindow->lost();
    }

 }

void Level::loadLevel(std::string fileName)
{
    QDir dir( QString( fileName.c_str() ) );
    dir.cdUp();

    QDomDocument doc;
    QFile *levelFile = new QFile( fileName.c_str() );
    levelFile->open( QFile::ReadOnly );
    QString levelContent = levelFile->readAll();

    levelContent = levelContent.replace( "\n", "" );
    levelContent = levelContent.replace( "\t", "" );

    doc.setContent( levelContent );

    QDomNodeList meshList = doc.elementsByTagName( "Mesh" );
    for( int x = 0; x < meshList.size(); x++ )
    {
        QDomNode meshNode = meshList.at( x );
        std::string file = meshNode.firstChildElement( "File" ).firstChild()
                .nodeValue().toStdString();

        GLNode *mesh = new GLNode( GLRenderer::getRootNode() );
        BMDImport::loadFromFile( mesh, dir.path().toStdString() + "/" + file );
        mLevelMeshes.push_back( mesh );

        glm::vec3 p;
        QDomNode pos = meshNode.firstChildElement( "Position" );
        p.x = pos.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        p.y = pos.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        p.z = pos.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();

        glm::vec3 r;
        QDomNode rot = meshNode.firstChildElement( "Rotation" );
        r.x = rot.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        r.y = rot.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        r.z = rot.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();

        mesh->setPosition( p );
        glm::quat rotation;

        rotation = glm::rotate( rotation, r.x, glm::vec3( 1.f, 0.f, 0.f ) );
        rotation = glm::rotate( rotation, r.y, glm::vec3( 0.f, 1.f, 0.f ) );
        rotation = glm::rotate( rotation, r.z, glm::vec3( 0.f, 0.f, 1.f ) );

        mesh->setRotation( rotation );

        bool nC;
        QString noCollision = meshNode.firstChildElement( "NoCollision" ).firstChild()
                .nodeValue();
        nC = noCollision == "True" ? true : false;

        if( !nC )
        {
            CollisionShape *shape = CollisionShape::newMeshShape( mesh, mesh->getVertices() );
            mesh->setCollisionShape( shape );
        }

        mesh->setTag( Game::NODE_LEVEL );
    }

    QDomNodeList enemies = doc.elementsByTagName( "Enemy" );
    for( int x = 0; x < enemies.size(); x++ )
    {
        QDomNode enemy = enemies.at( x );
        Path *path = new Path();

        glm::vec3 startPos;

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

            if( y == 0 )
                startPos = pos;
        }

        int hp = 3;
        QDomNode hitpoints = enemy.firstChildElement( "Hitpoints" );
        if( !hitpoints.isNull() )
            hp = hitpoints.firstChild().nodeValue().toInt();

        bool looping = false;

        QDomNode loop = enemy.firstChildElement( "Looping" );
        if( !loop.isNull() )
        {
            if( loop.firstChild().nodeValue() == "True" )
                looping = true;
        }

        path->setLooping( looping );

        Enemy *e = new Enemy( enemy.firstChildElement( "Name" ).firstChild()
                    .nodeValue().toStdString() );
        e->setPath( path );
        e->setHitpoints( hp );
        e->setStartPosition( startPos );

        mEnemies.push_back( e );
    }

    QDomNodeList playerShip = doc.elementsByTagName( "Player" );
    if( !playerShip.isEmpty() )
    {
        QDomNode pos = playerShip.at( 0 ).firstChildElement( "StartPosition" );
        mPlayerStart.x = pos.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        mPlayerStart.y = pos.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        mPlayerStart.z = pos.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();
    }

    QDomNodeList dirChangers = doc.elementsByTagName( "DirectionChanger" );
    for( int x = 0; x < dirChangers.size(); x++ )
    {
        QDomNode dirChanger = dirChangers.at( x );

        DirectionChanger dC;
        dC.node = new Node( GLRenderer::getRootNode() );

        glm::vec3 p;

        QDomNode pos = dirChanger.firstChildElement( "Position" );
        p.x = pos.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        p.y = pos.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        p.z = pos.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();

        dC.node->setPosition( p );

        QDomNode dir = dirChanger.firstChildElement( "Direction" );
        dC.direction.x = dir.firstChildElement( "X" ).firstChild()
                .nodeValue().toFloat();
        dC.direction.y = dir.firstChildElement( "Y" ).firstChild()
                .nodeValue().toFloat();
        dC.direction.z = dir.firstChildElement( "Z" ).firstChild()
                .nodeValue().toFloat();

        float rad = dirChanger.firstChildElement( "Radius" ).firstChild()
                .nodeValue().toFloat();

        QString mode = dirChanger.firstChildElement( "Mode" ).firstChild().nodeValue();
        dC.mode = mode == "Free" ? PlayerShip::MODE_FREEFLIGHT :
                    PlayerShip::MODE_FIXED_DIRECTION;

        dC.node->setCollisionShape( CollisionShape::newSphereShape( dC.node, rad ) );
        dC.node->setTag( Game::NODE_PLAYERSHIP_DIRECTION_CHANGER );

        dC.enabled = true;

        mDirectionChangers.push_back( dC );
    }

    mPlayerShip = new PlayerShip( "data/Models/PlayerSpaceship/ship1.bmd", mCamera );
    mPlayerShip->setPosition( mPlayerStart );

    delete levelFile;
}

void Level::action( PlayerShip::SHIP_ACTIONS action )
{
    if( mPlayerShip )
        mPlayerShip->action( action );
}
