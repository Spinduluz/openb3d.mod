#ifndef SHADEROBJECT_H
#define SHADEROBJECT_H

#include <map>
#include <vector>

#include "refobject.h"

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

class ProgramObject;
class Surface;

class ShaderObject : public ReferencedObject {
public:
#if defined(BLITZMAX_DEBUG)
	static int debug_count;
#endif
	static list<ShaderObject*> shaderobjects;

	int object;
	int type;

	string name;
	size_t hash;

	list<ProgramObject*> attached; // Shaders can be attached, or 'referenced by', more than 1 ProgramObject

	static ShaderObject* Create(unsigned int type,const string& src,const string& name);
	static ShaderObject* CreateFromFile(unsigned int type,const string& filename);
	// FIXME: Remove. This function has now real point now
	static void Delete(ShaderObject* shader);

	// These methods increases the reference count.
	static ShaderObject* FindShaderObject(size_t hash);
	static ShaderObject* FindShaderObject(const string& name);

	ShaderObject();
	~ShaderObject();
};

#endif // SHADEROBJECT_H
