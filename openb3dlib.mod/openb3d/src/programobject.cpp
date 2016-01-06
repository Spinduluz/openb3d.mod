#include "shadermat.h"
#include "surface.h"
#include "glew.h"

#include "string_helper.h"

int ProgramObject::default_program=0;
list<ProgramObject*> ProgramObject::programobjects;

ProgramObject* ProgramObject::Create(string name){
	ProgramObject* p = new ProgramObject;
		
	//Create a new GL ProgramObject
	p->program = glCreateProgram();
	if (!p->program){
		delete p;
		return NULL;
	}
	/*--------------------------------------
	'These lists contain any Vert or Frag
	'Shader Objects Attached to this Program
	'-------------------------------------
	p->vList:TList = CreateList();
	p->fList:TList = CreateList();*/
	
	//This Program Objects Name
	p->name=name;
	p->hash=StringHash(name);
		
	/*-----------------------------
	'Add this Program Object to the
	'Global list of ProgramObjects
	'------------------------------*/
	programobjects.push_back(p);
	return p;
}

ProgramObject::~ProgramObject(){
	glUseProgram(0);
	for(ShaderObject *shader : shaderobjects){
		glDetachShader(program,shader->object);
		shader->attached.remove(this);
		shader->DestroyRef();
	}
	glDeleteProgram(program);
}

void ProgramObject::Activate(){
	glUseProgram(program);
}

void ProgramObject::DeActivate(){
	glUseProgram(default_program);
}

void ProgramObject::RefreshTypeMap(){
	TypeMap.clear();
		
	int maxlen;
	int count;
		 
	glGetProgramiv(program,GL_ACTIVE_UNIFORM_MAX_LENGTH,&maxlen);
	glGetProgramiv(program,GL_ACTIVE_UNIFORMS,&count);

	GLenum type;
	int size;

	char *name=RequestTempString(maxlen+1);
	for(int i=0;i<=count;i++){
		glGetActiveUniform(program,i,maxlen,0,&size,&type,name);
		TypeMap[name]=1; // *Sigh* Uniform
	}
	HandleRequestedTempString(name,maxlen+1);

	glGetProgramiv(program,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&maxlen);
	glGetProgramiv(program,GL_ACTIVE_ATTRIBUTES,&count);

	name=RequestTempString(maxlen+1);
	for(int i=0;i<=count;i++){
		glGetActiveAttrib(program,i,maxlen,0,&size,&type,name);
		TypeMap[name]=2; // *Sigh* Attribute
	}
	HandleRequestedTempString(name,maxlen+1);
}

// Get the Uniform Variable Location from a ProgramObject
int ProgramObject::GetUniformLocation(const string& name){
	return glGetUniformLocation(program,name.c_str());
}

// Get the Attribute Variable Location from a ProgramObject
int ProgramObject::GetAttribLocation(const string& name){
	return glGetAttribLocation(program,name.c_str());
}

void ProgramObject::SetParameter1S(const string& name,float v1){
	int location=glGetAttribLocation(program,name.c_str());
	glVertexAttrib1s(location,v1);
}
	
void ProgramObject::SetParameter2S(const string& name,float v1,float v2) {
	int location= glGetAttribLocation(program,name.c_str());
	glVertexAttrib2s(location,v1,v2);
}
	 
void ProgramObject::SetParameter3S(const string& name,float v1,float v2,float v3){
	int location= glGetAttribLocation(program, name.c_str());
	glVertexAttrib3s(location,v1,v2,v3);
}
	  
void ProgramObject::SetParameter4S(const string& name,float v1,float v2,float v3,float v4){
	int location= glGetAttribLocation(program,name.c_str());
	glVertexAttrib4s(location,v1,v2,v3,v4);
}
	
//------------------------------------------------------------
// Int Parameter
	
void ProgramObject::SetParameter1I(const string& name,int v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform1i(location,v1);
}

void ProgramObject::SetParameter2I(const string& name,int v1,int v2){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform2i(location,v1,v2);
}

void ProgramObject::SetParameter3I(const string& name,int v1,int v2,int v3){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform3i(location,v1,v2,v3);
}

