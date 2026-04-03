#pragma once

#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <windowsx.h>
#include <stdio.h>
#include <math.h>
#include "vmath.h"
#include <vector>
#include <string>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MESH_EXTENT_XSCALE 512 /*[-MESH_EXTENT_XSCALE, MESH_EXTENT_XSCALE]. Amount by which we scale x axis in either side of origin. Mesh is in XZ plane*/
#define MESH_EXTENT_ZSCALE 512 /*[-MESH_EXTENT_ZSCALE, MESH_EXTENT_ZSCALE]. Mesh is in XZ plane*/
#define MESH_EXTENT_YSCALE 100.0 /*[0, MESH_EXTENT_YSCALE]*/
#define MESH_EXTENT_YSHIFT 5.0 /*[0 + MESH_EXTENT_YSHIFT, MESH_EXTENT_YSCALE + MESH_EXTENT_YSHIFT]*/
#define GRASS_INSTANCE_COUNT 5'00'000
#define CONFIG_ALLOW_OUT_OF_FOCUS_RENDER true

#define FRUSTUM_NEAR 0.01f
#define FRUSTUM_FAR 10000.0f


//also chage CONFIG_MAX_BONES_PER_VERTEX in shaders when changing this
#define CONFIG_MAX_BONES_PER_VERTEX 4 //max number of bones which can influence a vertex

#define CODE_DEBUG 1

#if CODE_DEBUG
#define RAY_TRACE_WORLD_COORD 1
#endif


#ifndef NDEBUG //only in debug binary
#define UTIL_SET_BREAKPOINT \
BOOL bIsRemoteDebuggerPresent = FALSE; \
CheckRemoteDebuggerPresent(GetCurrentProcess(), &bIsRemoteDebuggerPresent); \
if (IsDebuggerPresent() || bIsRemoteDebuggerPresent) \
{ \
    DebugBreak(); \
}
#else
#define UTIL_SET_BREAKPOINT ;
#endif

#define W2STR(wstr) (std::string(wstr.begin(),wstr.end()))
#define S2WTR(wstr) (std::wstring(wstr.begin(),wstr.end()))

static void WriteToConsole(std::string s)
{
    OutputDebugStringA(s.c_str());
    OutputDebugStringA("\n");
}

static void WriteToConsole(long long i)
{
    std::string s = std::to_string(i);
    OutputDebugStringA(s.c_str());
    OutputDebugStringA("\n");
}


static void WriteToConsole(long double d)
{
    std::string s = std::to_string(d);
    OutputDebugStringA(s.c_str());
    OutputDebugStringA("\n");
}

#define STR(x) (std::to_string(x))

#ifndef NDEBUG //only in debug binary
#define THROW_DEBUG_EXCEPTION(msg) {MessageBoxA(NULL, msg, "error", MB_OK); throw std::exception(msg);}
#define DEBUG_ASSERT(condition,msg) {if(!(condition)){THROW_DEBUG_EXCEPTION(msg);}}
#define DEBUG_ASSERT_NOTHROW(condition,msg) {if(!(condition)){MessageBoxA(NULL, msg, "error", MB_OK);}}//for recoverable errors
//sends message to OpenGL api so that we can see the strings printed in logs/trace obtained by tools like apiTrace, etc.
#define DEBUG_OGL_INFO(msg) {glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_LOW, (GLsizei)strlen(msg), msg);};
//to print to visual studio console
#define DEBUG_VSCONSOLE(msg) {WriteToConsole(msg);};
#else
#define THROW_DEBUG_EXCEPTION(msg) ;
#define DEBUG_ASSERT(condition,msg) ;
#define DEBUG_ASSERT_NOTHROW(condition,msg) ;
#define DEBUG_OGL_INFO(msg) ;
#define DEBUG_VSCONSOLE(msg) ;
#endif


enum {
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXCOORD0,
    AMC_ATTRIBUTE_OFFSET1,
    AMC_ATTRIBUTE_OFFSET2,
    AMC_ATTRIBUTE_OFFSET3,
    AMC_ATTRIBUTE_OFFSET4,

    //assimp
    AMC_ATTRIBUTE_JOINT_INDICES,
    AMC_ATTRIBUTE_JOINT_WEIGHTS,
};

using namespace vmath;

struct Vertex {
    float x;
    float y;
    float z;
    Vertex Normalize()
    {
        auto mag = sqrtf(x * x + y * y + z * z);
        Vertex v = *this;

        if (mag != 0.0)
        {
            v.x /= mag;
            v.y /= mag;
            v.z /= mag;
        }

        return v;
    }
};

