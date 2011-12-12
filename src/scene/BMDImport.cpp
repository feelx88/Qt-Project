#include "BMDImport.h"
#include "GLNode.h"
#include "../core/Buffer.h"

using namespace std;

bool BMDImport::loadFromFile( GLNode *node, std::string fileName )
{
    //open file
    fstream file( fileName.c_str(), fstream::in | fstream::binary );

    //create buffers
    Buffer iBuffer( sizeof( int ), file );
    Buffer fBuffer( sizeof( float ), file );
    Buffer sBuffer( 512, file );

    //test magic numbers
    int magicOne = iBuffer.convertFromStream<int>();
    int magicTwo = iBuffer.convertFromStream<int>();

    if( magicOne != 42 || magicTwo != 11 )
        return false;

    int vertexCount = iBuffer.convertFromStream<int>();
    int textureCount = iBuffer.convertFromStream<int>();

    float *vertices = new float[vertexCount * 3];
    float *normals = new float[vertexCount * 3];
    float *uvs = new float[vertexCount * 2 * textureCount];
    string *textureFileNames = new string[textureCount];

    for( int x = 0; x < textureCount; x++ )
    {
        textureFileNames[x] = string( sBuffer.getFromStream() );
    }

    for( int x = 0; x < vertexCount * 3; x += 3 )
    {
        vertices[x] = fBuffer.convertFromStream<float>();
        vertices[x + 1] = fBuffer.convertFromStream<float>();
        vertices[x + 2] = fBuffer.convertFromStream<float>();

        normals[x] = fBuffer.convertFromStream<float>();
        normals[x + 1] = fBuffer.convertFromStream<float>();
        normals[x + 2] = fBuffer.convertFromStream<float>();

        for( int y = 0; y < textureCount * 2; y += 2 )
        {
            uvs[x + y] = fBuffer.convertFromStream<float>();
            uvs[x + 1 + y] = fBuffer.convertFromStream<float>();
        }
    }

    node->setData( vertexCount, textureCount, vertices, normals, uvs,
                   textureFileNames );

    return true;
}