void ProgramObject::SetParameter4I(const string& name,int v1,int v2,int v3,int v4){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform4i(location,v1,v2,v3,v4);
}
	
//----------------------------------------------------------------------------------
// Int Vectors

void ProgramObject::SetVector1I(const string& name,int* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform1iv(location,1,v1);
}
				
void ProgramObject::SetVector2I(const string& name,int* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform2iv(location,1,v1);
}
										
void ProgramObject::SetVector3I(const string& name,int* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform3iv(location,1,v1);
}
										
void ProgramObject::SetVector4I(const string& name,int* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform4iv(location,1,v1);
}
				
//-------------------------------------------------------------------------------------
// Double Parameter ( automatically Attributes, because Uniform doubles does not exist)
	
void ProgramObject::SetParameter1D(const string& name,double v1){
	int location=glGetAttribLocation(program,name.c_str());
	glVertexAttrib1d(location,v1);
}
	 
void ProgramObject::SetParameter2D(const string& name,double v1,double v2){
	int location=glGetAttribLocation(program,name.c_str());
	glVertexAttrib2d(location,v1,v2);
}
	 
void ProgramObject::SetParameter3D(const string& name,double v1,double v2,double v3){
	int location=glGetAttribLocation(program,name.c_str());
	glVertexAttrib3d(location,v1,v2,v3);
}
	 
void ProgramObject::SetParameter4D(const string& name,double v1,double v2,double v3,double v4){
	int location=glGetAttribLocation(program,name.c_str());
	glVertexAttrib4d(location,v1,v2,v3,v4);
}


//-------------------------------------------------------------------------------------
// Array Parameter

void ProgramObject::SetParameterArray(const string& name,Surface* surf,int vbo){
	int location=glGetAttribLocation(program,name.c_str());

	if(surf->vbo_enabled){
		surf->reset_vbo=vbo;
		glBindBuffer(GL_ARRAY_BUFFER,0);
		switch (vbo){
		case 1:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[0]);
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,0,0);
			break;
		case 2:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[1]);
			glVertexAttribPointer(location,2,GL_FLOAT,GL_FALSE,0,0);
			break;
		case 3:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[2]);
			glVertexAttribPointer(location,2,GL_FLOAT,GL_FALSE,0,0);
			break;
		case 4:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[3]);
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,0,0);
			break;
		case 5:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[4]);
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,16,0);
			break;

		case 6:
			glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[4]);
			glVertexAttribPointer(location,4,GL_FLOAT,GL_FALSE,0,0);
			break;
		}
	}else{
		glBindBuffer(GL_ARRAY_BUFFER,0);
		switch (vbo){
		case 1:
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,0,&surf->vert_coords[0]);
			break;
		case 2:
			glVertexAttribPointer(location,2,GL_FLOAT,GL_FALSE,0,&surf->vert_tex_coords0[0]);
			break;
		case 3:
			glVertexAttribPointer(location,2,GL_FLOAT,GL_FALSE,0,&surf->vert_tex_coords1[0]);
			break;
		case 4:
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,0,&surf->vert_norm[0]);
			break;
		case 5:
			glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,16,&surf->vert_col[0]);
			break;

		case 6:
			glVertexAttribPointer(location,4,GL_FLOAT,GL_FALSE,0,&surf->vert_col[0]);
			break;
		}
	}
	glEnableVertexAttribArray(location);
}

void ProgramObject::SetParameterArray(const string& name,vector<float>* verticesptr,int vbo){
	vector<float>&vertices=*verticesptr;
	int location=glGetAttribLocation(program,name.c_str());

	//special case, terrain surface
	glBindBuffer(GL_ARRAY_BUFFER,0);
	switch (vbo){
	case 1:
		glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,32,&vertices[0]);
		break;
	case 2:
	case 3:
		glVertexAttribPointer(location,2,GL_FLOAT,GL_FALSE,32,&vertices[6]);
		break;
	case 4:
		glVertexAttribPointer(location,3,GL_FLOAT,GL_FALSE,32,&vertices[3]);
		break;
	}
	glEnableVertexAttribArray(location);
	return;
}


//-------------------------------------------------------------------------------------
// Float Parameter

