/*
 *  global.h
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

/*
GL_ARB_shader_objects
GL_ARB_shading_language_100

and one of these at least
GL_ARB_vertex_shader
GL_ARB_fragment_shader
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include "pivot.h"
#include "mesh.h"
#include "string_helper.h"

class Camera;

class Global{
public:
	struct GL_Extension{
		string name;
		size_t hash;

		GL_Extension(const string& other=""):name(other),hash(StringHash(other,false)){}
		bool operator==(const string& other) const{return (name==other);}
		bool operator==(size_t other) const{return (hash==other);}
	};

	static int width,height,mode,depth,rate;
	static float ambient_red,ambient_green,ambient_blue;
	static Shader* ambient_shader;

	static int vbo_enabled;
	static int vbo_min_tris;

	static bool gl_arb_texture_non_power_of_two;

	static bool gl_sgis_generate_mipmap;
	static bool gl_ext_framebuffer_object;
	static bool glu_build_mipmaps;

	static bool gl_arb_shader_ojects;
	static bool gl_arb_shading_language_100;
	static bool gl_arb_vertex_shader;
	static bool gl_arb_fragment_shader;

	static double gl_version;
	static double gl_shader_version;

	static vector<GL_Extension> gl_extensions;

	static int Shadows_enabled;
	static float anim_speed;
	static int fog_enabled; // used to keep track of whether fog is enabled between camera update and mesh render
	static Pivot root_ent_f;
	static Pivot* root_ent;
	static Camera* camera_in_use;

	static void Graphics();

	static void AmbientLight(float r,float g,float b);
	static void ClearCollisions();
	static void Collisions(int src_no,int dest_no,int method_no,int response_no=0);
	static void ClearWorld(int entities=true,int brushes=true,int textures=true);
	static void UpdateWorld(float anim_speed=1.0);
	static void RenderWorld();
	static void UpdateEntityAnim(Mesh& mesh);

	static bool CheckExtension(const string& extension);

	static int alpha_enable;
	static int blend_mode;
	static int fx1;
	static int fx2;
};

bool CompareEntityOrder(Entity* ent1,Entity* ent2);

#endif
