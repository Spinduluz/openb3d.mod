#ifndef SHADER_MAT_H
#define SHADER_MAT_H

//
// There be monsters here, aka memory leaks
//
// This will probably have to be rewritten. I'm not sure it can be salvaged.
// 2016-01-12 It is salvaged...


//#include "global.h"
#include "matrix.h"
//#include "surface.h"
#include "texture.h"
#include "shaderobject.h"
#include "programobject.h"

#include "refobject.h"
#include "objectalloc.h"

#include <string>
#include <sstream>
#include <array>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

class Surface;
class Entity;

// ==========================================================================================================

struct ShaderData{
	string name;
	int type;

	union{
		//float valuef[3];
		//int valuei[3];
		//double valued[3];
		float* fp[4];
		int* ip[4];
		Entity* ent;
		struct {
			Surface* surf;
			int vbo;
		};
	};
};

// ==========================================================================================================

class Material : public Texture{
public:
	static Material* LoadMaterial(string filename,int flags=0, int frame_width=0,int frame_height=0,int first_frame=0,int frame_count=1);
	void BufferToMaterial(unsigned char* buffer);
	void MaterialToBuffer(unsigned char* buffer);
};

// ==========================================================================================================

struct Sampler{
#if defined(BLITZMAX_DEBUG)
	static int debug_count;
#endif
	bool is_used; // FIXME:

	string name;
	int slot;
	Texture* texture;
	bool is_texture3d; // -> BOOL=false/true <-

	Sampler(string name="",int slot=0,Texture *tex=NULL):is_used(false),name(name),slot(slot),texture(tex),is_texture3d(false) {
		texture->AddRef();
	}

	~Sampler(){
		texture->DestroyRef();
	}
	
	static Sampler* Create(string name, int slot, Texture* tex);
};

// ==========================================================================================================
struct ShaderTextureArray2{
	int count;
	Sampler items[255];

	ShaderTextureArray2():count(0),items(){
	}

	Sampler& operator[](int i){
		return items[i];
	}
};

class ShaderTextureArray : public array<Sampler*,255>{
public:
	int count;

	ShaderTextureArray():array<Sampler*,255>(),count(0){
	}
};

// ==========================================================================================================

#if defined(_MSC_VER)
#	define MAX_SHADER_COUNT 256
#endif

// New implementation of custom allocators (not required)
//	This is implemetation is twofold.
//	One: 
//	All objects are kept on the stack. 
//	This limits the number of ShaderObjects that can be created but removes the need for allocations/deallocations
//  Two:
//	Debugging. Using this I can see how many objects that are still allocated on application exit
//	Set ModuleInfo "CCFLAGS: -DMAX_SHADER_COUNT=[value] in openb3dlib/openb3dlib.bmx to enable this

// FIXME: Stackbased allocation unstable for Shader class for some reason

class Shader : public ReferencedObject {
public: 
	CLASS_ALLOCTOR_DECL(Shader)
// Move this to make all members public.
// Does not matter much since all other classes have their members public
	static int ShaderIDCount;

	ShaderTextureArray shader_texture;
	ProgramObject* arb_program; // Why a pointer?

	int id;

	string name;
	size_t hash;

	bool updatesampler; // Change from char to bool and lowercased the string

	vector<ShaderData> parameters;

	//-------------------------------------------------------------------------------	
	
	//void UpdateData(Surface* surf);
	
	// internal 
#if defined(BLITZMAX_DEBUG)
	static int debug_count;
#endif

	static Shader* CreateShaderMaterial(const string& name="");

	Shader();
	~Shader();

	// FIXME: Bind and Unbind instead
	void TurnOn(Matrix& mat, Surface* surf, vector<float>* vertices=0);
	void TurnOff();

	void AddShader(string vsfilename, string fsfilename);
	void AddShaderFromString(string vssrc, string fssrc,const string& name);

	void AddSampler2D(string name, int slot, Texture* tex);
	void AddSampler3D(string name, int slot, Texture* tex);

	void ProgramAttriBegin();
	void ProgramAttriEnd();

	void SetFloat(string name, float v1);
	void SetFloat2(string name, float v1, float v2);
	void SetFloat3(string name, float v1, float v2, float v3);
	void SetFloat4(string name, float v1, float v2, float v3, float v4);
	void UseFloat(string name, float* v1);
	void UseFloat2(string name, float* v1, float* v2);
	void UseFloat3(string name, float* v1, float* v2, float* v3);
	void UseFloat4(string name, float* v1, float* v2, float* v3, float* v4);
	void SetInteger(string name, int v1);
	void SetInteger2(string name, int v1, int v2);
	void SetInteger3(string name, int v1, int v2, int v3);
	void SetInteger4(string name, int v1, int v2, int v3, int v4);
	void UseInteger(string name, int* v1);
	void UseInteger2(string name, int* v1, int* v2);
	void UseInteger3(string name, int* v1, int* v2, int* v3);
	void UseInteger4(string name, int* v1, int* v2, int* v3, int* v4);

	void UseSurface(string name, Surface* surf, int vbo);
	void UseMatrix(string name, int mode);

	/*void SetParameter1S(string name, float v1);
	void SetParameter2S(string name, float v1, float v2);
	void SetParameter3S(string name, float v1, float v2, float v3);
	void SetParameter4S(string name, float v1, float v2, float v3, float v4);
	void SetParameter1I(string name, int v1);
	void SetParameter2I(string name, int v1, int v2);
	void SetParameter3I(string name, int v1, int v2, int v3);
	void SetParameter4I(string name, int v1, int v2, int v3, int v4);
	void SetVector1I(string name, int* v1);
	void SetVector2I(string name, int* v1);
	void SetVector3I(string name, int* v1);
	void SetVector4I(string name, int* v1);
	void SetParameter1F(string name, float v1);
	void SetParameter2F(string name, float v1, float v2);
	void SetParameter3F(string name, float v1, float v2, float v3);
	void SetParameter4F(string name, float v1, float v2, float v3, float v4);
	void SetVector1F(string name, float* v1);
	void SetVector2F(string name, float* v1);
	void SetVector3F(string name, float* v1);
	void SetVector4F(string name, float* v1);
	void SetMatrix2F(string name, float* m);
	void SetMatrix3F(string name, float* m);
	void SetMatrix4F(string name, float* m);
	void SetParameter1D(string name, double v1);
	void SetParameter2D(string name, double v1, double v2);
	void SetParameter3D(string name, double v1, double v2, double v3);
	void SetParameter4D(string name, double v1, double v2, double v3, double v4);*/

};

#if defined(BLITZMAX_DEBUG)
class _AllocatedStuff{
public:
	_AllocatedStuff(){
	}
	~_AllocatedStuff(){
		DebugLog("Shaders        [%i]",Shader::debug_count);
		DebugLog("Samplers       [%i]",Sampler::debug_count);
		DebugLog("ProgramObjects [%i]",ProgramObject::debug_count);
		DebugLog("ShaderObjects  [%i]",ShaderObject::debug_count);
	}
};
#endif

#endif
