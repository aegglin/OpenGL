#pragma once
#include <GL/glew.h>


#define ASSERT(x) if (!(x)) __debugbreak() //__debugbreak is MSVC intrinsic (different for other compilers)

#define GLCall(x) GLClearError();x;ASSERT(GLLogCall(#x,__FILE__,__LINE__)) //file and line are not compiler intrinsic (should be supported by all compilers)

// Don't do the error checking in release mode
//#ifdef DEBUG
//	#define GLCall(x) GLClearError();x;ASSERT(GLLogCall())	
//#else
//	#define GLCall(x) x
//#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);