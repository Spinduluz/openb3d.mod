/*
 *  global.mm
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

#include "global.h"

#include "entity.h"
#include "camera.h"
#include "mesh.h"
#include "sprite.h"
#include "animation.h"
#include "pick.h"
#include "collision2.h"
#include "shadow.h"
#include "particle.h"
#include "physics.h"
#include "actions.h"

#include <list>
#include <stdlib.h>

#if defined(BLITZMAX_DEBUG)
#include "bmaxdebug.h"
#endif

using namespace std;

float Global::ambient_red=0.5;
float Global::ambient_green=0.5;
float Global::ambient_blue=0.5;

Shader* Global::ambient_shader=NULL;

int Global::vbo_enabled=true;
int Global::vbo_min_tris=false;

bool Global::gl_arb_texture_non_power_of_two=false;

bool Global::gl_sgis_generate_mipmap=false;
bool Global::gl_ext_framebuffer_object=false;
bool Global::glu_build_mipmaps=false;

bool Global::gl_arb_shader_ojects=false;
bool Global::gl_arb_shading_language_100=false;
bool Global::gl_arb_vertex_shader=false;
bool Global::gl_arb_fragment_shader=false;

double Global::gl_version=0.0;
double Global::gl_shader_version=0.0;

vector<Global::GL_Extension> Global::gl_extensions;

float Global::anim_speed=1.0;
int Global::fog_enabled=false;
int Global::width=640;
int Global::height=480;
int Global::Shadows_enabled=false;
int Global::alpha_enable=-1;
int Global::blend_mode=-1;
int Global::fx1=-1;
int Global::fx2=-1;

Pivot Global::root_ent_f;
Pivot* Global::root_ent=&root_ent_f;
//Pivot* Global::root_ent=new Pivot(); // Ever released? Probably not
Camera* Global::camera_in_use;

void Global::Graphics(){
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glAlphaFunc(GL_GEQUAL,0.5);
	//glAlphaFunc(GL_NOTEQUAL,0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//glDisable(GL_BLEND);

	glEnable(GL_FOG);
	glEnable(GL_CULL_FACE);
	glEnable(GL_SCISSOR_TEST);

	glEnable(GL_NORMALIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	float amb[]={0.5,0.5,0.5,1.0};

	float flag[]={0.0};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,flag); // 0 for one sided, 1 for two sided

	Texture::AddTextureFilter("",9);

	const char *version=(const char*)glGetString(GL_VERSION);
	gl_version=atof(version);

	version=(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	gl_shader_version=atof(version);

	string ext=(const char*)glGetString(GL_EXTENSIONS);
	size_t e=ext.find_first_of(' ',0);
	size_t s=0;

	if(e!=string::npos){
		gl_extensions.push_back(ext.substr(s,e));
		s=e+1;
		while((e=ext.find_first_of(' ',e+1))!=string::npos){
			gl_extensions.push_back(ext.substr(s,e-s));
			s=e+1;
		}
	}

#if 0
	FilePtr f=File::WriteFile("GL_EXTENSIONS.TXT");
	for(GL_Extension& o : gl_extensions){
		stringstream ss;
		ss << o.name << " " << o.hash;
		f->WriteLine(ss.str());
	}
	f.reset();
#endif

	if(gl_version<1.5) Global::vbo_enabled=false;
	// Check for shader support
	if(gl_version<2.0){
		gl_arb_shader_ojects=GLEW_ARB_shader_objects;
		gl_arb_shading_language_100=GLEW_ARB_shading_language_100;
		gl_arb_vertex_shader=GLEW_ARB_vertex_shader;
		gl_arb_fragment_shader=GLEW_ARB_fragment_shader;
	}

	if(GLEW_SGIS_generate_mipmap){
		gl_sgis_generate_mipmap=true;
		glu_build_mipmaps=false;
	}
	if(GLEW_EXT_framebuffer_object){
		gl_ext_framebuffer_object=true;
		gl_sgis_generate_mipmap=false;
		glu_build_mipmaps=false;
	}
	if(!gl_ext_framebuffer_object && !gl_sgis_generate_mipmap) glu_build_mipmaps=true;

	if(GLEW_ARB_texture_non_power_of_two) gl_arb_texture_non_power_of_two=true;
}

void Global::AmbientLight(float r,float g,float b){
	ambient_red  =r/255.0;
	ambient_green=g/255.0;
	ambient_blue =b/255.0;
}

void Global::ClearCollisions(){
	list<CollisionPair*>::iterator it;
	for(it=CollisionPair::cp_list.begin();it!=CollisionPair::cp_list.end();it++){
		CollisionPair* cp=*it;
		delete cp;
	}
	CollisionPair::cp_list.clear();
}

void Global::Collisions(int src_no,int dest_no,int method_no,int response_no){

	CollisionPair* cp=new CollisionPair;
	cp->src_type=src_no;
	cp->des_type=dest_no;
	cp->col_method=method_no;
	cp->response=response_no;

	// check to see if same collision pair already exists
	list<CollisionPair*>::iterator it;

	for(it=CollisionPair::cp_list.begin();it!=CollisionPair::cp_list.end();it++){
		CollisionPair* cp2=*it;
		if(cp2->src_type==cp->src_type){
			if(cp2->des_type==cp->des_type){
				// overwrite old method and response values
				cp2->col_method=cp->col_method;
				cp2->response=cp->response;
   			return;
			}
		}
	}

	CollisionPair::cp_list.push_back(cp);

}

void Global::ClearWorld(int entities,int brushes,int textures){

	if(entities){
		//list<Entity*>::iterator it;
		//for(it=Entity::entity_list.begin();it!=Entity::entity_list.end();it++){
		//	Entity* ent=*it;
		//	ent->FreeEntity();
		//}
		// Uh oh... is anything freed at all?
		Global::root_ent->FreeEntity();
		Entity::entity_list.clear();
		Entity::animate_list.clear();
		Camera::cam_list.clear();
		ClearCollisions();
		Pick::ent_list.clear();
	}

	if(textures){
		/*list<Texture*>::iterator it;
		for(it=Texture::tex_list.begin();it!=Texture::tex_list.end();it++){
			Texture* tex=*it;
			tex->FreeTexture();
			it=Texture::tex_list.begin();
			it--;
		}*/
		// Not necessary to free texture by texture when using shared/unique pointers.
		// They will delete themselves when they go out of scope. i.e. deleted
		for(Texture *tex : Texture::tex_list){
			while(tex->DestroyRef());
		}
	}
}

void Global::UpdateWorld(float anim_speed){
	Global::anim_speed=anim_speed;

	// particles
	list<ParticleEmitter*>::iterator it2;

	//for(it2=ParticleEmitter::emitter_list.begin();it2!=ParticleEmitter::emitter_list.end();it2++){
	//	ParticleEmitter* emitter=*it2;
	for(ParticleEmitter* emitter : ParticleEmitter::emitter_list){
		emitter->Update();
	}

	// Actions
	Action::Update();

	// anim
	list<Entity*>::iterator it;

	for(it=Entity::animate_list.begin();it!=Entity::animate_list.end();it++){
		Mesh* mesh=dynamic_cast<Mesh*>(*it);
		UpdateEntityAnim(*mesh);
	}

	//Physics
	Constraint::Update();
	RigidBody::Update();

	// collision
	UpdateCollisions();


}

void Global::RenderWorld(){
	Camera::cam_list.sort(CompareEntityOrder); // sort cam list based on entity order
#if 0
	list<Camera*>::iterator it;
	for(it=Camera::cam_list.begin();it!=Camera::cam_list.end();it++){
		//Camera* cam=*it;
		camera_in_use=*it;
		if(camera_in_use->Hidden()==true) continue;
		camera_in_use->Render();
	}

	if (Shadows_enabled==true){
		for(it=Camera::cam_list.begin();it!=Camera::cam_list.end();it++){
			//Camera* cam=*it;
			camera_in_use=*it;
			if(camera_in_use->Hidden()==true) continue;
			ShadowObject::Update(camera_in_use);
		}
	}
#else
	for(Camera* cam : Camera::cam_list){
		camera_in_use=cam;
		if(camera_in_use->Hidden()) continue;
		camera_in_use->Render();
	}

	if(Shadows_enabled){
		for(Camera* cam : Camera::cam_list){
			camera_in_use=cam;
			if(camera_in_use->Hidden()) continue;
			ShadowObject::Update(camera_in_use);
		}
	}
#endif
}

void Global::UpdateEntityAnim(Mesh& mesh){
	if (mesh.anim && mesh.anim_update==true) {
		int first=mesh.anim_seqs_first[mesh.anim_seq];
		int last=mesh.anim_seqs_last[mesh.anim_seq];
		int anim_start=false;

		if(mesh.anim_trans>0){
			mesh.anim_trans=mesh.anim_trans-1;
			if(mesh.anim_trans==1) anim_start=true;
		}

		if(mesh.anim_trans>0){

			float r=1.0-mesh.anim_time;
			r=r/mesh.anim_trans;
			mesh.anim_time=mesh.anim_time+r;

			Animation::AnimateMesh2(&mesh,mesh.anim_time,first,last);
			if(anim_start) mesh.anim_time=first;

		}else{
			if(mesh.anim_mode==4){	//Manual mode
				Animation::AnimateMesh3(&mesh);
				return;
			}

			Animation::AnimateMesh(&mesh,mesh.anim_time,first,last);

			if(mesh.anim_mode==0) mesh.anim_update=false; // after updating animation so that animation is in final 'stop' pose - don't update again
			if(mesh.anim_mode==1){
				// +=
				mesh.anim_time=mesh.anim_time+(mesh.anim_speed*anim_speed);
				if(mesh.anim_time>last){
					mesh.anim_time=first+(mesh.anim_time-last);
				}
				return;
			}

			if(mesh.anim_mode==2){
				if(mesh.anim_dir==1){
					mesh.anim_time=mesh.anim_time+(mesh.anim_speed*anim_speed);
					if(mesh.anim_time>last){
						mesh.anim_time=mesh.anim_time-(mesh.anim_speed*anim_speed);
						mesh.anim_dir=-1;
					}
				}

				if(mesh.anim_dir==-1){
					mesh.anim_time=mesh.anim_time-(mesh.anim_speed*anim_speed);
					if(mesh.anim_time<first){
						mesh.anim_time=mesh.anim_time+(mesh.anim_speed*anim_speed);
						mesh.anim_dir=1;
					}
				}
				return;
			}

			if(mesh.anim_mode==3){
				mesh.anim_time=mesh.anim_time+(mesh.anim_speed*anim_speed);
				if(mesh.anim_time>last){
					mesh.anim_time=last;
					mesh.anim_mode=0;
				}
			}
		}
	}
}

bool Global::CheckExtension(const string& extension){
	size_t hash=StringHash(extension,false);
	for(GL_Extension& e : gl_extensions){
		if(e.hash==hash) return true;
	}
	return false;
}

bool CompareEntityOrder(Entity* ent1,Entity* ent2){
	if(ent1->order>ent2->order){
		return true;
	}else{
		return false;
	}
}

// ==========================================================================================================

enum{
	RENDERSTATE_UNKNOWN,
	RENDERSTATE_ENABLED,
	RENDERSTATE_DISABLED,
};

struct GLState{
	GLenum type;
	int state;
};

static GLState gl_state[]={
	{GL_ALPHA_TEST,RENDERSTATE_UNKNOWN},
	{GL_BLEND,RENDERSTATE_UNKNOWN},
	{GL_COLOR_MATERIAL,RENDERSTATE_UNKNOWN},
	{GL_CULL_FACE,RENDERSTATE_UNKNOWN},
	{GL_DEPTH_TEST,RENDERSTATE_UNKNOWN},
	{GL_FOG,RENDERSTATE_UNKNOWN},
	{GL_LIGHTING,RENDERSTATE_UNKNOWN},
	{GL_NORMALIZE,RENDERSTATE_UNKNOWN},
	{GL_POINT_SMOOTH,RENDERSTATE_UNKNOWN},
	{GL_POINT_SPRITE,RENDERSTATE_UNKNOWN},
	{GL_POLYGON_OFFSET_FILL,RENDERSTATE_UNKNOWN},
	{GL_SCISSOR_TEST,RENDERSTATE_UNKNOWN},
	{GL_STENCIL_TEST,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_2D,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_3D,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_CUBE_MAP,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_GEN_Q,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_GEN_R,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_GEN_S,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_GEN_T,RENDERSTATE_UNKNOWN},
	{GL_LIGHT0,RENDERSTATE_UNKNOWN},
	{GL_LIGHT1,RENDERSTATE_UNKNOWN},
	{GL_LIGHT2,RENDERSTATE_UNKNOWN},
	{GL_LIGHT3,RENDERSTATE_UNKNOWN},
	{GL_LIGHT4,RENDERSTATE_UNKNOWN},
	{GL_LIGHT5,RENDERSTATE_UNKNOWN},
	{GL_LIGHT6,RENDERSTATE_UNKNOWN},
	{GL_LIGHT7,RENDERSTATE_UNKNOWN}/*,
	{GL_COLOR_ARRAY,RENDERSTATE_UNKNOWN},
	{GL_NORMAL_ARRAY,RENDERSTATE_UNKNOWN},
	{GL_TEXTURE_COORD_ARRAY,RENDERSTATE_UNKNOWN},
	{GL_VERTEX_ARRAY,RENDERSTATE_UNKNOWN}*/
};

#define STATE_COUNT sizeof(gl_state)/sizeof(GLState)

void GL_Enable(GLenum type){
	for(unsigned int i=0; i<STATE_COUNT; i++){
		if(gl_state[i].type==type){
			// Already enabled. Skip.
			if(gl_state[i].state==RENDERSTATE_ENABLED) return;
			// Set state and skip iteration
			glEnable(type);
			gl_state[i].state=RENDERSTATE_ENABLED;
			return;
		}
	}
	// Not found. Set anyway but log occurance
#if defined(BLITZMAX_DEBUG)
	DebugLog("GL_Enable: Unhandled render state");
#endif
	glEnable(type);
}

void GL_Disable(GLenum type){
	for(unsigned int i=0; i<STATE_COUNT; i++){
		if(gl_state[i].type==type){
			// Already enabled. Skip.
			if(gl_state[i].state==RENDERSTATE_DISABLED) return;
			// Set state and skip iteration
			glDisable(type);
			gl_state[i].state=RENDERSTATE_DISABLED;
			return;
		}
	}
	// Not found. Set anyway but log occurance
#if defined(BLITZMAX_DEBUG)
	DebugLog("GL_Disable: Unhandled render state");
#endif
	glDisable(type);
}

static GLenum gl_error=GL_NO_ERROR;

bool GL_Error(){
	gl_error=glGetError();
	return (gl_error!=GL_NO_ERROR);
}

const char *GL_GetErrorString(GLenum error){
	switch(error){
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	case GL_CONTEXT_LOST:
		return "GL_CONTEXT_LOST";
	case GL_TABLE_TOO_LARGE:
		return "GL_TABLE_TOO_LARGE";

	default:
		return "Unknown error";
	}
}

void GL_TraverseErrors(){
#if defined(BLITZMAX_DEBUG)
	if(gl_error==GL_NO_ERROR) return;

	do{
		DebugLog("glGetError:%s",GL_GetErrorString(gl_error));
	}while((gl_error=glGetError())!=GL_NO_ERROR);
#endif
}

