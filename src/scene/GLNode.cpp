#include "GLNode.h"
#include <QtOpenGL>

#include "../ui/GLRenderer.h"

GLNode::GLNode()
    : Node()
{}

GLNode::GLNode( const glm::vec3& position, const glm::quat& rotation )
    : Node( position, rotation )
{}

GLNode::~GLNode()
{
    mFaceCount = 0;
    mTextureCount = 0;
    delete[] mVertices;
    delete[] mNormals;
    delete[] mUVs;
    delete[] mTextureFileNames;
    delete[] mTextures;
}

void GLNode::init()
{}

void GLNode::update()
{
    glPushMatrix();

    glTranslatef( mPosition.x, mPosition.y, mPosition.z );
    float angle = 0.f, x = 0.f, y = 0.f, z = 0.f;
    glm::gtx::quaternion::angleAxis( angle, x, y, z );
    glRotatef( angle, x, y, z );

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
                           mTextures[x]->height(), GL_RGBA, GL_UNSIGNED_BYTE,
                           mTextures[x]->bits() );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
    }
}
