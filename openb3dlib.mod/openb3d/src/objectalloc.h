#ifndef OBJECTALLOC_H
#define OBJECTALLOC_H

#include <memory.h>
#include <stdlib.h>
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
			if(array[i].item_ptr==ptr){
				array[i].item.~type();
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
				array[i].item.~type();
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
		return malloc(sizeof(type));
	}

	void remove(void* ptr){
		count--;
		free(ptr);
	}
};

// Ok, this is messy.
// But it works as a quick and dirty solution.
// I don't reallt wan't to redesign the classes too much right now
// especially since I plan to implement these on all core classes

#if defined(MAX_TEXTURE_COUNT)
typedef ObjectArrayAlloc<class Texture,MAX_TEXTURE_COUNT> TextureAlloc;
#elif defined(TEXTURE_VECTOR_ALLOC)
typedef ObjectVectorAlloc<class Texture> TextureAlloc;
#else
typedef ObjectStandardAlloc<class Texture> TextureAlloc;
#endif

#if defined(MAX_SHADER_COUNT)
typedef ObjectArrayAlloc<class Shader,MAX_SHADER_COUNT> ShaderAlloc;
#elif defined(SHADER_VECTOR_ALLOC)
typedef ObjectVectorAlloc<class Shader> ShaderAlloc;
#else
typedef ObjectStandardAlloc<class Shader> ShaderAlloc;
#endif

#if defined(MAX_PROGRAMOBJECT_COUNT)
typedef ObjectArrayAlloc<class ProgramObject,MAX_PROGRAMOBJECT_COUNT> ProgramObjectAlloc;
#elif defined(PROGRAMOBJECT_VECTOR_ALLOC)
typedef ObjectVectorAlloc<class ProgramObject> ProgramObjectAlloc;
#else
typedef ObjectStandardAlloc<class ProgramObject> ProgramObjectAlloc;
#endif

#if defined(MAX_SHADEROBJECT_COUNT)
typedef ObjectArrayAlloc<class ShaderObject,MAX_SHADEROBJECT_COUNT> ShaderObjectAlloc;
#elif defined(SHADEROBJECT_VECTOR_ALLOC)
typedef ObjectVectorAlloc<class ShaderObject> ShaderObjectAlloc;
#else
typedef ObjectStandardAlloc<class ShaderObject> ShaderObjectAlloc;
#endif

#define TYPEDEF_STANDARD_ALLOC(CLASS) typedef ObjectStandardAlloc<class CLASS> CLASS##Alloc

TYPEDEF_STANDARD_ALLOC(Mesh);
TYPEDEF_STANDARD_ALLOC(Brush);
TYPEDEF_STANDARD_ALLOC(Surface);
TYPEDEF_STANDARD_ALLOC(Light);
TYPEDEF_STANDARD_ALLOC(Camera);
TYPEDEF_STANDARD_ALLOC(Pivot);
TYPEDEF_STANDARD_ALLOC(Terrain);
TYPEDEF_STANDARD_ALLOC(Sprite);
TYPEDEF_STANDARD_ALLOC(VoxelSprite);
TYPEDEF_STANDARD_ALLOC(Fluid);
TYPEDEF_STANDARD_ALLOC(Geosphere);
TYPEDEF_STANDARD_ALLOC(ParticleBatch);
TYPEDEF_STANDARD_ALLOC(ParticleEmitter);

#define CLASS_ALLOCTOR_DECL(CLASS)					\
	static CLASS##Alloc object##CLASS##Alloc;		\
	void* operator new (size_t){					\
		return object##CLASS##Alloc.alloc();		\
	}												\
	void operator delete(void* ptr){				\
		object##CLASS##Alloc.remove(ptr);			\
	}

#if defined(BLITZMAX_DEBUG) || defined(_DEBUG)
#define CLASS_ALLOCATOR_IMPL(CLASS)					\
	CLASS##Alloc CLASS::object##CLASS##Alloc(#CLASS)
#else
#define CLASS_ALLOCATOR_IMPL(CLASS)					\
	CLASS##Alloc CLASS::object##CLASS##Alloc;
#endif

#endif // OBJECTALLOC_H

