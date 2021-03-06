#include "GLNode.h"
#include <QtOpenGL>
#include <GL/glext.h>
#include <GL/glu.h>

#include "../ui/GLRenderer.h"

std::map<std::string,std::pair<QImage*, unsigned int> > GLNode::sTexturePool;

GLNode::GLNode( Node *parent )
    : Node( parent ), mTextureCount( 0 ), mZPass( false ), mColor( 0 ),
      mTexEnvMode( GL_MODULATE )
{
    setColor( 1.f, 1.f, 1.f, 1.f );
}

GLNode::GLNode(Node *parent, const glm::core::type::vec3 &position, const glm::gtc::quaternion::quat &rotation)
    : Node( parent, position, rotation ), mZPass( false ), mColor( 0 ),
      mTexEnvMode( GL_MODULATE )
{
    setColor( 1.f, 1.f, 1.f, 1.f );
}

GLNode::~GLNode()
{
    delete[] mVertices;
    delete[] mNormals;
    delete[] mUVs;
    delete[] mTextureFileNames;
    delete[] mTextures;
    delete[] mTextureHandles;

    delete[] mColor;
}

void GLNode::init()
{}

void GLNode::update()
{
    Node::update();

    if( !mVisible )
        return;

    glPushMatrix();

    if( mZPass )
        glDepthFunc( GL_ALWAYS );

    glTranslatef( mPosition.x, mPosition.y, mPosition.z );
    float angle = glm::gtx::quaternion::angle( mRotation );
    glm::vec3 axis = glm::gtx::quaternion::axis( mRotation );
    glRotatef( angle, axis.x, axis.y, axis.z );

    glScalef( mScale.x, mScale.y, mScale.z );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glVertexPointer( 3, GL_FLOAT, 0, mVertices );
    glNormalPointer( GL_FLOAT, 0, mNormals );

    if( mTextureCount > 0 )
    {
        glEnable( GL_TEXTURE_2D );

        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer( 2, GL_FLOAT, 0, mUVs );

        glBindTexture( GL_TEXTURE_2D, mTextureHandles[0] );
    }

    glColor4fv( mColor );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mTexEnvMode );

    glDrawArrays( GL_TRIANGLES, 0, mFaceCount * 3 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );

    if( mTextureCount > 0 )
    {
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        glDisable( GL_TEXTURE_2D );
    }

    glDepthFunc( GL_LESS );

    glPopMatrix();
}

void GLNode::setData( unsigned int faceCount, unsigned int textureCount,
                      float *vertices, float *normals, float *uvs,
                      std::string *textureFileNames )
{
    mFaceCount = faceCount;
    mTextureCount = textureCount;
    mVertices = vertices;
    mNormals = normals;
    mUVs = uvs;
    mTextureFileNames = textureFileNames;

    mTextureHandles = new unsigned int[textureCount];
    mTextures = new QImage*[mTextureCount];

    std::map<std::string,std::pair<QImage*, unsigned int> >::iterator tex =
            sTexturePool.find( mTextureFileNames[0] );

    if( tex != sTexturePool.end() )
    {
        mTextures[0] = tex->second.first;
        mTextureHandles[0] = tex->second.second;
        return;
    }

    glGenTextures( textureCount, mTextureHandles );

    for( unsigned int x = 0; x < textureCount; x++ )
    {
        glBindTexture( GL_TEXTURE_2D, mTextureHandles[x] );
        mTextures[x] = new QImage( QString( mTextureFileNames[x].c_str() ) );

        QGLWidget::convertToGLFormat( *mTextures[x] );

        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, mTextures[x]->width(),
                           mTextures[x]->height(), GL_BGRA, GL_UNSIGNED_BYTE,
                           mTextures[x]->bits() );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
    }

    sTexturePool.insert( std::make_pair( mTextureFileNames[0], std::make_pair(
                                             mTextures[0], mTextureHandles[0] ) ) );
}

std::vector<glm::vec3> GLNode::getVertices()
{
    std::vector<glm::vec3> verts;

    for( unsigned int x = 0; x < mFaceCount * 3 * 3; x += 3 )
    {
        verts.push_back( glm::vec3( mVertices[x], mVertices[x + 1], mVertices[x + 2] ) );
    }
    return verts;
}

void GLNode::setColor(float r, float g, float b, float a)
{
    if( mColor )
        delete mColor;
    mColor = new float[4];
    mColor[0] = r;
    mColor[1] = g;
    mColor[2] = b;
    mColor[3] = a;
}

void GLNode::clearTextures()
{
    std::map<std::string,std::pair<QImage*, unsigned int> >::iterator tex;
    for( tex = sTexturePool.begin(); tex != sTexturePool.end(); tex++ )
    {
        delete tex->second.first;
    }
}
