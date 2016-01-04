#include "texture.h"
#include "entity.h"
#include "mesh.h"
#include "global.h"
#include "camera.h"
#include "pick.h"
#include "light.h"
#include "shadow.h"
#include "stencil.h"
#include "csg.h"
#include "voxel.h"
#include "octree.h"
#include "geosphere.h"
#include "isosurface.h"
#include "particle.h"
#include "physics.h"
#include "actions.h"

#include <Windows.h>

extern "C" { 
	bool gl_alpha_test;
	bool gl_blend;
	bool gl_color_material;
	bool gl_cull_face;
	bool gl_depth_test;
	bool gl_fog;
	bool gl_lighting;
	bool gl_normalize;
	bool gl_point_smooth;
	bool gl_point_sprite;
	bool gl_polygon_offset_fill;
	bool gl_scissor_test;
	bool gl_stencil_test;
	bool gl_texture_2d;
	bool gl_texture_3d;
	bool gl_texture_cube_map;
	bool gl_texture_gen_q;
	bool gl_texture_gen_r;
	bool gl_texture_gen_s;
	bool gl_texture_gen_t;
	bool gl_color_array;
	bool gl_normal_array;
	bool gl_texture_coord_array;
	bool gl_vertex_array;

	void GL_Enable( GLenum type )
	{
		switch( type )
		{
		case GL_ALPHA_TEST:
			if ( !gl_alpha_test )
			{
				glEnable( GL_ALPHA_TEST );
				gl_alpha_test = true;
				return;
			}
			break;	
		case GL_BLEND:
			if ( !gl_blend )
			{
				glEnable( GL_BLEND );
				gl_blend = true;
				return;
			}
			break;
		case GL_COLOR_MATERIAL:
			if ( !gl_color_material )
			{
				glEnable( GL_COLOR_MATERIAL );
				gl_color_material = true;
				return;
			}
			break;
		case GL_CULL_FACE:
			if ( !gl_cull_face )
			{
				glEnable( GL_CULL_FACE );
				gl_cull_face = true;
				return;
			}
			break;
		case GL_DEPTH_TEST:
			if ( !gl_depth_test )
			{
				glEnable( GL_DEPTH_TEST );
				gl_depth_test = true;
				return;
			}
			break;
		case GL_FOG:
			if ( !gl_fog )
			{
				glEnable( GL_FOG );
				gl_fog = true;
				return;
			}
			break;
		case GL_LIGHTING:
			if ( !gl_lighting )
			{
				glEnable( GL_LIGHTING );
				gl_lighting = true;
				return;
			}
			break;
		case GL_NORMALIZE:
			if ( !gl_normalize )
			{
				glEnable( GL_NORMALIZE );
				gl_normalize = true;
				return;
			}
			break;
		case GL_POINT_SMOOTH:
			if ( !gl_point_smooth )
			{
				glEnable( GL_POINT_SMOOTH );
				gl_point_smooth = true;
				return;
			}
			break;
		case GL_POINT_SPRITE:
			if ( !gl_point_sprite )
			{
				glEnable( GL_POINT_SPRITE );
				gl_point_sprite = true;
				return;
			}
			break;
		case GL_POLYGON_OFFSET_FILL:
			if ( !gl_polygon_offset_fill )
			{
				glEnable( GL_POLYGON_OFFSET_FILL );
				gl_polygon_offset_fill = true;
				return;
			}
			break;
		case GL_SCISSOR_TEST:
			if ( !gl_scissor_test )
			{
				glEnable( GL_SCISSOR_TEST );
				gl_scissor_test = true;
				return;
			}
			break;
		case GL_STENCIL_TEST:
			if ( !gl_stencil_test )
			{
				glEnable( GL_STENCIL_TEST );
				gl_stencil_test = true;
				return;
			}
			break;
		case GL_TEXTURE_2D:
			if ( !gl_texture_2d )
			{
				glEnable( GL_TEXTURE_2D );
				gl_texture_2d = true;
				return;
			}
			break;
		case GL_TEXTURE_3D:
			if ( !gl_texture_3d )
			{
				glEnable( GL_TEXTURE_3D );
				gl_texture_3d = true;
				return;
			}
			break;
		case GL_TEXTURE_CUBE_MAP:
			if ( !gl_texture_cube_map )
			{
				glEnable( GL_TEXTURE_CUBE_MAP );
				gl_texture_cube_map = true;
				return;
			}
			break;
		case GL_TEXTURE_GEN_Q:
			if ( !gl_texture_gen_q )
			{
				glEnable( GL_TEXTURE_GEN_Q );
				gl_texture_gen_q = true;
				return;
			}
			break;
		case GL_TEXTURE_GEN_R:
			if ( !gl_texture_gen_r )
			{
				glEnable( GL_TEXTURE_GEN_R );
				gl_texture_gen_r = true;
				return;
			}
			break;
		case GL_TEXTURE_GEN_S:
			if ( !gl_texture_gen_s )
			{
				glEnable( GL_TEXTURE_GEN_S );
				gl_texture_gen_s = true;
				return;
			}
			break;
		case GL_TEXTURE_GEN_T:
			if ( !gl_texture_gen_t )
			{
				glEnable( GL_TEXTURE_GEN_T );
				gl_texture_gen_t = true;
				return;
			}
			break;
		case GL_COLOR_ARRAY:
			if ( !gl_color_array )
			{
				glEnableClientState( GL_COLOR_ARRAY );
				gl_color_array = true;
				return;
			}
			break;
		case GL_NORMAL_ARRAY:
			if ( !gl_normal_array )
			{
				glEnableClientState( GL_NORMAL_ARRAY );
				gl_normal_array = true;
				return;
			}
			break;
		case GL_TEXTURE_COORD_ARRAY:
			if ( !gl_texture_coord_array )
			{
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				gl_texture_coord_array = true;
				return;
			}
			break;
		case GL_VERTEX_ARRAY:
			if ( !gl_vertex_array )
			{
				glEnableClientState( GL_VERTEX_ARRAY );
				gl_vertex_array = true;
				return;
			}
			break;
		}
	
		glEnable( type );
	}

	void GL_Disable( GLenum type )
	{
		switch( type )
		{
		case GL_ALPHA_TEST:
			if ( gl_alpha_test )
			{
				glDisable( GL_ALPHA_TEST );
				gl_alpha_test = false;
				return;
			}
			break;	
		case GL_BLEND:
			if ( gl_blend )
			{
				glDisable( GL_BLEND );
				gl_blend = false;
				return;
			}
			break;
		case GL_COLOR_MATERIAL:
			if ( gl_color_material )
			{
				glDisable( GL_COLOR_MATERIAL );
				gl_color_material = false;
				return;
			}
			break;
		case GL_CULL_FACE:
			if ( gl_cull_face )
			{
				glDisable( GL_CULL_FACE );
				gl_cull_face = false;
				return;
			}
			break;
		case GL_DEPTH_TEST:
			if ( gl_depth_test )
			{
				glDisable( GL_DEPTH_TEST );
				gl_depth_test = false;
				return;
			}
			break;
		case GL_FOG:
			if ( gl_fog )
			{
				glDisable( GL_FOG );
				gl_fog = false;
				return;
			}
			break;
		case GL_LIGHTING:
			if ( gl_lighting )
			{
				glDisable( GL_LIGHTING );
				gl_lighting = false;
				return;
			}
			break;
		case GL_NORMALIZE:
			if ( gl_normalize )
			{
				glDisable( GL_NORMALIZE );
				gl_normalize = false;
				return;
			}
			break;
		case GL_POINT_SMOOTH:
			if ( gl_point_smooth )
			{
				glDisable( GL_POINT_SMOOTH );
				gl_point_smooth = false;
				return;
			}
			break;
		case GL_POINT_SPRITE:
			if ( gl_point_sprite )
			{
				glDisable( GL_POINT_SPRITE );
				gl_point_sprite = false;
				return;
			}
			break;
		case GL_POLYGON_OFFSET_FILL:
			if ( gl_polygon_offset_fill )
			{
				glDisable( GL_POLYGON_OFFSET_FILL );
				gl_polygon_offset_fill = false;
				return;
			}
			break;
		case GL_SCISSOR_TEST:
			if ( gl_scissor_test )
			{
				glDisable( GL_SCISSOR_TEST );
				gl_scissor_test = false;
				return;
			}
			break;
		case GL_STENCIL_TEST:
			if ( gl_stencil_test )
			{
				glDisable( GL_STENCIL_TEST );
				gl_stencil_test = false;
				return;
			}
			break;
		case GL_TEXTURE_2D:
			if ( gl_texture_2d )
			{
				glDisable( GL_TEXTURE_2D );
				gl_texture_2d = false;
				return;
			}
			break;
		case GL_TEXTURE_3D:
			if ( gl_texture_3d )
			{
				glDisable( GL_TEXTURE_3D );
				gl_texture_3d = false;
				return;
			}
			break;
		case GL_TEXTURE_CUBE_MAP:
			if ( gl_texture_cube_map )
			{
				glDisable( GL_TEXTURE_CUBE_MAP );
				gl_texture_cube_map = false;
				return;
			}
			break;
		case GL_TEXTURE_GEN_Q:
			if ( gl_texture_gen_q )
			{
				glDisable( GL_TEXTURE_GEN_Q );
				gl_texture_gen_q = false;
				return;
			}
			break;
		case GL_TEXTURE_GEN_R:
			if ( gl_texture_gen_r )
			{
				glDisable( GL_TEXTURE_GEN_R );
				gl_texture_gen_r = false;
				return;
			}
			break;
		case GL_TEXTURE_GEN_S:
			if ( gl_texture_gen_s )
			{
				glDisable( GL_TEXTURE_GEN_S );
				gl_texture_gen_s = false;
				return;
			}
			break;
		case GL_TEXTURE_GEN_T:
			if ( gl_texture_gen_t )
			{
				glDisable( GL_TEXTURE_GEN_T );
				gl_texture_gen_t = false;
				return;
			}
			break;
		case GL_COLOR_ARRAY:
			if ( gl_color_array )
			{
				glDisableClientState( GL_COLOR_ARRAY );
				gl_color_array = false;
				return;
			}
			break;
		case GL_NORMAL_ARRAY:
			if ( gl_normal_array )
			{
				glDisableClientState( GL_NORMAL_ARRAY );
				gl_normal_array = false;
				return;
			}
			break;
		case GL_TEXTURE_COORD_ARRAY:
			if ( gl_texture_coord_array )
			{
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
				gl_texture_coord_array = false;
				return;
			}
			break;
		case GL_VERTEX_ARRAY:
			if ( gl_vertex_array )
			{
				glDisableClientState( GL_VERTEX_ARRAY );
				gl_vertex_array = false;
				return;
			}
			break;
		}
	
		glDisable( type );
	}

	Texture *CreateTextureEx( GLuint id, int width, int height, int flags, int frames, string file )
	{
		Texture *tex = new Texture;

		tex->flags = flags;
		tex->FilterFlags();
		tex->file_name = file;
		tex->width = width;
		tex->height = height;
		tex->texture = id;

		if (flags&128)
		{
			tex->no_frames = 1;
			tex->frames = new unsigned int[6];
		}
		else
		{
			tex->no_frames = frames;
			if ( tex->no_frames > 1 )
				tex->frames = new unsigned int[tex->no_frames];
		}

		return tex;
	}
}
