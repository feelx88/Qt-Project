#ifndef BMDIMPORT_H
#define BMDIMPORT_H

#include <string>

class GLNode;

class BMDImport
{
public:
    static GLNode* loadFromFile( std::string fileName );
};

#endif // BMDIMPORT_H
