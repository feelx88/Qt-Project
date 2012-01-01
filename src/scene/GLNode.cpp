#include "GLNode.h"
#include <QtOpenGL>
#include <GL/glext.h>
#include <GL/glu.h>

#include "../ui/GLRenderer.h"

GLNode::~GLNode()
{
    delete[] mVertices;
    delete[] mNormals;
    delete[] mUVs;
    delete[] mTextureFileNames;
    for( int x = 0; x < mTextureCount; x++ )
        delete mTextures[x];
    delete[] mTextures;
    delete[] mTextureHandles;
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
}