void ProgramObject::SetParameter1F(const string& name,float v){
	if(TypeMap.find(name)==TypeMap.end()){
		return;
	}

	int location;
	int type=TypeMap.find(name)->second;
	
	if(type==1){		//"Uniform" 
		location=glGetUniformLocation(program,name.c_str());
		glUniform1f(location,v);			
	}else if(type==2){ //"Attribute"
		location=glGetAttribLocation(program,name.c_str());
		glVertexAttrib1f(location,v);
	}
		
}

void ProgramObject::SetParameter2F(const string& name,float v1,float v2){
	if (TypeMap.find(name)==TypeMap.end()){
		return;
	}

	int location;
	int type=TypeMap.find(name)->second;
	
	if(type==1){		//"Uniform" 
		location=glGetUniformLocation(program,name.c_str());
		glUniform2f(location,v1,v2);
			
	}else if(type==2){	//"Attribute"
		location=glGetAttribLocation(program,name.c_str());
		glVertexAttrib2f(location,v1,v2);
	}

}

void ProgramObject::SetParameter3F(const string& name,float v1,float v2,float v3){
		
	if (TypeMap.find(name)==TypeMap.end() ){
		return;
	}

	int location;
	int type=TypeMap.find(name)->second;
		
	if(type==1){		//"Uniform" 
		location=glGetUniformLocation(program,name.c_str());
		glUniform3f(location,v1,v2,v3);
	}else if(type==2){	//"Attribute"
		location=glGetAttribLocation(program,name.c_str());
		glVertexAttrib3f(location,v1,v2,v3);
	}
}

void ProgramObject::SetParameter4F(const string& name,float v1,float v2,float v3,float v4){
	if (TypeMap.find(name)==TypeMap.end() ){
		return;
	}

	int location;
	int type=TypeMap.find(name)->second;
		
	if(type==1){		//"Uniform" 
		location=glGetUniformLocation(program,name.c_str());
		glUniform4f(location,v1,v2,v3,v4);	
	}else if(type==2){ //"Attribute"
		location=glGetAttribLocation(program,name.c_str());
		glVertexAttrib4f(location,v1,v2,v3,v4);
	}
}

//---------------------------------------------------------------------------------------------------
// Float Vectors


void ProgramObject::SetVector1F(const string& name,float* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform1fv(location,1,v1);
}

void ProgramObject::SetVector2F(const string& name,float* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform2fv(location,1,v1);
}

void ProgramObject::SetVector3F(const string& name,float* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform3fv(location,1,v1);
}

void ProgramObject::SetVector4F(const string& name,float* v1){
	int location=glGetUniformLocation(program,name.c_str());
	glUniform4fv(location,1,v1);
}


//--------------------------------------------------------------------------------------------------
// Matrices

void ProgramObject::SetMatrix2F(const string& name,float* m){
	int location=glGetUniformLocation(program,name.c_str());
	glUniformMatrix2fv(location,1,0,m);
} 

void ProgramObject::SetMatrix3F(const string& name,float* m){
	int location=glGetUniformLocation(program,name.c_str());
	glUniformMatrix3fv(location,1,0,m);	
}

void ProgramObject::SetMatrix4F(const string& name,float* m){
	int location=glGetUniformLocation(program,name.c_str());
	glUniformMatrix4fv(location,1,0,m);
}

bool ProgramObject::AttachShader(ShaderObject *shader){
	glAttachShader(program,shader->object);
	glLinkProgram(program);

	int linked;
	glValidateProgram(program);
	glGetProgramiv(program,GL_LINK_STATUS,&linked);
	if(!linked) return false;

	shaderobjects.push_back(shader);
	shader->attached.push_back(this);
	RefreshTypeMap();

	return true;
}

void ProgramObject::DetachShader(ShaderObject* shader){
	if(!shader) return;

	list<ShaderObject*>::iterator i,b,e;
	
	b=shaderobjects.begin();
	e=shaderobjects.end();
	
	for(i=b;i!=e;i++){
		if(*i==shader){
			glDetachShader(program,shader->object);
			// Remove references
			shaderobjects.remove(shader);
			shader->attached.remove(this);
			// Let this one go out of scope
			if(!shader->attached.size()) ShaderObject::Delete(shader);
			break;
		}
	}
}
