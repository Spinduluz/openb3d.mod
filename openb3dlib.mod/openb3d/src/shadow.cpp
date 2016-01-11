
#include <GL/glew.h>

/*
#ifdef linux
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#ifdef WIN32
#include <gl\GLee.h>
#endif

#ifdef __APPLE__
#include "GLee.h"
#endif
*/

#include "shadow.h"
#include "light.h"
#include "mesh.h"
#include "global.h"

float ShadowObject::ShadowRed=0;
float ShadowObject::ShadowGreen=0;
float ShadowObject::ShadowBlue=0;
float ShadowObject::ShadowAlpha=.5;

list<ShadowObject*> ShadowObject::shadow_list;

float ShadowObject::light_x;
float ShadowObject::light_y;
float ShadowObject::light_z;

bool ShadowObject::top_caps=true;
float ShadowObject::VolumeLength=1000;
int ShadowObject::parallel;
int ShadowObject::midStencilVal;
//int ShadowObject::RenderedVolumes;
//int ShadowObject::Frame;
//int ShadowObject::Time;

void ShadowObject::FreeShadow() {
	shadow_list.remove(this);
	ShadowMesh->FreeEntity();

	//C++11'ify
	for(ShadowTriangle* t : shadow_triangles) delete t;
	if (!shadow_list.size()) Global::Shadows_enabled=0;

}

void ShadowObject::SetShadowColor(float red,float green,float blue,float alpha){
	ShadowRed=red/255.0;
	ShadowGreen=green/255.0;
	ShadowBlue=blue/255.0;
	ShadowAlpha=alpha/255.0;
}

void ShadowObject::ShadowInit(){
	int StencilBits;
	glGetIntegerv(GL_STENCIL_BITS, &StencilBits); // Move this to Global
	midStencilVal = (StencilBits - 1)^2;
	glClearStencil(midStencilVal);
}

ShadowObject* ShadowObject::Create(Mesh* parent, bool static_object){
		ShadowObject* shadow = new ShadowObject;
		shadow->ShadowMesh=Mesh::CreateMesh();
		Global::root_ent->child_list.remove(shadow->ShadowMesh);
		shadow->ShadowMesh->EntityBlend(0);
		shadow->ShadowMesh->EntityAlpha(0.2);
		shadow->ShadowMesh->EntityColor(255, 0, 0);
		shadow->ShadowMesh->EntityFX(17);
		shadow->ShadowVolume = shadow->ShadowMesh->CreateSurface();
		shadow->parent = parent;
		shadow->static_object = static_object;
		shadow->volume_created=false;
		shadow_list.push_back(shadow);
		shadow->Init();
		return shadow;
}

void ShadowObject::RemoveShadowfromMesh(Mesh* mesh) {
	// C++11'ify
	for(ShadowObject *shadow : shadow_list){
		if(shadow->parent==mesh){
			// We should not free it or something?
			// Or can it be referenced by another mesh?
			// I'm going to take a stab in the dark and remove it
			shadow_list.remove(shadow);
			shadow->FreeShadow();
			return;
		}
	}
}

void ShadowObject::Update(Camera* cam){
	// C++11'ify
	for(Light* light : Light::light_list){
		if(light->hide || !light->cast_shadow) continue;

		light_x=light->EntityX(true) * (1+parallel*1);
		light_y=light->EntityY(true) * (1+parallel*1);
		light_z=light->EntityZ(true) * (1+parallel*1);

		cam->CameraClsMode(false,false);
		cam->Update();

		for(ShadowObject *shadow : shadow_list){
			if (!shadow->parent->hide && light->EntityDistance(shadow->parent)<1/light->range){
				VolumeLength=(cam->range_far-shadow->parent->EntityDistance(cam))/(shadow->parent->EntityDistance(light)+abs(shadow->parent->cull_radius));
				shadow->ShadowMesh->reset_bounds=true;
				shadow->ShadowMesh->GetBounds();
				shadow->UpdateCaster();
				shadow->render=true;
			}else{
				shadow->render=false;
			}
		}

		cam->CameraClsMode(false,false);
		ShadowRenderWorldZFail();
	}
	cam->CameraClsMode(true,true) ;
}

void ShadowObject::RenderVolume(){
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.0,4.0);

	// C++11'ify
	for(ShadowObject* shadow : shadow_list){
		if(shadow->render) shadow->ShadowMesh->UpdateShadow();
	}
	glDisable(GL_POLYGON_OFFSET_FILL);
}