class BITMAP_INFO {
    std::vector<unsigned char> m_bitmapBits;
    int m_numColorComponents;
    LONG m_width, m_height;
public:
    /*
    * number of color components per pixel - 3 for rgb, bgr, etc.
    */
    BITMAP_INFO(BITMAP& bmp, int colorComponents);

    BITMAP_INFO();

    unsigned char operator[](int index)
    {
        return m_bitmapBits[index];
    }

    LONG GetWidth();

    LONG GetHeight();

    int GetNumColorComponents();
};

typedef long double vecType;


//template for N dimentional vector
template<typename T, const unsigned int len>
class VNT
{
public:
    T vectorComponents[len];
    VNT()
    {
        for (unsigned int i = 0; i < len; i++)
        {
            vectorComponents[i] = 0;
        }
    }

    T& i()//helper method to support V3.i, V3.j, V3.k idom for 3 dimentional vectors
    {
        DEBUG_ASSERT(len >= 1, "can only use in V1 and above");
        return vectorComponents[0];
    }

    T& j()//helper method to support V3.i, V3.j, V3.k idom for 3 dimentional vectors
    {
        DEBUG_ASSERT(len >= 2, "can only use in V2 and above");
        return vectorComponents[1];
    }

    T& k()//helper method to support V3.i, V3.j, V3.k idom for 3 dimentional vectors
    {
        DEBUG_ASSERT(len >= 3, "can only use in V3 and above");
        return vectorComponents[2];
    }

    T& w()//helper method to support V3.i, V3.j, V3.k idom for 3 dimentional vectors
    {
        DEBUG_ASSERT(len >= 4, "can only use in V4 and above");
        return vectorComponents[3];
    }

    VNT(const std::initializer_list<T>& args) //initializer_list
    {
        DEBUG_ASSERT(len == args.size(), "initializer_list<> size mismatch");
        for (int i = 0; i < len; i++)
        {
            vectorComponents[i] = *(args.begin() + i);
        }
    }

    VNT(const VNT& that) //copy contructor
    {
        for (unsigned int i = 0; i < len; i++)
        {
            vectorComponents[i] = that.vectorComponents[i];
        }
    }

    VNT(const T scalar) //contructor from scalar like V4(1.0) n glsl
    {
        for (unsigned int i = 0; i < len; i++)
        {
            vectorComponents[i] = scalar;
        }
    }

    T Magnitude() const
    {
        T sum = 0;
        for (unsigned int i = 0; i < len; i++)
        {
            sum += vectorComponents[i] * vectorComponents[i];
        }

        sum = powl(sum, 0.5);
        return sum;
    }

    // operator< -> not working correctly for std::map() several different V3T were being considered as the same key, hence commenting out the faulty implementation
    //bool operator< (const V3T& that) const
    //{
    //    return (this->Magnitude() < that.Magnitude());
    //}

    T& operator[](const int index)//sahils-test check
    {
        DEBUG_ASSERT(index <= len, "out of index access");
        return vectorComponents[index];
    }

    bool operator== (const VNT& that) const
    {
        bool retVal = true;
        for (unsigned int i = 0; i < len; i++)
        {
            if (vectorComponents[i] != that.vectorComponents[i])
            {
                retVal = false;
                break;
            }
        }

        return retVal;
    }

    bool operator!= (const VNT& that) const
    {
        return !(*this == that);
    }

    VNT Normalize()
    {
        VNT No = *this;
        auto mag = Magnitude();
        if (mag == 0)
        {
            return VNT(0);
        }

        for (unsigned int i = 0; i < len; i++)
        {
            No.vectorComponents[i] /= mag;
        }

        return No;
    }

    VNT operator+(VNT that)
    {
        VNT v = *this;

        for (unsigned int i = 0; i < len; i++)
        {
            v.vectorComponents[i] += that.vectorComponents[i];
        }
        return v;
    }

    T operator*(VNT that)//Dot product
    {
        VNT v = *this;
        T sum = 0;
        for (unsigned int i = 0; i < len; i++)
        {
            sum += vectorComponents[i] * that.vectorComponents[i];
        }
        return sum;
    }

    VNT operator/(T scalar)//scalar divide
    {
        VNT v = *this;
        for (unsigned int i = 0; i < len; i++)
        {
            v[i] /= scalar;
        }
        return v;
    }

    VNT operator-()
    {
        VNT v = *this;

        for (unsigned int i = 0; i < len; i++)
        {
            v.vectorComponents[i] += -1;
        }

        return v;
    }
};


typedef VNT<vecType, 3> V3;
typedef VNT<float, 3> V3f;
typedef VNT<GLfloat, 3> V3GLf;
typedef VNT<GLfloat, 2> V2GLf;
typedef VNT<GLfloat, 4> V4GLf;
typedef VNT<GLint, 4> V4GLi;
typedef VNT<GLuint, 4> V4GLui;
