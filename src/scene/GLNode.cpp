#include "GLNode.h"

GLNode::GLNode()
    : Node()
{
}

GLNode::GLNode( const glm::vec3& position, const glm::quat& rotation )
    : Node( position, rotation )
{
}

GLNode::~GLNode()
{
}

void GLNode::update()
{
}

void GLNode::setData( int vertexCount, int textureCount, float *vertices,
                      float *normals, float *uvs, std::string *textureFileNames )
{
    mVertexCount = vertexCount;
    mVertices = vertices;
    mNormals = normals;
    mUVs = uvs;
    mTextureFileNames = textureFileNames;
}
