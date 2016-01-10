#include <iostream>
#include <string>

#include <GL/glew.h>

#include "surface.h"
#include "camera.h"
#include "shadermat.h"
#include "file.h"
#include "global.h"
#include "stb_image.h"

//static int default_program=0;

enum{
	USE_FLOAT_1,
	USE_FLOAT_2,
	USE_FLOAT_3,
	USE_FLOAT_4,
	USE_INTEGER_1,
	USE_INTEGER_2,
	USE_INTEGER_3,
	USE_INTEGER_4,
	USE_ENTITY_COORDS,
	USE_SURFACE,
	USE_MODEL_MATRIX,
	USE_VIEW_MATRIX,
	USE_PROJ_MATRIX,
	USE_MODELVIEW_MATRIX
};

#if defined(BLITZMAX_DEBUG)
//static _AllocatedStuff _activeitems;
#endif

// ==========================================================================================================
//ShaderMat

#if defined(BLITZMAX_DEBUG)
int Sampler::debug_count=0;
#endif

Sampler* Sampler::Create(string name,int slot,Texture* tex){
	return new Sampler(name,slot,tex);
}

// ==========================================================================================================
#if defined(BLITZMAX_DEBUG)
int Shader::debug_count=0;
#endif
int Shader::ShaderIDCount;
	
Shader* Shader::CreateShaderMaterial(const string& name){
	Shader* shader=new Shader;
	if (1){//HardwareInfo->ShaderSupport{
		stringstream s;
		shader->id=ShaderIDCount;
		s << (name.empty()?"NoName":name);
		if(name.empty()) s << shader->id;

		shader->arb_program=ProgramObject::Create(s.str());
		shader->name = s.str();
	}else{
		return NULL;
	}
	ShaderIDCount++;

	shader->shader_texture.fill(NULL);

	return shader;
}
	
/*void Shader::UpdateData(Surface* surf) {
	for (unsigned int i=0;i<Parameters.size();i++){
		switch(Parameters[i].type){
		case 0:
			if (arb_program){arb_program->SetParameter1F(Parameters[i].name,*(Parameters[i].fp[0]));}
			break;
		case 7:
			if (arb_program){arb_program->SetParameter3F(Parameters[i].name, 
			Parameters[i].ent->EntityX(), 
			Parameters[i].ent->EntityY(),
			Parameters[i].ent->EntityZ());}
			break;
		case 13:
			if (arb_program){arb_program->SetParameterArray(Parameters[i].name,Parameters[i].surf,Parameters[i].vbo);}
			break;
		case 14:
			if (arb_program){
				GLfloat modelViewMatrix[16];
				glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
				arb_program->SetMatrix4F(Parameters[i].name, modelViewMatrix);
			}
			break;

		case 15:
			if (arb_program){
				GLfloat modelViewMatrix[16];
				glGetFloatv(GL_PROJECTION_MATRIX, modelViewMatrix);
				arb_program->SetMatrix4F(Parameters[i].name, modelViewMatrix);
			}
			break;


		//default:
		}
	}
}
	*/

Shader::Shader():shader_texture(),arb_program(NULL),id(0),name(),updatesampler(false),parameters(){
}

Shader::~Shader(){
	for(int i=0; i<255; i++){
		if(shader_texture[i]) delete shader_texture[i];
	}
	if(arb_program) arb_program->DestroyRef();
#if defined(BLITZMAX_DEBUG)
	DebugLog("Deleted shader %s",name.c_str());
#endif
}

// internal 
void Shader::TurnOn(Matrix& mat, Surface* surf, vector<float>* vertices){
	ProgramAttriBegin();
	// Update Data
#if 1
	for(ShaderData& param : parameters){
		switch(param.type){
		case USE_FLOAT_1:
			if (arb_program) arb_program->SetParameter1F(param.name,*(param.fp[0]));
			break;
		case USE_FLOAT_2:
			if (arb_program) arb_program->SetParameter2F(param.name,*(param.fp[0]),*(param.fp[1]));
			break;
		case USE_FLOAT_3:
			if (arb_program) arb_program->SetParameter3F(param.name,*(param.fp[0]),*(param.fp[1]),*(param.fp[2]));
			break;
		case USE_FLOAT_4:
			if (arb_program) arb_program->SetParameter4F(param.name,*(param.fp[0]),*(param.fp[1]),*(param.fp[2]),*(param.fp[3]));
			break;
		case USE_INTEGER_1:
			if (arb_program) arb_program->SetParameter1I(param.name,*(param.ip[0]));
			break;
		case USE_INTEGER_2:
			if (arb_program) arb_program->SetParameter2I(param.name,*(param.ip[0]),*(param.ip[1]));
			break;
		case USE_INTEGER_3:
			if (arb_program) arb_program->SetParameter3I(param.name,*(param.ip[0]),*(param.ip[1]),*(param.ip[2]));
			break;
		case USE_INTEGER_4:
			if (arb_program) arb_program->SetParameter4I(param.name,*(param.ip[0]),*(param.ip[1]),*(param.ip[2]),*(param.ip[3]));
			break;
		case USE_ENTITY_COORDS:
			if (arb_program) arb_program->SetParameter3F(param.name,param.ent->EntityX(),param.ent->EntityY(),param.ent->EntityZ());
			break;
		case USE_SURFACE:
			if (arb_program){
				if(param.surf){
					arb_program->SetParameterArray(param.name,param.surf,param.vbo);
				}else{
					if(surf){
						arb_program->SetParameterArray(param.name,surf,param.vbo);
					}else{
						arb_program->SetParameterArray(param.name,vertices,param.vbo);
					}
				}
			}
			break;
		case USE_MODEL_MATRIX:
			if (arb_program) arb_program->SetMatrix4F(param.name,&mat.grid[0][0]);
			break;
		case USE_VIEW_MATRIX:
			if (arb_program){
				Matrix new_mat;
				Global::camera_in_use->mat.GetInverse(new_mat);
				arb_program->SetMatrix4F(param.name,&new_mat.grid[0][0]);
			}
			break;

		case USE_PROJ_MATRIX:
			if (arb_program) arb_program->SetMatrix4F(param.name,&Global::camera_in_use->proj_mat[0]);
			break;
		case USE_MODELVIEW_MATRIX:
			if (arb_program){
				Matrix new_mat;
				Global::camera_in_use->mat.GetInverse(new_mat);
				new_mat.Multiply(mat);
				arb_program->SetMatrix4F(param.name, &new_mat.grid[0][0]);
			}
			break;


		//default:
		}
	}
#else
	for (unsigned int i=0;i<parameters.size();i++){
		switch(parameters[i].type){
		case USE_FLOAT_1:
			if (arb_program){arb_program->SetParameter1F(parameters[i].name,*(parameters[i].fp[0]));}
			break;
		case USE_FLOAT_2:
			if (arb_program){arb_program->SetParameter2F(parameters[i].name,*(parameters[i].fp[0]),*(parameters[i].fp[1]));}
			break;
		case USE_FLOAT_3:
			if (arb_program){arb_program->SetParameter3F(parameters[i].name,*(parameters[i].fp[0]),*(parameters[i].fp[1]),
			*(parameters[i].fp[2]));}
			break;
		case USE_FLOAT_4:
			if (arb_program){arb_program->SetParameter4F(parameters[i].name,*(parameters[i].fp[0]),*(parameters[i].fp[1]),
			*(parameters[i].fp[2]),*(parameters[i].fp[3]));}
			break;
		case USE_INTEGER_1:
			if (arb_program){arb_program->SetParameter1I(parameters[i].name,*(parameters[i].ip[0]));}
			break;
		case USE_INTEGER_2:
			if (arb_program){arb_program->SetParameter2I(parameters[i].name,*(parameters[i].ip[0]),*(parameters[i].ip[1]));}
			break;
		case USE_INTEGER_3:
			if (arb_program){arb_program->SetParameter3I(parameters[i].name,*(parameters[i].ip[0]),*(parameters[i].ip[1]),
			*(parameters[i].ip[2]));}
			break;
		case USE_INTEGER_4:
			if (arb_program){arb_program->SetParameter4I(parameters[i].name,*(parameters[i].ip[0]),*(parameters[i].ip[1]),
			*(parameters[i].ip[2]),*(parameters[i].ip[3]));}
			break;
		case USE_ENTITY_COORDS:
			if (arb_program){arb_program->SetParameter3F(parameters[i].name, 
			parameters[i].ent->EntityX(), 
			parameters[i].ent->EntityY(),
			parameters[i].ent->EntityZ());}
			break;
		case USE_SURFACE:
			if (arb_program){
				if(parameters[i].surf){
					arb_program->SetParameterArray(parameters[i].name,parameters[i].surf,parameters[i].vbo);
				}else{
					if(surf){
						arb_program->SetParameterArray(parameters[i].name,surf,parameters[i].vbo);
					}else{
						arb_program->SetParameterArray(parameters[i].name,vertices,parameters[i].vbo);
					}
				}
			}
			break;
		case USE_MODEL_MATRIX:
			if (arb_program) arb_program->SetMatrix4F(parameters[i].name, &mat.grid[0][0]);
			break;
		case USE_VIEW_MATRIX:
			if (arb_program){
				Matrix new_mat;
				Global::camera_in_use->mat.GetInverse(new_mat);
				arb_program->SetMatrix4F(parameters[i].name,&new_mat.grid[0][0]);
			}
			break;

		case USE_PROJ_MATRIX:
			if (arb_program) arb_program->SetMatrix4F(parameters[i].name,&Global::camera_in_use->proj_mat[0]);
			break;
		case USE_MODELVIEW_MATRIX:
			if (arb_program){
				Matrix new_mat;
				Global::camera_in_use->mat.GetInverse(new_mat);
				new_mat.Multiply(mat);
				arb_program->SetMatrix4F(parameters[i].name, &new_mat.grid[0][0]);
			}
			break;


		//default:
		}
	}
#endif
	if (updatesampler){
		for (int i=0; i<255; i++){
			//if (shader_texture[i] == 0) break;
			if(!shader_texture[i]) break;
			if(arb_program) arb_program->SetParameter1I(shader_texture[i]->name,shader_texture[i]->slot);
		}
		updatesampler=false;
	}
	
	if (surf) {
		int DisableCubeSphereMapping=0;
		for (int ix=0;ix<255;ix++){
			if (!shader_texture[ix]) break;
			// Main brush texture takes precedent over surface brush texture
			unsigned int texture=0;
			int tex_flags=0;
			int tex_blend=0;
			int tex_coords=0;

			float tex_u_scale=1.0;
			float tex_v_scale=1.0;
			float tex_u_pos=0.0;
			float tex_v_pos=0.0;
			float tex_ang=0.0;

			int tex_cube_mode=0;//,frame=0;

			texture=shader_texture[ix]->texture->texture;
			tex_flags=shader_texture[ix]->texture->flags;
			tex_blend=shader_texture[ix]->texture->blend;
			tex_coords=shader_texture[ix]->texture->coords;
			tex_u_scale=shader_texture[ix]->texture->u_scale;
			tex_v_scale=shader_texture[ix]->texture->v_scale;
			tex_u_pos=shader_texture[ix]->texture->u_pos;
			tex_v_pos=shader_texture[ix]->texture->v_pos;
			tex_ang=shader_texture[ix]->texture->angle;
			tex_cube_mode=shader_texture[ix]->texture->cube_mode;
											
			glActiveTexture(GL_TEXTURE0+ix);
			glClientActiveTexture(GL_TEXTURE0+ix);

			if (shader_texture[ix]->is_texture3d){
				glEnable(GL_TEXTURE_3D);
				glBindTexture(GL_TEXTURE_3D,texture); // call before glTexParameteri
				// mipmapping texture flag
				if(tex_flags&8){
					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				}else{
					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				}
				
			}else{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,texture); // call before glTexParameteri
				// mipmapping texture flag
				if(tex_flags&8){
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
				}else{
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				}
			}

			// masked texture flag
			if(tex_flags&4){
				glEnable(GL_ALPHA_TEST);
			}else{
				glDisable(GL_ALPHA_TEST);
			}
		
		
			// clamp u flag
			if(tex_flags&16){
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			}else{						
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			}
			
			// clamp v flag
			if(tex_flags&32){
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			}else{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			}
	
			// ***!ES***
			///*
			// spherical environment map texture flag
			if(tex_flags&64){
				glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
				glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				DisableCubeSphereMapping=1;
			}/*else{
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
			}*/
					
			// cubic environment map texture flag
			if(tex_flags&128){

				glEnable(GL_TEXTURE_CUBE_MAP);
				glBindTexture(GL_TEXTURE_CUBE_MAP,texture); // call before glTexParameteri
				
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
				
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);
				//glEnable(GL_TEXTURE_GEN_Q)
				if(tex_cube_mode==1){
					glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
					glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
					glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP);
				}
				
				if(tex_cube_mode==2){
					glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
					glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
					glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_NORMAL_MAP);
				}
				DisableCubeSphereMapping=1;
			}
			else if (DisableCubeSphereMapping!=0){

				glDisable(GL_TEXTURE_CUBE_MAP);
				
				// only disable tex gen s and t if sphere mapping isn't using them
				if((tex_flags&64)==0){
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
				}
				
				glDisable(GL_TEXTURE_GEN_R);
				//glDisable(GL_TEXTURE_GEN_Q)

			}
			
			switch(tex_blend){
				case 0: 
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
					break;
				case 1: 
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
					break;
				case 2: 
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
				//case 2 glTexEnvf(GL_TEXTURE_ENV,GL_COMBINE_RGB_EXT,GL_MODULATE);
					break;
				case 3: 
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD);
					break;
				case 4:
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT); 
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_DOT3_RGB_EXT); 
					break;
				case 5:
					glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_COMBINE);
					glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB,GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV,GL_RGB_SCALE,2.0);
					break;
				default: 
					glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
			}

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (shader_texture[ix]->is_texture3d){

				if(surf->vbo_enabled==true && surf->no_tris>=Global::vbo_min_tris){
			
					glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[2]);
					glTexCoordPointer(3,GL_FLOAT,0,NULL);
				
				}else{
			
					//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0)
					glTexCoordPointer(3,GL_FLOAT,0,&surf->vert_tex_coords1[0]);

				}

			}else{
				
				if(surf->vbo_enabled && surf->no_tris>=Global::vbo_min_tris){
			
					if(tex_coords==0){
						glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[1]);
						glTexCoordPointer(2,GL_FLOAT,0,NULL);
					}else{
						glBindBuffer(GL_ARRAY_BUFFER,surf->vbo_id[2]);
						glTexCoordPointer(2,GL_FLOAT,0,NULL);
					}
				
				}else{
			
					if(tex_coords==0){
						//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0) already reset above
						glTexCoordPointer(2,GL_FLOAT,0,&surf->vert_tex_coords0[0]);
					}else{
						//glBindBufferARB(GL_ARRAY_BUFFER_ARB,0)
						glTexCoordPointer(2,GL_FLOAT,0,&surf->vert_tex_coords1[0]);
					}

				}

			}

							
			// reset texture matrix
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
					
			if(tex_u_pos!=0.0 || tex_v_pos!=0.0){
				glTranslatef(tex_u_pos,tex_v_pos,0.0);
			}
			if(tex_ang!=0.0){
				glRotatef(tex_ang,0.0,0.0,1.0);
			}
			if(tex_u_scale!=1.0 || tex_v_scale!=1.0){
				glScalef(tex_u_scale,tex_v_scale,1.0);
			}

			///* ***!ES***
			// if spheremap flag=true then flip tex
			if(tex_flags&64){
				glScalef(1.0,-1.0,-1.0);
			}
			
			// if cubemap flag=true then manipulate texture matrix so that cubemap is displayed properly 
			if(tex_flags&128){

				glScalef(1.0,-1.0,-1.0);
				
				// get current modelview matrix (set in last camera update)
				float mod_mat[16];
				glGetFloatv(GL_MODELVIEW_MATRIX,&mod_mat[0]);
				// get rotational inverse of current modelview matrix
				Matrix new_mat;
				new_mat.LoadIdentity();
					
				new_mat.grid[0][0] = mod_mat[0];
				new_mat.grid[1][0] = mod_mat[1];
				new_mat.grid[2][0] = mod_mat[2];

				new_mat.grid[0][1] = mod_mat[4];
				new_mat.grid[1][1] = mod_mat[5];
				new_mat.grid[2][1] = mod_mat[6];

				new_mat.grid[0][2] = mod_mat[8];
				new_mat.grid[1][2] = mod_mat[9];
				new_mat.grid[2][2] = mod_mat[10];
					
				glMultMatrixf(&new_mat.grid[0][0]);

			}
			//*/
						
		}
	}

}
	
void Shader::TurnOff(){
	ProgramAttriEnd();
	for (int i=0; i<255; i++){
		if (!shader_texture[i]) break;

		glActiveTexture(GL_TEXTURE0+i);
		glClientActiveTexture(GL_TEXTURE0+i);
				
		// reset texture matrix
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				
		if (shader_texture[i]->is_texture3d){
			glDisable(GL_TEXTURE_3D);
		}else{
			glDisable(GL_TEXTURE_2D);
		}
				
		glDisable(GL_TEXTURE_CUBE_MAP);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
			
	}
	for (unsigned int i=0;i<parameters.size();i++){
		switch(parameters[i].type){
		case USE_SURFACE:
			if (arb_program){	
				int location=glGetAttribLocation(arb_program->program,parameters[i].name.c_str());
				glDisableVertexAttribArray(location);
			}
		}
	}


}
		

/*	Method AddShader(_vert:String = Null , _frag:String = Null)
'		If arb_program = Null Then Return
'		Local Vert:TShaderObject
'		Local Frag:TShaderObject
'		
'		If _Vert <> Null Then 
'			Vert = TGlobal.RenderInterface.CreateVertexShaderObj(arb_program.languageType,_vert,arb_program)
'			arb_program.attachVertShader(Vert)
'		EndIf
'		
'		If _frag <> Null Then 
'			frag = TGlobal.RenderInterface.CreatePixelShaderObj(arb_program.languageType,_frag,arb_program)
'			arb_program.attachFragShader(frag)
'		EndIf
'	End Method*/
	
void Shader::AddShader(string vsfilename, string fsfilename){
	if (!arb_program) return;

	ShaderObject* vs;
	ShaderObject* fs;
	
	if (!vsfilename.empty()){
		vs=ShaderObject::CreateFromFile(GL_VERTEX_SHADER,vsfilename);
		if(vs){
			arb_program->AttachShader(vs);
			vs->DestroyRef(); // AttachShader increases the reference count by 1
		}
	}
		
	if (!fsfilename.empty()){
		fs=ShaderObject::CreateFromFile(GL_FRAGMENT_SHADER,fsfilename);
		if(fs){
			arb_program->AttachShader(fs);
			fs->DestroyRef(); // AttachShader increases the reference count by 1
		}
	}
}
	
void Shader::AddShaderFromString(string vssrc,string fssrc,const string& name){
	if (!arb_program) return;

	ShaderObject* vs;
	ShaderObject* fs;
		
	if (!vssrc.empty()){
		vs=ShaderObject::Create(GL_VERTEX_SHADER,vssrc,name+(name.empty()?"":".vert"));
		if(vs){
			arb_program->AttachShader(vs);
			vs->DestroyRef(); // AttachShader increases the reference count by 1
		}
	}
		
	if (!fssrc.empty()){
		fs=ShaderObject::Create(GL_FRAGMENT_SHADER,fssrc,name+(name.empty()?"":".frag"));
		if(fs){
			arb_program->AttachShader(fs);
			fs->DestroyRef(); // AttachShader increases the reference count by 1
		}
	}
}



	/*Method ListShaders()
		If arb_program = Null Then Return
		'arb_program.ListAttachedShaders()
	End Method */

void Shader::AddSampler2D(string name,int slot,Texture* tex){
	shader_texture[slot]=Sampler::Create(name,slot,tex);
	updatesampler=true;
	shader_texture[slot]->is_texture3d=false;
	shader_texture.count++;
}
	
void Shader::AddSampler3D(string name,int slot,Texture* tex){
	shader_texture[slot] = Sampler::Create(name,slot,tex);
	updatesampler=true;
	shader_texture[slot]->is_texture3d=true;
	shader_texture.count++;
}


void Shader::ProgramAttriBegin(){
	if (arb_program){
		if (this==Global::ambient_shader){
			if (ProgramObject::default_program==arb_program->program) return;
			ProgramObject::default_program=arb_program->program;
		}
		arb_program->Activate();
	}
}	

void Shader::ProgramAttriEnd(){
	if (arb_program){
		if (ProgramObject::default_program==arb_program->program) return;
		arb_program->DeActivate();
	}
}	

void Shader::SetFloat(string name, float v1){
	ProgramAttriBegin();
	if (arb_program) arb_program->SetParameter1F(name,v1);
	ProgramAttriEnd();
}

void Shader::SetFloat2(string name, float v1, float v2){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter2F(name, v1, v2);}
	ProgramAttriEnd();
}

void Shader::SetFloat3(string name, float v1, float v2, float v3){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter3F(name, v1, v2, v3);}
	ProgramAttriEnd();
}

void Shader::SetFloat4(string name, float v1, float v2, float v3, float v4){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter4F(name, v1, v2, v3, v4);}
	ProgramAttriEnd();
}

void Shader::UseFloat(string name, float* v1){
	ShaderData data;
	data.name=name;
	data.type=USE_FLOAT_1;
	data.fp[0]=v1;
	parameters.push_back(data);
}

void Shader::UseFloat2(string name, float* v1, float* v2){
	ShaderData data;
	data.name=name;
	data.type=USE_FLOAT_2;
	data.fp[0]=v1;
	data.fp[1]=v2;
	parameters.push_back(data);
}

void Shader::UseFloat3(string name, float* v1, float* v2, float* v3){
	ShaderData data;
	data.name=name;
	data.type=USE_FLOAT_3;
	data.fp[0]=v1;
	data.fp[1]=v2;
	data.fp[2]=v3;
	parameters.push_back(data);
}

void Shader::UseFloat4(string name, float* v1, float* v2, float* v3, float* v4){
	ShaderData data;
	data.name=name;
	data.type=USE_FLOAT_4;
	data.fp[0]=v1;
	data.fp[1]=v2;
	data.fp[2]=v3;
	data.fp[3]=v4;
	parameters.push_back(data);
}

void Shader::SetInteger(string name, int v1){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter1I(name, v1);}
	ProgramAttriEnd();
}

void Shader::SetInteger2(string name, int v1, int v2){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter2I(name, v1, v2);}
	ProgramAttriEnd();
}

void Shader::SetInteger3(string name, int v1, int v2, int v3){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter3I(name, v1, v2, v3);}
	ProgramAttriEnd();
}

void Shader::SetInteger4(string name, int v1, int v2, int v3, int v4){
	ProgramAttriBegin();
	if (arb_program){arb_program->SetParameter4I(name, v1, v2, v3, v4);}
	ProgramAttriEnd();
}

void Shader::UseInteger(string name, int* v1){
	ShaderData data;
	data.name=name;
	data.type=USE_INTEGER_1;
	data.ip[0]=v1;
	parameters.push_back(data);
}

void Shader::UseInteger2(string name, int* v1, int* v2){
	ShaderData data;
	data.name=name;
	data.type=USE_INTEGER_2;
	data.ip[0]=v1;
	data.ip[1]=v2;
	parameters.push_back(data);
}

void Shader::UseInteger3(string name, int* v1, int* v2, int* v3){
	ShaderData data;
	data.name=name;
	data.type=USE_INTEGER_3;
	data.ip[0]=v1;
	data.ip[1]=v2;
	data.ip[2]=v3;
	parameters.push_back(data);
}

void Shader::UseInteger4(string name, int* v1, int* v2, int* v3, int* v4){
	ShaderData data;
	data.name=name;
	data.type=USE_INTEGER_4;
	data.ip[0]=v1;
	data.ip[1]=v2;
	data.ip[2]=v3;
	data.ip[3]=v4;
	parameters.push_back(data);
}



void Shader::UseSurface(string name, Surface* surf, int vbo){
	ShaderData data;
	data.name=name;
	data.type=USE_SURFACE;
	data.surf=surf;
	data.vbo=vbo;
	parameters.push_back(data);
}

void Shader::UseMatrix(string name, int mode){
	ShaderData data;
	data.name=name;
	if (mode==0) {		//model matrix
		data.type=USE_MODEL_MATRIX;
	}else if(mode==1){	//view matrix
		data.type=USE_VIEW_MATRIX;
	}else if(mode==2){	//projection matrix
		data.type=USE_PROJ_MATRIX;
	}else if(mode==3){	//modelview matrix
		data.type=USE_MODELVIEW_MATRIX;
	}
	parameters.push_back(data);
}
/*void Shader::SetParameter1S(string name, float v1){
	if (arb_program){arb_program->SetParameter1S(name, v1);}
}

void Shader::SetParameter2S(string name, float v1, float v2){
	if (arb_program){arb_program->SetParameter2S(name, v1, v2);}
}

void Shader::SetParameter3S(string name, float v1, float v2, float v3){
	if (arb_program){arb_program->SetParameter3S(name, v1, v2, v3);}
}

void Shader::SetParameter4S(string name, float v1, float v2, float v3, float v4){
	if (arb_program){arb_program->SetParameter4S(name, v1, v2, v3, v4);}
}

void Shader::SetParameter1I(string name, int v1){
	if (arb_program){arb_program->SetParameter1I(name, v1);}
}

void Shader::SetParameter2I(string name, int v1, int v2){
	if (arb_program){arb_program->SetParameter2I(name, v1, v2);}
}

void Shader::SetParameter3I(string name, int v1, int v2, int v3){
	if (arb_program){arb_program->SetParameter3I(name, v1, v2, v3);}
}

void Shader::SetParameter4I(string name, int v1, int v2, int v3, int v4){
	if (arb_program){arb_program->SetParameter4I(name, v1, v2, v3, v4);}
}
 
void Shader::SetVector1I(string name, int* v1){
	if (arb_program){arb_program->SetVector1I(name, v1);}
}

void Shader::SetVector2I(string name, int* v1){
	if (arb_program){arb_program->SetVector2I(name, v1);}
}

void Shader::SetVector3I(string name, int* v1){
	if (arb_program){arb_program->SetVector3I(name, v1);}
}

void Shader::SetVector4I(string name, int* v1){
	if (arb_program){arb_program->SetVector4I(name, v1);}
}

void Shader::SetParameter1F(string name, float v1){
	//if (arb_program){arb_program->SetParameter1F(name, v1);}
	ShaderData data;
	data.name=name;
	data.type=0;
	data.valuef[0]=v1;
	Parameters.push_back(data);
}

void Shader::SetParameter2F(string name, float v1, float v2){
	if (arb_program){arb_program->SetParameter2F(name, v1, v2);}
}

void Shader::SetParameter3F(string name, float v1, float v2, float v3){
	if (arb_program){arb_program->SetParameter3F(name, v1, v2, v3);}
}

void Shader::SetParameter4F(string name, float v1, float v2, float v3, float v4){
	if (arb_program){arb_program->SetParameter4F(name, v1, v2, v3, v4);}
}

void Shader::SetVector1F(string name, float* v1){
	if (arb_program){arb_program->SetVector1F(name, v1);}
}

void Shader::SetVector2F(string name, float* v1){
	if (arb_program){arb_program->SetVector2F(name, v1);}
}

void Shader::SetVector3F(string name, float* v1){
	if (arb_program){arb_program->SetVector3F(name, v1);}
}

void Shader::SetVector4F(string name, float* v1){
	if (arb_program){arb_program->SetVector4F(name, v1);}
}

void Shader::SetMatrix2F(string name, float* m){
	//if (arb_program){arb_program->SetMatrix2F(name, m);}
	ShaderData data;
	data.name=name;
	data.type=11;
	data.pf=m;
	Parameters.push_back(data);

}

void Shader::SetMatrix3F(string name, float* m){
	if (arb_program){arb_program->SetMatrix3F(name, m);}
}

void Shader::SetMatrix4F(string name, float* m){
	if (arb_program){arb_program->SetMatrix4F(name, m);}
}

void Shader::SetParameter1D(string name, double v1){
	if (arb_program){arb_program->SetParameter1D(name, v1);}
}

void Shader::SetParameter2D(string name, double v1, double v2){
	if (arb_program){arb_program->SetParameter2D(name, v1, v2);}
}

void Shader::SetParameter3D(string name, double v1, double v2, double v3){
	if (arb_program){arb_program->SetParameter3D(name, v1, v2, v3);}
}

void Shader::SetParameter4D(string name, double v1, double v2, double v3, double v4){
	if (arb_program){arb_program->SetParameter4D(name, v1, v2, v3, v4);}
}*/

//ShaderObject
// ==========================================================================================================


/*------------------------------------------------------
/Dump a list of Shaders attached to this tProgramObject
/------------------------------------------------------
Method ListAttachedShaders()
	Print "Vertex Shader(s) attached to ProgramObject '"+Self.progName+"'"
	Print "----------------------------------------------------------------------"
	If vList.Count() = 0
		Print "No Vertex Shaders attached"
	Else
		For Local v:tShaderObject = EachIn vList
			Print v.shaderName
		Next
	End If
		
	Print
	Print "Fragment Shader(s) attached to ProgramObject '"+Self.progName+"'"
	Print "-----------------------------------------------------------------------"
	If fList.Count() = 0
		Print "No Fragment Shaders attached"
	Else
		For Local f:tShaderObject = EachIn fList
			Print f.shaderName
		Next
	End If
	Print
End Method*/

void CopyPixels (unsigned char *src, unsigned int srcWidth, unsigned int srcHeight, unsigned int srcX, unsigned int srcY, unsigned char *dst, unsigned int dstWidth, unsigned int dstHeight, unsigned int bytesPerPixel);


Material* Material::LoadMaterial(string filename,int flags, int frame_width,int frame_height,int first_frame,int frame_count){

	filename=File::ResourceFilePath(filename);

	/*if (filename==""){
		cout << "Error: Cannot Find Texture: " << filename << endl;
		return NULL;
	}*/

	Material* tex=new Material();
	tex->file_name=filename;

	// set tex.flags before TexInList
	tex->flags=flags;
	tex->FilterFlags();

	/*// check to see if texture with same properties exists already, if so return existing texture
	Texture* old_tex=tex->TexInList();
	if(old_tex){
		return old_tex;
	}else{
		tex_list.push_back(tex);
	}*/

	//const char* c_filename_left=filename_left.c_str();
	//const char* c_filename_right=filename_right.c_str();


	unsigned char* buffer;
	//buffer=stbi_load(filename.c_str(),&tex->width,&tex->height,0,4);
	buffer=Texture::loadpixbuf(filename.c_str(),&tex->width,&tex->height);

	unsigned int name;

	glGenTextures(1,&name);
	glBindTexture(GL_TEXTURE_3D,name);

	tex->no_frames=frame_count;
	//tex->frames=new unsigned int[frame_count];

	unsigned char* dstbuffer=new unsigned char[frame_width*frame_height*4*frame_count];

	glGenTextures(1,&name);
	glBindTexture(GL_TEXTURE_3D,name);


	//tex.gltex=tex.gltex[..tex.no_frames]

	int frames_in_row=tex->width/frame_width;

	for (int i=0;i<frame_count;i++){
		CopyPixels (buffer,tex->width, tex->height,frame_width*(i%frames_in_row), frame_height*(i/frames_in_row),
		dstbuffer+i*(frame_width * frame_height * 4), frame_width, frame_height, 4);


	}

	glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA,frame_width, frame_height, frame_count, 0, GL_RGBA, GL_UNSIGNED_BYTE, dstbuffer);

	tex->texture=name;
	tex->width=frame_width;
	tex->height=frame_height;
	delete dstbuffer;
	//stbi_image_free(buffer);
	Texture::freepixbuf(buffer);

	return tex;

}

