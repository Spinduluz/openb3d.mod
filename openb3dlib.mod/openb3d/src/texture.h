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

using namespace std;

class Camera;

typedef shared_ptr<unsigned int> GLTextureName;
typedef shared_ptr<unsigned int> TextureFrames;

class Texture : public ReferencedObject{
public:
	typedef unsigned char *(*LoadPixbuf)(const char *filename,int *width,int *height);
	typedef void (*FreePixbuf)(unsigned char *buf);

	static LoadPixbuf loadpixbuf;
	static FreePixbuf freepixbuf;
	static list<Texture*> tex_list;

	unsigned int texture;
	GLTextureName texture_ref; // This is used to ensure that the opengl texture isn't released/deleted before
							   // all references are gone

	string file_name;
	string file_abs;
	size_t file_hash;

	unsigned int* frames;
	TextureFrames frames_ref;  // This is used to ensure that the texture frames isn't released/deleted before
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

	Texture();
	~Texture();

	static Texture* LoadTexture(string filename,int flags=0);
	static Texture* LoadAnimTexture(string filename,int flags=0, int frame_width=0,int frame_height=0,int first_frame=0,int frame_count=1);
	static Texture* CreateTexture(int width=256,int height=256,int flags=3, int frames=0,string fname="noname");

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
