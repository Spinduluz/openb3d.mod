#ifndef PROGRAMOBJECT_H
#define PROGRAMOBJECT_H

#include <string>
#include <list>
#include <map>
#include <memory>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

using namespace std;

class ShaderObject;

class ProgramObject : public ReferencedObject{
public:
#if defined(BLITZMAX_DEBUG)
	static int debug_count;
#endif
	static int default_program;
	static list<ProgramObject*> programobjects;

	int program;				// The ProgramObject

	string name;
	size_t hash;

	list<ShaderObject*> shaderobjects;

	/*list<ShaderObject*> vertexshaders;
	list<ShaderObject*> fragmentshaders;*/

	map<string,int> TypeMap;

	static ProgramObject* Create(string name="null");

	ProgramObject(){
	}
	~ProgramObject();

	void Activate();
	void DeActivate();
	void RefreshTypeMap();

	// Get the Uniform Variable Location from a ProgramObject
	int GetUniformLocation(const string& name);
	// Get the Attribute Variable Location from a ProgramObject
	int GetAttribLocation(const string& name);

	void SetParameter1S(const string& name,float v1);
	void SetParameter2S(const string& name,float v1,float v2);
	void SetParameter3S(const string& name,float v1,float v2,float v3);
	void SetParameter4S(const string& name,float v1,float v2,float v3,float v4);

	//------------------------------------------------------------
	// Int Parameter
	
	void SetParameter1I(const string& name,int v1);
	void SetParameter2I(const string& name,int v1,int v2);
	void SetParameter3I(const string& name,int v1,int v2,int v3);
	void SetParameter4I(const string& name,int v1,int v2,int v3,int v4);
	
	//----------------------------------------------------------------------------------
	// Int Vectors

	void SetVector1I(const string& name,int* v1);
	void SetVector2I(const string& name,int* v1);
	void SetVector3I(const string& name,int* v1);
	void SetVector4I(const string& name,int* v1);
				
	//-------------------------------------------------------------------------------------
	// Double Parameter ( automatically Attributes, because Uniform doubles does not exist)
	
	void SetParameter1D(const string& name,double v1);
	void SetParameter2D(const string& name,double v1,double v2); 
	void SetParameter3D(const string& name,double v1,double v2,double v3);
	void SetParameter4D(const string& name,double v1,double v2,double v3, double v4);

	//-------------------------------------------------------------------------------------
	//  Array Parameter
	void SetParameterArray(const string& name,Surface* surf,int vbo);
	void SetParameterArray(const string& name,vector<float>* vertices,int vbo);

	//-------------------------------------------------------------------------------------
	// Float Parameter

	void SetParameter1F(const string& name,float v);
	void SetParameter2F(const string& name,float v1,float v2);
	void SetParameter3F(const string& name,float v1,float v2,float v3);
	void SetParameter4F(const string& name,float v1,float v2,float v3,float v4);

	//---------------------------------------------------------------------------------------------------
	// Float Vectors


	void SetVector1F(const string& name,float* v1);
	void SetVector2F(const string& name,float* v1);
	void SetVector3F(const string& name,float* v1);
	void SetVector4F(const string& name,float* v1);

	//--------------------------------------------------------------------------------------------------
	// Matrices

	void SetMatrix2F(const string& name,float* m);
	void SetMatrix3F(const string& name,float* m);
	void SetMatrix4F(const string& name,float* m);

	bool AttachShader(ShaderObject *shader);
	void DetachShader(ShaderObject* shader);
};

#endif // PROGRAMOBJECT_H

