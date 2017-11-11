#pragma once

#pragma once

#define PY_SSIZE_T_CLEAN

// REF: http://bytes.com/topic/python/answers/101537-vc-linking-problem
#if defined(_DEBUG)
#define WITH_DEBUG    
#undef _DEBUG
#endif
#include <Python.h> 
#include <structmember.h>
#include <structseq.h>
#if defined(WITH_DEBUG)
#define _DEBUG
#endif