void ShadowObject::UpdateAnim(){
	Surface* surf;// = Parent->GetSurface(1);
	Surface* anim_surf;

	list<Surface*>::iterator surf_it;
	surf_it=parent->surf_list.begin();

	list<Surface*>::iterator an_it;
	an_it=parent->anim_surf_list.begin();

	vector<ShadowTriangle*>::iterator it;

	int cnt;
	int cnt_surf = parent->CountSurfaces();
	it=shadow_triangles.begin();
  // for(it=Tri.begin();it!=Tri.end();it++){

	for (int s=1;s<=cnt_surf;s++){
		surf = *surf_it;
		anim_surf = *an_it;
		cnt = surf->CountTriangles() - 1;

		for (int v = 0;v<=cnt;v++){
			ShadowTriangle* T=*it;
			int vert0 = surf->TriangleVertex(T->tris, 0);
			int vert1 = surf->TriangleVertex(T->tris, 1);
			int vert2 = surf->TriangleVertex(T->tris, 2);
			T->v1x = anim_surf->VertexX(vert0);
			T->v1y = anim_surf->VertexY(vert0);
			T->v1z = anim_surf->VertexZ(vert0);
			T->v2x = anim_surf->VertexX(vert1);
			T->v2y = anim_surf->VertexY(vert1);
			T->v2z = anim_surf->VertexZ(vert1);
			T->v3x = anim_surf->VertexX(vert2);
			T->v3y = anim_surf->VertexY(vert2);
			T->v3z = anim_surf->VertexZ(vert2);
			it++;
		}
		surf_it++;
		an_it++;
	}
}

void ShadowObject::Init(){
	int cnt = -1;
	cnt_tris = - 1;
	int cnt_surf = parent->CountSurfaces();

	for (int s = 1; s<= cnt_surf;s++){
		Surface* surf = parent->GetSurface(s);
		cnt = surf->CountTriangles() - 1;

		for (int v = 0;v<=cnt;v++){
			cnt_tris = cnt_tris + 1;
			/*if (cnt+1 > Tri.size){
					tri = tri[..tri.length + 1]
			}*/
			ShadowTriangle* etet = new ShadowTriangle;
			shadow_triangles.push_back(etet);
			etet->tris = v;
			int vert0 = surf->TriangleVertex(v, 0);
			int vert1 = surf->TriangleVertex(v, 1);
			int vert2 = surf->TriangleVertex(v, 2);
			etet->v1x = surf->VertexX(vert0);
			etet->v1y = surf->VertexY(vert0);
			etet->v1z = surf->VertexZ(vert0);
			etet->v2x = surf->VertexX(vert1);
			etet->v2y = surf->VertexY(vert1);
			etet->v2z = surf->VertexZ(vert1);
			etet->v3x = surf->VertexX(vert2);
			etet->v3y = surf->VertexY(vert2);
			etet->v3z = surf->VertexZ(vert2);
			etet->ta = -1;
			etet->tb = -1;
			etet->tc = -1;
		}
	}
	//int cnt_tris = cnt_tris
	float v1a_x, v1a_y, v1a_z, v1b_x, v1b_y, v1b_z, v1c_x, v1c_y, v1c_z;

	bool v1a_v2a, v1a_v2b, v1a_v2c, v1b_v2a, v1b_v2b, v1b_v2c, v1c_v2a, v1c_v2b, v1c_v2c;

	for (int a = 0;a<= cnt_tris;a++){
		ShadowTriangle* at = shadow_triangles[a];
		v1a_x  = at->v1x;
		v1a_y  = at->v1y;
		v1a_z  = at->v1z;
		v1b_x  = at->v2x;
		v1b_y  = at->v2y;
		v1b_z  = at->v2z;
		v1c_x  = at->v3x;
		v1c_y  = at->v3y;
		v1c_z  = at->v3z;
		for (int b = a + 1;b<= cnt_tris; b++){
			ShadowTriangle* bt = shadow_triangles[b];
			v1a_v2a = (v1a_x == bt->v1x && v1a_y == bt->v1y && v1a_z == bt->v1z);
			v1a_v2b = (v1a_x == bt->v2x && v1a_y == bt->v2y && v1a_z == bt->v2z);
			v1a_v2c = (v1a_x == bt->v3x && v1a_y == bt->v3y && v1a_z == bt->v3z);
			v1b_v2a = (v1b_x == bt->v1x && v1b_y == bt->v1y && v1b_z == bt->v1z);
			v1b_v2b = (v1b_x == bt->v2x && v1b_y == bt->v2y && v1b_z == bt->v2z);
			v1b_v2c = (v1b_x == bt->v3x && v1b_y == bt->v3y && v1b_z == bt->v3z);
			v1c_v2a = (v1c_x == bt->v1x && v1c_y == bt->v1y && v1c_z == bt->v1z);
			v1c_v2b = (v1c_x == bt->v2x && v1c_y == bt->v2y && v1c_z == bt->v2z);
			v1c_v2c = (v1c_x == bt->v3x && v1c_y == bt->v3y && v1c_z == bt->v3z);

			if (bt->ta==-1){
				if (v1a_v2b!=0 && v1b_v2a!=0 && at->ta==-1){
					at->ta = b;
					bt->ta = a;}
				else if (v1b_v2b!=0 && v1c_v2a!=0 && at->tb==-1){
					at->tb = b;
					bt->ta = a;}
				else if (v1c_v2b!=0 && v1a_v2a!=0 && at->ta==-1){
					at->tc = b;
					bt->ta = a;}
			}
			if (bt->tb==-1){
				if (v1a_v2c!=0 && v1b_v2b!=0 && at->ta==-1){
					at->ta = b;
					bt->tb = a;}
				else if (v1b_v2c!=0 && v1c_v2b!=0 && at->tb==-1){
					at->tb = b;
					bt->tb = a;}
				else if (v1c_v2c!=0 && v1a_v2b!=0 && at->tc==-1){
					at->tc = b;
					bt->tb = a;}
			}
			if (bt->tc==-1){
				if (v1a_v2a!=0 && v1b_v2c!=0 && at->ta==-1){
					at->ta = b;
					bt->tc = a;}
				else if (v1b_v2a!=0 && v1c_v2c!=0 && at->tb==-1){
					at->tb = b;
					bt->tc = a;}
				else if (v1c_v2a!=0 && v1a_v2c!=0 && at->tc==-1){
					at->tc = b;
					bt->tc = a;
				}
			}
		}

	}
	InitShadow();
}

