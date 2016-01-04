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

using namespace std;

class Camera;

class Texture{
public:

	typedef unsigned char *(*LoadPixbuf)(const char *filename,int *width,int *height);
	typedef void (*FreePixbuf)(unsigned char *buf);

	static LoadPixbuf loadpixbuf;
	static FreePixbuf freepixbuf;
	static list<Texture*> tex_list;

	unsigned int texture;

	string file_name;
	string file_abs;
	size_t file_hash;

	unsigned int* frames;

	int flags;
	int blend;
	int coords;

	int format; // Added can be GL_RGBA8 or one of the DXT compressed ones

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
	
	int glTexEnv_count;//
	int glTexEnv[3][12];//

	Texture();

	static Texture* LoadTexture(string filename,int flags=0);
	static Texture* LoadAnimTexture(string filename,int flags=0, int frame_width=0,int frame_height=0,int first_frame=0,int frame_count=1);
	static Texture* CreateTexture(int width=256,int height=256,int flags=3, int frames=0,string fname="noname");

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
	Texture* TexInList();
	void FilterFlags();
	//static string Strip(string filename);
};

#endif
