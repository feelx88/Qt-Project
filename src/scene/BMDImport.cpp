#include "BMDImport.h"
#include "GLNode.h"
#include "../core/Buffer.h"

using namespace std;

GLNode* BMDImport::loadFromFile(std::string fileName)
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
        return 0;

    GLNode *node = new GLNode();

    int vertexCount = iBuffer.convertFromStream<int>();
    int textureCount = iBuffer.convertFromStream<int>();

    float *vertices = new float[vertexCount];
    float *normals = new float[vertexCount];
    float *uvs = new float[vertexCount];
    string *textureFileNames = new string[textureCount];

    for( int x = 0; x < vertexCount; x += 3 )
    {
        vertices[x] = fBuffer.convertFromStream<float>();
        vertices[x + 1] = fBuffer.convertFromStream<float>();
        vertices[x + 2] = fBuffer.convertFromStream<float>();

        normals[x] = fBuffer.convertFromStream<float>();
        normals[x + 1] = fBuffer.convertFromStream<float>();
        normals[x + 2] = fBuffer.convertFromStream<float>();

        uvs[x] = fBuffer.convertFromStream<float>();
        uvs[x + 1] = fBuffer.convertFromStream<float>();
        uvs[x + 2] = fBuffer.convertFromStream<float>();
    }

    for( int x = 0; x < textureCount; x++ )
        textureFileNames[x] = sBuffer.convertFromStream<char*>();

    node->setData( vertexCount, textureCount, vertices, normals, uvs,
                   textureFileNames );

    return node;

}
