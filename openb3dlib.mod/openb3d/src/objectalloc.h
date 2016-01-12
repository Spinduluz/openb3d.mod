#ifndef OBJECTALLOC_H
#define OBJECTALLOC_H

#include <memory.h>
//#include <string>
#include "bmaxdebug.h"

using namespace std;

class ObjectAllocBase{
public:
	int count;

	ObjectAllocBase():count(0){
	}
	virtual ~ObjectAllocBase(){
#if defined(BLITZMAX_DEBUG)
		DebugLog("%i objects still allocated for object %s",count,objectclass.c_str());
#endif
	}

#if defined(BLITZMAX_DEBUG)
	string objectclass;
	ObjectAllocBase(const string& objectclass):count(0),objectclass(objectclass){
	}
#endif
};

template<typename type,int size>
class ObjectArrayAlloc : public ObjectAllocBase {
public:
	struct Type{
		type item;
		type *item_ptr;

		Type():item(),item_ptr(NULL){}
	};

	Type array[size];

	ObjectArrayAlloc():ObjectAllocBase(){
	}
#if defined(BLITZMAX_DEBUG)
	ObjectArrayAlloc(const string objectclass):ObjectAllocBase(objectclass){
	}
#endif
	~ObjectArrayAlloc(){
	}

	void* alloc(){
		if(count==size) return NULL;
		for(int i=0;i<size;i++){
			if(!array[i].item_ptr){
				array[i].item_ptr=&array[i].item;
				count++;
				return array[i].item_ptr;
			}
		}
		return NULL;
	}

	void remove(void* ptr){
		for(int i=0;i<size;i++){
			if(array[i].item_ptr==(type*)ptr){
				array[i].item_ptr->~type();
				array[i].item_ptr=NULL;
				count--;
				return;
			}
		}
	}
};

#define VECTOR_ALLOC_GRANULARITY 128

template<typename type>
class ObjectVectorAlloc : public ObjectAllocBase{
public:
	struct Type{
		type item;
		type *item_ptr;

		Type():item(),item_ptr(NULL){}
	};

	vector<Type> array;

	ObjectVectorAlloc():ObjectAllocBase(){
		size_t size=0;

		size=size+VECTOR_ALLOC_GRANULARITY-(size%VECTOR_ALLOC_GRANULARITY);
		array.resize(size);
	}
	~ObjectVectorAlloc(){
	}
#if defined(BLITZMAX_DEBUG)
	ObjectVectorAlloc(const string& objectclass):ObjectAllocBase(objectclass){
		size_t size=0;

		size=size+VECTOR_ALLOC_GRANULARITY-(size%VECTOR_ALLOC_GRANULARITY);
		array.resize(size);
	}
#endif

	void* alloc(){
		if(count==array.capacity()){
			size_t size=array.capacity()+1;
			size=size+VECTOR_ALLOC_GRANULARITY-(size%VECTOR_ALLOC_GRANULARITY);
			array.resize(size);
		}

		for(int i=0;i<array.capacity();i++){
			if(!array[i].item_ptr){
				array[i].item_ptr=&array[i].item;
				count++;
				return array[i].item_ptr;
			}
		}
		return NULL;
	}

	void remove(void* ptr){
		for(int i=0;i<array.capacity();i++){
			if(array[i].item_ptr==ptr){
				array[i].item_ptr->~type();
				array[i].item_ptr=NULL;
				count--;
				return;
			}
		}
	}
};

template<typename type>
class ObjectStandardAlloc : public ObjectAllocBase{
public:
	ObjectStandardAlloc():ObjectAllocBase(){
	}
	~ObjectStandardAlloc(){
	}
#if defined(BLITZMAX_DEBUG)
	ObjectStandardAlloc(const string& objectclass):ObjectAllocBase(objectclass){
	}
#endif

	void* alloc(){
		count++;
		type *r=::new type;
		return (void*)r;
	}

	void remove(void* ptr){
		count--;
		::delete ((type*)ptr);
	}
};

// Ok, this is messy.
// But it works as a quick and dirty solution.
// I don't reallt wan't to redesign the classes too much right now
// especially since I plan to implement these on all core classes

class Texture;
class Shader;
class ProgramObject;
class ShaderObject;
class Mesh;
class Brush;
class Surface;

#if defined(MAX_TEXTURE_COUNT)
typedef ObjectArrayAlloc<Texture,MAX_TEXTURE_COUNT> TextureAlloc;
#elif defined(TEXTURE_VECTOR_ALLOC)
typedef ObjectVectorAlloc<Texture> TextureAlloc;
#else
typedef ObjectStandardAlloc<Texture> TextureAlloc;
#endif

#if defined(MAX_SHADER_COUNT)
typedef ObjectArrayAlloc<Shader,MAX_SHADER_COUNT> ShaderAlloc;
#elif defined(SHADER_VECTOR_ALLOC)
typedef ObjectVectorAlloc<Shader> ShaderAlloc;
#else
typedef ObjectStandardAlloc<Shader> ShaderAlloc;
#endif

#if defined(MAX_PROGRAMOBJECT_COUNT)
typedef ObjectArrayAlloc<ProgramObject,MAX_PROGRAMOBJECT_COUNT> ProgramObjectAlloc;
#elif defined(PROGRAMOBJECT_VECTOR_ALLOC)
typedef ObjectVectorAlloc<ProgramObject> ProgramObjectAlloc;
#else
typedef ObjectStandardAlloc<ProgramObject> ProgramObjectAlloc;
#endif

#if defined(MAX_SHADEROBJECT_COUNT)
typedef ObjectArrayAlloc<ShaderObject,MAX_SHADEROBJECT_COUNT> ShaderObjectAlloc;
#elif defined(SHADEROBJECT_VECTOR_ALLOC)
typedef ObjectVectorAlloc<ShaderObject> ShaderObjectAlloc;
#else
typedef ObjectStandardAlloc<ShaderObject> ShaderObjectAlloc;
#endif

typedef ObjectStandardAlloc<Mesh> MeshAlloc;
typedef ObjectStandardAlloc<Brush> BrushAlloc;
typedef ObjectStandardAlloc<Surface> SurfaceAlloc;

#define CLASS_ALLOCTOR_DECL(CLASS)			\
	static CLASS##Alloc objectAlloc;		\
	void* operator new (size_t){			\
		return objectAlloc.alloc();			\
	}										\
	void operator delete(void* ptr){		\
		objectAlloc.remove(ptr);			\
	}

#if defined(BLITZMAX_DEBUG) || defined(_DEBUG)
#define CLASS_ALLOCATOR_IMPL(CLASS)			\
	CLASS##Alloc CLASS::objectAlloc(#CLASS)
#else
#define CLASS_ALLOCATOR_IMPL(CLASS)			\
	CLASS##Alloc CLASS::objectAlloc;
#endif

#endif // OBJECTALLOC_H

