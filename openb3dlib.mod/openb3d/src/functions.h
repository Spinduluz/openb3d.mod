#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#if defined(_WIN32) && !defined(BUILD_STATIC)
#	ifdef BUILD_DLL
#		define DLL_EXPORT __declspec(dllexport)
#	else
#		define DLL_EXPORT __declspec(dllimport)
#	endif
#else
#	define DLL_EXPORT
#endif

typedef void bbTexture;
typedef void bbBrush;
typedef void bbEntity;
typedef void bbMesh;
typedef void bbSurface;
typedef void bbCamera;
typedef void bbTerrain;
typedef void bbLight;
typedef void bbPivot;
typedef void bbShadowObject;
typedef void bbSprite;
typedef void bbStencil;
typedef void bbShader;
typedef void bbVoxelSprite;
typedef void bbGeosphere;
typedef void bbMaterial;
typedef void bbOcTree;
typedef void bbShaderObject;

#ifdef __cplusplus
extern "C" {
#include "functions_cpp.h"
} // extern "C"
#else
#include "functions_c.h"
#endif

#endif // FUNCTIONS_H
