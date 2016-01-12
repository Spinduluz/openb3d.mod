#ifndef SHADEROBJECT_H
#define SHADEROBJECT_H

#include <map>
#include <vector>

#include "refobject.h"
#include "objectalloc.h"

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

class ProgramObject;
class Surface;

#if defined(_MSC_VER)
#	define MAX_SHADEROBJECT_COUNT 512
#endif

// New implementation of custom allocators (not required)
//	This is implemetation is twofold.
//	One: 
//	All objects are kept on the stack. 
//	This limits the number of ShaderObjects that can be created but removes the need for allocations/deallocations
//  Two:
//	Debugging. Using this I can see how many objects that are still allocated on application exit
//	Set ModuleInfo "CCFLAGS: -DMAX_SHADEROBJECT_COUNT=[value] in openb3dlib/openb3dlib.bmx to enable this

class ShaderObject : public ReferencedObject {
public:
	CLASS_ALLOCTOR_DECL(ShaderObject)
#if defined(BLITZMAX_DEBUG)
	static int debug_count;
#endif
	static list<ShaderObject*> shaderobjects;

	int object;
	int type;

	string name;
	size_t hash;

	// Remove? This one just confuses me.. well not really but can't see much point of it
	list<ProgramObject*> attached; // Shaders can be attached, or 'referenced by', more than 1 ProgramObject

	static ShaderObject* Create(unsigned int type,const string& src,const string& name);
	static ShaderObject* CreateFromFile(unsigned int type,const string& filename);
#if 0
	// FIXME: Remove. This function has now real point now
	static void Delete(ShaderObject* shader);
#endif
	// These methods increases the reference count.
	static ShaderObject* FindShaderObject(size_t hash);
	static ShaderObject* FindShaderObject(const string& name);

	ShaderObject();
	~ShaderObject();
};

#endif // SHADEROBJECT_H