void ShadowObject::InitShadow(){
	if (Global::Shadows_enabled==0){
	  Global::Shadows_enabled=true;
	  ShadowInit();
	}
	for (int v = 0;v<=cnt_tris;v++){
		ShadowTriangle* etet = shadow_triangles[v];
		if (etet->ta > -1) etet->id_ta = shadow_triangles[etet->ta];
		if (etet->tb > -1) etet->id_tb = shadow_triangles[etet->tb];
		if (etet->tc > -1) etet->id_tc = shadow_triangles[etet->tc];
	}
}

void ShadowObject::UpdateCaster(){
	if (static_object && volume_created) return;
	volume_created=true;
	if (ShadowVolume) ShadowVolume->ClearSurface();
	if (parent->anim!=0) UpdateAnim();

	float e0x, e0y, e0z, e1x, e1y, e1z;

	float normlight_x, normlight_y, normlight_z;

	float r1x, r1y, r1z, r2x, r2y, r2z, r3x, r3y, r3z;

	int vert1, vert2, vert3, vert4;

	char check1=0;
	char check2=0;
	char check3=0;

	/*Matrix mat1;
	Parent->MQ_GetMatrix(mat1, true);*/

	vector<ShadowTriangle*>::iterator it;


/*	for (int v = 0;v<=cnt_tris;v++){
		ShadowTriangle* etet = Tri[v];

		Entity::TFormPoint (etet->v1x, etet->v1y, etet->v1z, Parent, 0);
		etet->tf_v1x= Entity::TFormedX();
		etet->tf_v1y= Entity::TFormedY();
		etet->tf_v1z= Entity::TFormedZ();
		Entity::TFormPoint (etet->v2x, etet->v2y, etet->v2z, Parent, 0);
		etet->tf_v2x= Entity::TFormedX();
		etet->tf_v2y= Entity::TFormedY();
		etet->tf_v2z= Entity::TFormedZ();
		Entity::TFormPoint (etet->v3x, etet->v3y, etet->v3z, Parent, 0);
		etet->tf_v3x= Entity::TFormedX();
		etet->tf_v3y= Entity::TFormedY();
		etet->tf_v3z= Entity::TFormedZ();
		*/
	// FIXME: C++11'ify
	for(it=shadow_triangles.begin();it!=shadow_triangles.end();it++){
		ShadowTriangle* etet = *it;

		etet->tf_v1x= etet->v1x;
		etet->tf_v1y= etet->v1y;
		etet->tf_v1z= etet->v1z;
		parent->mat.TransformVec(etet->tf_v1x, etet->tf_v1y, etet->tf_v1z, 1);

		etet->tf_v2x= etet->v2x;
		etet->tf_v2y= etet->v2y;
		etet->tf_v2z= etet->v2z;
		parent->mat.TransformVec(etet->tf_v2x, etet->tf_v2y, etet->tf_v2z, 1);

		etet->tf_v3x= etet->v3x;
		etet->tf_v3y= etet->v3y;
		etet->tf_v3z= etet->v3z;
		parent->mat.TransformVec(etet->tf_v3x, etet->tf_v3y, etet->tf_v3z, 1);

		e0x= etet->tf_v3x - etet->tf_v2x;
		e0y= etet->tf_v3y - etet->tf_v2y;
		e0z= etet->tf_v3z - etet->tf_v2z;
		e1x= etet->tf_v2x - etet->tf_v1x;
		e1y= etet->tf_v2y - etet->tf_v1y;
		e1z= etet->tf_v2z - etet->tf_v1z;
		normlight_x  = (etet->tf_v1x  - light_x ) * (e0y  * e1z  - e0z  * e1y );
		normlight_y  = (etet->tf_v1y  - light_y ) * (e0z  * e1x  - e0x  * e1z );
		normlight_z  = (etet->tf_v1z  - light_z ) * (e0x  * e1y  - e0y  * e1x );
		etet->cull = (normlight_x  + normlight_y  + normlight_z  > 0);
	}

//	for (int v = 0;v<=cnt_tris;v++){
//		ShadowTriangle* etet = Tri[v];
	for(it=shadow_triangles.begin();it!=shadow_triangles.end();it++){
		ShadowTriangle* etet = *it;
		r1x  = etet->tf_v1x  + (etet->tf_v1x  - light_x ) * VolumeLength;
		r1y  = etet->tf_v1y  + (etet->tf_v1y  - light_y ) * VolumeLength;
		r1z  = etet->tf_v1z  + (etet->tf_v1z  - light_z ) * VolumeLength;
		r2x  = etet->tf_v2x  + (etet->tf_v2x  - light_x ) * VolumeLength;
		r2y  = etet->tf_v2y  + (etet->tf_v2y  - light_y ) * VolumeLength;
		r2z  = etet->tf_v2z  + (etet->tf_v2z  - light_z ) * VolumeLength;
		r3x  = etet->tf_v3x  + (etet->tf_v3x  - light_x ) * VolumeLength;
		r3y  = etet->tf_v3y  + (etet->tf_v3y  - light_y ) * VolumeLength;
		r3z  = etet->tf_v3z  + (etet->tf_v3z  - light_z ) * VolumeLength;



		if (etet->cull == 0) {
			if (etet->ta > -1) {check1 = etet->id_ta->cull;} else {check1 = 1;}
			if (etet->tb > -1) {check2 = etet->id_tb->cull;} else {check2 = 1;}
			if (etet->tc > -1) {check3 = etet->id_tc->cull;} else {check3 = 1;}

			if (check1!=0){
				vert1 = ShadowVolume->AddVertex(etet->tf_v1x , etet->tf_v1y , etet->tf_v1z);
				vert2 = ShadowVolume->AddVertex(etet->tf_v2x , etet->tf_v2y , etet->tf_v2z);
				vert3 = ShadowVolume->AddVertex(r1x , r1y , r1z);
				vert4 = ShadowVolume->AddVertex(r2x , r2y , r2z);
				ShadowVolume->AddTriangle (vert1, vert3, vert4);
				ShadowVolume->AddTriangle (vert1, vert4, vert2);
			}
			if (check2!=0){
				vert1 = ShadowVolume->AddVertex(etet->tf_v2x , etet->tf_v2y , etet->tf_v2z);
				vert2 = ShadowVolume->AddVertex(etet->tf_v3x , etet->tf_v3y , etet->tf_v3z);
				vert3 = ShadowVolume->AddVertex(r2x , r2y , r2z);
				vert4 = ShadowVolume->AddVertex(r3x , r3y , r3z);
				ShadowVolume->AddTriangle (vert1, vert3, vert4);
				ShadowVolume->AddTriangle (vert1, vert4, vert2);
			}
			if (check3!=0){
				vert1 = ShadowVolume->AddVertex(etet->tf_v1x , etet->tf_v1y , etet->tf_v1z);
				vert2 = ShadowVolume->AddVertex(etet->tf_v3x , etet->tf_v3y , etet->tf_v3z);
				vert3 = ShadowVolume->AddVertex(r1x , r1y , r1z);
				vert4 = ShadowVolume->AddVertex(r3x , r3y , r3z);
				ShadowVolume->AddTriangle (vert1, vert4, vert3);
				ShadowVolume->AddTriangle (vert1, vert2, vert4);
			}
			if (top_caps){
				vert1 = ShadowVolume->AddVertex(etet->tf_v1x , etet->tf_v1y , etet->tf_v1z);
				vert2 = ShadowVolume->AddVertex(etet->tf_v2x , etet->tf_v2y , etet->tf_v2z);
				vert3 = ShadowVolume->AddVertex(etet->tf_v3x , etet->tf_v3y , etet->tf_v3z);

				ShadowVolume->AddTriangle (vert1, vert2, vert3);
			}
		/*} else */
			if (parallel == 0){
				vert1 = ShadowVolume->AddVertex(r1x , r1y , r1z);
				vert2 = ShadowVolume->AddVertex(r2x , r2y , r2z);
				vert3 = ShadowVolume->AddVertex(r3x , r3y , r3z);
				ShadowVolume->AddTriangle (vert1, vert3, vert2);
			}
		}
	}
}

