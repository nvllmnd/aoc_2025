//! Just a simple header that wraps unix's unistd.h and win32 io.h
#pragma once

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif
