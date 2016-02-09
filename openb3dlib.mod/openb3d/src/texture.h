#ifndef TEXTURE_H
#define TEXTURE_H

/*
 *  texture.h
 *  iminib3d
 *
 *  Created by Simon Harrison.
 *  Copyright Si Design. All rights reserved.
 *
 */

//#import "Texture2D.h"

#include <list>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "refobject.h"
#include "objectalloc.h"

using namespace std;

class Camera;

#if defined(_MSC_VER)
#	define MAX_TEXTURE_COUNT 1024
#endif

// New implementation of custom allocators (not required)
//	This is implemetation is twofold.
//	One: 
//	All objects are kept on the stack. 
//	This limits the number of Textures that can be created but removes the need for allocations/deallocations
//  Two:
//	Debugging. Using this I can see how many objects that are still allocated on application exit
//	Set ModuleInfo "CCFLAGS: -DMAX_TEXTURE_COUNT=[value] in openb3dlib/openb3dlib.bmx to enable this

typedef shared_ptr<unsigned int> tex_name_t;
typedef shared_ptr<unsigned int> tex_frames_t;

class Texture : public ReferencedObject{
public:
	CLASS_ALLOCTOR_DECL(Texture)

	typedef unsigned char *(*LoadPixbuf)(const char *filename,int *width,int *height);
	typedef void (*FreePixbuf)(unsigned char *buf);

	static LoadPixbuf loadpixbuf; // Remove
	static FreePixbuf freepixbuf; // Remove
	static list<Texture*> tex_list;
	static list<unsigned int> name_list;

	unsigned int texture;
	tex_name_t texture_ref; // This is used to ensure that the opengl texture isn't released/deleted before
							// all references are gone. This also enable us to share the underlying opengl texture
							// across classes.

	string file_name;
	string file_abs;
	size_t file_hash;

	unsigned int* frames;
	tex_frames_t frames_ref;  // This is used to ensure that the texture frames isn't released/deleted before
							  // all references are gone
	int flags;
	int blend;
	int coords;

	int format; // Added can be GL_RGBA or one of the DXT compressed ones

	float u_scale;
	float v_scale;
	
	float u_pos;
	float v_pos;
	float angle;

	int width;
	int height; // returned by Name/Width/Height commands

	int no_frames;
	unsigned int* framebuffer;

	int cube_face;
	int cube_mode;
	
	int texenv_count;
	int texenv[3][12];

	static Texture* LoadTexture(string filename,int flags=0);
	static Texture* LoadAnimTexture(string filename,int flags=0, int frame_width=0,int frame_height=0,int first_frame=0,int frame_count=1);
	static Texture* CreateTexture(int width=256,int height=256,int flags=3,int frames=0,string fname="noname");

	Texture();
	~Texture();

	void Bind();

	void FreeTexture();
	void DrawTexture(int x,int y);
	void TextureBlend(int blend_no);
	void TextureCoords(int coords_no);
	void ScaleTexture(float u_s,float v_s);
	void PositionTexture(float u_p,float v_p);
	void RotateTexture(float ang);
	void BufferToTex(unsigned char* buffer, int frames=0);
	void TexToBuffer(unsigned char* buffer, int frames=0);
	void BackBufferToTex(int frames=0);
	void CameraToTex(Camera* cam, int frames=0);
	void DepthBufferToTex(Camera* cam);
	string TextureName();
	void SetTextureName(string name);

	static void ClearTextureFilters();
	static void AddTextureFilter(string text_match,int flags);

	static Texture* TexInList(size_t hash,int flags);
	static Texture* TexInList(size_t hash,int flags,int blend,int coords,float u_pos,float v_pos,float u_scale,float v_scale,float angle);
	Texture* TexInList();
	static void FilterFlags(const string& filename,int& flags);
	void FilterFlags();
	//static string Strip(string filename);
};

#endif
