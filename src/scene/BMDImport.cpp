#include "BMDImport.h"
#include "GLNode.h"
#include "../core/Buffer.h"

using namespace std;

bool BMDImport::loadFromFile( GLNode *node, std::string fileName )
{
    //prepare file name
    string filePath = fileName;

    if( fileName.rfind( '/' ) != string::npos )
        filePath = filePath.substr( 0, fileName.rfind( '/' ) + 1 );

    //open file
    fstream file( fileName.c_str(), fstream::in | fstream::binary );

    //create buffers
    Buffer iBuffer( sizeof( int ), file );
    Buffer fBuffer( sizeof( float ), file );
    Buffer sBuffer( sizeof( char ) * 512, file );
    Buffer cBuffer( sizeof( char ), file );

    if( cBuffer.convertFromStream<char>() == 'b' &&
            cBuffer.convertFromStream<char>() == 'm' &&
            cBuffer.convertFromStream<char>() == 'd' )
        cBuffer.readFromStream();
    else
        return false;

    int faceCount = iBuffer.convertFromStream<int>();
    int textureCount = iBuffer.convertFromStream<int>();

    float *vertices = new float[faceCount * 3 * 3];
    float *normals = new float[faceCount * 3 * 3];
    float *uvs = new float[faceCount * 3 * 2 * textureCount];
    string *textureFileNames = new string[textureCount];

    for( int x = 0; x < textureCount; x++ )
    {
        textureFileNames[x] = string( sBuffer.getFromStream() );
        if( textureFileNames[x].substr( 0, 2 ) == "//" )
            textureFileNames[x] = textureFileNames[x].substr( 2 );
        textureFileNames[x] = filePath + textureFileNames[x];
    }

    for( int face = 0, x = 0; face < faceCount * 3 * 3;
         face += 3 * 3, x += 3 * 2 )
    {
        iBuffer.readFromStream();

        for( int vert = 0, uv = 0; vert < 9; vert += 3, uv += 2 )
        {
            vertices[face + vert] = fBuffer.convertFromStream<float>();
            vertices[face + vert + 1] = fBuffer.convertFromStream<float>();
            vertices[face + vert + 2] = fBuffer.convertFromStream<float>();

            normals[face + vert] = fBuffer.convertFromStream<float>();
            normals[face + vert + 1] = fBuffer.convertFromStream<float>();
            normals[face + vert + 2] = fBuffer.convertFromStream<float>();

            uvs[x + uv] = fBuffer.convertFromStream<float>();
            uvs[x + uv + 1] = fBuffer.convertFromStream<float>();
        }
    }

    node->setData( faceCount, textureCount, vertices, normals, uvs,
                   textureFileNames );

    file.close();

    return true;
}
