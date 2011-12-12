#ifndef BUFFER_H
#define BUFFER_H

#include <fstream>

class Buffer
{
public:
    template <class T>
    Buffer( T x )
    {
        mSize = sizeof( x );
        mBuffer = new char[mSize];
    }

    Buffer( const int &size )
        : mSize( size )
    {
        mBuffer = new char[mSize];
    }

    Buffer( const int &size, std::fstream &stream )
        : mSize( size ), mStream( &stream )
    {
        mBuffer = new char[mSize];
    }

    virtual ~Buffer()
    {
        mSize = 0;
        delete[] mBuffer;
    }

    void clear()
    {
        for( int x = 0; x < mSize; x++ )
            mBuffer[x] = 0;
    }

    void setStream( std::fstream& stream )
    {
        mStream = &stream;
    }

    template <class T>
    T convert()
    {
        //DEBUG
        //std::cout << *reinterpret_cast<T*>( mBuffer ) << std::endl;

        return *reinterpret_cast<T*>( mBuffer );
    }

    bool readFromStream( std::fstream &stream )
    {
        clear();
        stream.read( mBuffer, mSize );
        return true;
    }

    template <class T>
    T convertFromStream( std::fstream& stream )
    {
        readFromStream( stream );
        return convert<T>();
    }

    template <class T>
    T convertFromStream()
    {
        return convertFromStream<T>( *mStream );
    }

    const char* get()
    {
        return mBuffer;
    }

    const char* getFromStream( std::fstream& stream )
    {
        readFromStream( stream );
        return get();
    }

    const char* getFromStream()
    {
        return getFromStream( *mStream );
    }

protected:
    int mSize;
    char *mBuffer;
    std::fstream *mStream;
};

#endif // BUFFER_H
