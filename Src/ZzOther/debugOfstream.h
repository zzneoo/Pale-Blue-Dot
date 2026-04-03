#pragma once
#include <fstream>
#include <string>

#define CONFIG_DEBUG_ALLOW_FILE_IO 1

class debugOfstream :
#if CONFIG_DEBUG_ALLOW_FILE_IO
    public std::ofstream
#else
    private std::ofstream
#endif
{
public:
    explicit debugOfstream(const char* filename, ios_base::openmode mode = ios_base::out) 
#if CONFIG_DEBUG_ALLOW_FILE_IO
        : std::ofstream(filename, mode)
#endif
    {

    }

    explicit debugOfstream(const std::string& filename, ios_base::openmode mode = ios_base::out)
#if CONFIG_DEBUG_ALLOW_FILE_IO
        : std::ofstream(filename, mode)
#endif
    {

    }

#if !CONFIG_DEBUG_ALLOW_FILE_IO
    debugOfstream& operator<< (bool val){return *this;}
    debugOfstream& operator<< (short val){return *this;}
    debugOfstream& operator<< (unsigned short val){return *this;}
    debugOfstream& operator<< (int val){return *this;}
    debugOfstream& operator<< (unsigned int val){return *this;}
    debugOfstream& operator<< (long val){return *this;}
    debugOfstream& operator<< (unsigned long val){return *this;}
    debugOfstream& operator<< (long long val){return *this;}
    debugOfstream& operator<< (unsigned long long val){return *this;}
    debugOfstream& operator<< (float val){return *this;}
    debugOfstream& operator<< (double val){return *this;}
    debugOfstream& operator<< (long double val){return *this;}
    debugOfstream& operator<< (void* val){return *this;}
    debugOfstream& operator<< (const char* val){return *this;}
    debugOfstream& operator<< (std::string val){return *this;}
    debugOfstream& operator<< (std::streambuf* sb){return *this;}
    debugOfstream& operator<< (std::ostream& (*pf)(std::ostream&)){return *this;}
    debugOfstream& operator<< (std::ios& (*pf)(std::ios&)){return *this;}
    debugOfstream& operator<< (ios_base& (*pf)(ios_base&)){return *this;}

    void close()
    {

    }
#endif
};

