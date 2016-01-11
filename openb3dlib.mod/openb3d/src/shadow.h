/*
 *  shadow.h
 *  minib3d
 *
 *
 */

// I suspect we probably have possible memory leaks here as well
// I will have to analyze more.

#ifndef SHADOW_H
#define SHADOW_H

#include "entity.h"
#include "camera.h"
#include "mesh.h"
#include "surface.h"


class ShadowTriangle{
public:
	int surf;
	int tris;
	float v1x, v1y, v1z;
	float v2x, v2y, v2z;
	float v3x, v3y, v3z;
	float tf_v1x, tf_v1y, tf_v1z;
	float tf_v2x, tf_v2y, tf_v2z;
	float tf_v3x, tf_v3y, tf_v3z;
	float nx, ny, nz;
	int cull;
	int ta, tb, tc;
	ShadowTriangle* id_ta;
	ShadowTriangle* id_tb;
	ShadowTriangle* id_tc;
};

class Edge{
	Surface* surf;
	float data[7];
	
	Edge* Copy(){
		Edge* e=new Edge;
		e->surf=surf; // Need to be reference counted?
		memcpy(e->data,data,sizeof(data));
		return e;
	}
};

class ShadowObject{
public:
	static list<ShadowObject*> shadow_list;

	// FIXME: 
	//	I really don't like mixed coding conventions. It's a "vice" I have
	//	So lowercase these suckers at the earliest convenience
	Mesh* parent; // Chaged to lowercase
	int cnt_tris;

	vector<ShadowTriangle*> shadow_triangles; // Changed name from Tri to shadow_triangles

	Mesh* ShadowMesh;

	Surface* ShadowVolume;
	Surface* ShadowCap;

	bool render;			// Changed from char to bool and lowercased
	bool static_object;		// Changed from char to bool and lowercased
	bool volume_created;	// Changed to bool and renamed to volume_created

	static float VolumeLength;
	
	static bool top_caps; // Changed from char to bool
	static int parallel;

	static float light_x;
	static float light_y;
	static float light_z;

	static int midStencilVal;

	static float ShadowRed;
	static float ShadowGreen;
	static float ShadowBlue;
	static float ShadowAlpha;

	/*static int Time;
	static int Frame;*/
	static int RenderedVolumes;

	void FreeShadow();
	static ShadowObject* Create(Mesh* parent, bool static_object = false); // lowercased
	static void SetShadowColor(float red=0,float green=0,float blue=0,float alpha=0.5f); // Changed from int to float
	static void ShadowInit();
	void RemoveShadowfromMesh(Mesh* mesh);
	static void Update(Camera* cam);
	static void RenderVolume();
	void UpdateAnim();
	void Init();
	void InitShadow();
	void UpdateCaster();
	static void ShadowRenderWorldZFail();
};

#endif