void ShadowObject::ShadowRenderWorldZFail(){
	glEnable(GL_STENCIL_TEST);
	/*Pass 1 Ambient Pass
	 First clear the stencil Buffer */
	glClearStencil(midStencilVal);

	glClear(GL_STENCIL_BUFFER_BIT);

	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	// we wanted the first ambient pass above in the Color Buffer but we don't want the volumes in
	// there ..removing the above line is fun though :P
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);

	// Render the shadow volume And increment the stencil every where a front
	// facing polygon is rendered.

	// Incrementing the stencil Buffer when back face depth fails
	glStencilFunc(GL_ALWAYS,midStencilVal,0xffffffff); // ~0 is like 0xFFFFFFFF Or something :P ( eh yes. ~0 is -1 signed or 0xffffffff unsigned for a 32 bit number, 'Bitwise NOT' )
	glStencilOp(GL_KEEP,GL_INCR_WRAP,GL_KEEP); // incrementing on the depth fail
	glCullFace(GL_BACK); // cull front facing polys For this pass

	RenderVolume();

	// Render the shadow volume And decrement the stencil every where a back
	// facing polygon is rendered.
	glStencilOp(GL_KEEP,GL_DECR_WRAP,GL_KEEP); // decrementing on the depth fail
	glCullFace(GL_FRONT); // And now culling back facing polys

	RenderVolume();

	// When done, set the states back To something more typical.
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);

	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

	//
	// Render the lit part...
	//
	glStencilFunc(GL_EQUAL,midStencilVal,0xffffffff); // again with the ~0 :P
	// When done, set the states back To something more typical.
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK); // cull front facing polys For this pass

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);

	glCullFace(GL_BACK);
	glStencilFunc(GL_NOTEQUAL, midStencilVal, 0xffffff);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

// NOTE: is it the projektion matrix ?
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0,1,1,0,0,1);

	//float no_mat[]={0.0,0.0};
	float mat_ambient[]={ShadowRed,ShadowGreen,ShadowBlue,1.0};
	float mat_diffuse[]={0,0,0,ShadowAlpha};
	float mat_specular[]={0,0,0,0.5};
	float mat_shininess[]={0.0}; // upto 128

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.0,0.0,0.0,1.0);

	/*glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(0, 1);
		glVertex2i(1, 1);
		glVertex2i(1, 0);
	glEnd();*/
	if(Global::fx1!=true){
		Global::fx1=true;
		glDisableClientState(GL_NORMAL_ARRAY);
	}
	if(Global::fx2!=false){
		Global::fx2=false;
		glDisableClientState(GL_COLOR_ARRAY);
	}

	GLfloat q3[] = {0,0,0,1,1,1,1,0};
	 
	glVertexPointer(2,GL_FLOAT,0,q3);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);


	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	// NOTE: is it the projektion matrix ?
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glCullFace(GL_BACK); // cull front facing polys For this pass
	glDisable(GL_STENCIL_TEST);

}


