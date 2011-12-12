#ifndef BMDIMPORT_H
#define BMDIMPORT_H

#include <string>

class GLNode;

class BMDImport
{
public:
    static bool loadFromFile( GLNode *node, std::string fileName );
};

#endif // BMDIMPORT_H
