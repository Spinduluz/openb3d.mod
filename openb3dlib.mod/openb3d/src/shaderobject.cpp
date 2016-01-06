#include <list>
#include <string>

#include "glew.h"

#include "surface.h"
#include "file.h"
#include "shaderobject.h"
#include "string_helper.h"

using namespace std;

#if defined(BLITZMAX_DEBUG)
int ShaderObject::debug_count=0;
int ProgramObject::debug_count=0;
#endif

list<ShaderObject*> ShaderObject::shaderobjects;

ShaderObject::ShaderObject(){
}

ShaderObject::~ShaderObject(){
	ShaderObject::shaderobjects.remove(this);
	glDeleteShader(object);
}

ShaderObject* ShaderObject::Create(GLenum type, const string& src){
	if(src.empty()) return NULL;
	// wrong size "void main(){}"
	if (src.length()<13){
		return NULL;
	}
	//int ShaderObject;
	ShaderObject* shader = new ShaderObject;
	
	shader->object=glCreateShader(type);
	shader->type=type;

	// Send shader to ShaderObject, then compile it
	const GLchar *source[]={src.c_str(),NULL};
	glShaderSource(shader->object,1,source,0);
	glCompileShader(shader->object);

	// Did the shader compile successfuly?
	int compiled;
	glGetShaderiv(shader->object,GL_COMPILE_STATUS, &compiled);
	if (!compiled){
		delete shader;
		return NULL;
	}

	stringstream s;
	s << "Unnamed" << ShaderObject::shaderobjects.size();


	shader->name=s.str();
	shader->hash=StringHash(shader->name);

	ShaderObject::shaderobjects.push_back(shader);
	
	return shader;
}

ShaderObject* ShaderObject::CreateFromFile(GLenum type,const string& filename){
	if(filename.empty()) return NULL;

	size_t hash=StringHash(filename);
	ShaderObject *shader=FindShaderObject(hash);
	if(shader) return shader;

	FilePtr file=File::ReadFile(filename);
	if(!file) return NULL;

	int filelen=file->FileSize();
	if(filelen<13) return NULL;

	string src;
	while(!file->Eof()){
		src+=file->ReadByte();
	}
	src+='\0';

	shader=Create(type,src);

	shader->name=filename;
	shader->hash=hash;

	return shader;
}

ShaderObject* ShaderObject::FindShaderObject(size_t hash){
	for(ShaderObject *shader : ShaderObject::shaderobjects){
		if(shader->hash==hash){
			shader->AddRef();
			return shader;
		}
	}
	return NULL;
}

ShaderObject* ShaderObject::FindShaderObject(const string& name){
	size_t hash=StringHash(name);
	return FindShaderObject(hash);
}

// Are you ever used? Ever? No?
void ShaderObject::Delete(ShaderObject *shader){
	if (!shader) return;

	for(ProgramObject* o : ProgramObject::programobjects){
		o->DetachShader(shader);
	}
	
	glDeleteShader(shader->object);
	ShaderObject::shaderobjects.remove(shader);
	delete shader;
}	

