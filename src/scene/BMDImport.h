#ifndef BMDIMPORT_H
#define BMDIMPORT_H

#include <iostream>
#include <string>
#include <exception>

class GLNode;

class BMDImport
{
public:
    struct BMDException : public std::exception
    {
        BMDException( std::string file )
        {
            std::cerr << "Error while importing file: " << file << std::endl;
        }

        const char* what() const throw()
        {
            return "Error!";
        }
    };

    static bool loadFromFile( GLNode *node, std::string fileName );
};

#endif // BMDIMPORT_H
