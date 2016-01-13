#ifndef FUNCTIONS_CPP_H
#define FUNCTIONS_CPP_H

void DLL_EXPORT TextureGLTexEnv(bbTexture* tex, int target, int pname, int param);
void DLL_EXPORT BrushGLColor(bbBrush* brush, float r, float g, float b, float a=1);
void DLL_EXPORT BrushGLBlendFunc(bbBrush* brush, int sfactor, int dfactor);

void DLL_EXPORT BufferToTex(bbTexture* tex,unsigned char* buffer, int frame=0);
void DLL_EXPORT BackBufferToTex(bbTexture* tex,int frame=0);
void DLL_EXPORT TexToBuffer(bbTexture* tex,unsigned char* buffer, int frame=0);

void DLL_EXPORT CameraToTex(bbTexture* tex,bbCamera* cam,int frame=0);


/*
bbdoc: Minib3d Only
about:
This command is the equivalent of Blitz3D's MeshCullBox command.

It is used to set the radius of a mesh's 'cull sphere' - if the 'cull sphere' is not inside the viewing area, the mesh will not
be rendered.

A mesh's cull radius is set automatically, therefore in most cases you will not have to use this command.

One time you may have to use it is for animated meshes where the default cull radius may not take into account all animation
positions, resulting in the mesh being wrongly culled at extreme positions.
*/
void DLL_EXPORT MeshCullRadius(bbEntity* ent, float radius);
// Blitz3D functions, A-Z

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AddAnimSeq">Online Help</a>
*/
int DLL_EXPORT AddAnimSeq(bbEntity* ent,int length);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AddMesh">Online Help</a>
*/
void DLL_EXPORT AddMesh(bbMesh* mesh1,bbMesh* mesh2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AddTriangle">Online Help</a>
*/
int DLL_EXPORT AddTriangle(bbSurface* surf,int v0,int v1,int v2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AddVertex">Online Help</a>
*/
int DLL_EXPORT AddVertex(bbSurface* surf,float x, float y,float z,float u=0, float v=0,float w=0);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AmbientLight">Online Help</a>
*/
void DLL_EXPORT AmbientLight(float r,float g,float b);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AntiAlias">Online Help</a>
*/
void DLL_EXPORT AntiAlias(int samples);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=Animate">Online Help</a>
*/
void DLL_EXPORT Animate(bbEntity* ent,int mode=1,float speed=1,int seq=0,int trans=0);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=Animating">Online Help</a>
*/
int DLL_EXPORT Animating(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AnimLength">Online Help</a>
*/
int DLL_EXPORT AnimLength(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AnimSeq">Online Help</a>
*/
int DLL_EXPORT AnimSeq(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=AnimTime">Online Help</a>
*/
float DLL_EXPORT AnimTime(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushAlpha">Online Help</a>
*/
void DLL_EXPORT BrushAlpha(bbBrush* brush, float a);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushBlend">Online Help</a>
*/
void DLL_EXPORT BrushBlend(bbBrush* brush,int blend);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushColor">Online Help</a>
*/
void DLL_EXPORT BrushColor(bbBrush* brush,float r,float g,float b);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushFX">Online Help</a>
*/
void DLL_EXPORT BrushFX(bbBrush* brush,int fx);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushShininess">Online Help</a>
*/
void DLL_EXPORT BrushShininess(bbBrush* brush,float s);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=BrushTexture">Online Help</a>
*/
void DLL_EXPORT BrushTexture(bbBrush* brush,bbTexture* tex,int frame=0,int index=0);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraClsColor">Online Help</a>
*/
void DLL_EXPORT CameraClsColor(bbCamera* cam, float r,float g,float b);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraClsMode">Online Help</a>
*/
void DLL_EXPORT CameraClsMode(bbCamera* cam,int cls_depth,int cls_zbuffer);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraFogColor">Online Help</a>
*/
void DLL_EXPORT CameraFogColor(bbCamera* cam,float r,float g,float b);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraFogMode">Online Help</a>
*/
void DLL_EXPORT CameraFogMode(bbCamera* cam,int mode);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraFogRange">Online Help</a>
*/
void DLL_EXPORT CameraFogRange(bbCamera* cam,float nnear,float nfar);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraPick">Online Help</a>
*/
bbEntity* DLL_EXPORT CameraPick(bbCamera* cam,float x,float y);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraProject">Online Help</a>
*/
void DLL_EXPORT CameraProject(bbCamera* cam,float x,float y,float z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraProjMode">Online Help</a>
*/
void DLL_EXPORT CameraProjMode(bbCamera* cam,int mode);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraRange">Online Help</a>
*/
void DLL_EXPORT CameraRange(bbCamera* cam,float nnear,float nfar);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraViewport">Online Help</a>
*/
void DLL_EXPORT CameraViewport(bbCamera* cam,int x,int y,int width,int height);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CameraZoom">Online Help</a>
*/
void DLL_EXPORT CameraZoom(bbCamera* cam,float zoom);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ClearCollisions">Online Help</a>
int ClearCollisions(){
	Global::ClearCollisions();
}
*/
/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ClearSurface">Online Help</a>
*/
void DLL_EXPORT ClearSurface(bbSurface* surf,int clear_verts=1,int clear_tris=1);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ClearTextureFilters">Online Help</a>
*/
void DLL_EXPORT ClearTextureFilters();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ClearWorld">Online Help</a>
*/
void DLL_EXPORT ClearWorld(int entities=1,int brushes=1,int textures=1);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionEntity">Online Help</a>
*/
bbEntity* DLL_EXPORT CollisionEntity(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=Collisions">Online Help</a>
*/
void DLL_EXPORT Collisions(int src_no,int dest_no,int method_no,int response_no=0);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionNX">Online Help</a>
*/
float DLL_EXPORT CollisionNX(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionNY">Online Help</a>
*/
float DLL_EXPORT CollisionNY(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionNZ">Online Help</a>
*/
float DLL_EXPORT CollisionNZ(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionSurface">Online Help</a>
*/
bbSurface* DLL_EXPORT CollisionSurface(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionTime">Online Help</a>
*/
float DLL_EXPORT CollisionTime(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionTriangle">Online Help</a>
*/
int DLL_EXPORT CollisionTriangle(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionX">Online Help</a>
*/
float DLL_EXPORT CollisionX(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionY">Online Help</a>
*/
float DLL_EXPORT CollisionY(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CollisionZ">Online Help</a>
*/
float DLL_EXPORT CollisionZ(bbEntity* ent,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CountChildren">Online Help</a>
*/
int DLL_EXPORT CountChildren(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CountCollisions">Online Help</a>
*/
int DLL_EXPORT CountCollisions(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CopyEntity">Online Help</a>
*/
bbEntity* DLL_EXPORT CopyEntity(bbEntity* ent,bbEntity* parent=nullptr);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CopyMesh">Online Help</a>
*/
bbMesh* DLL_EXPORT CopyMesh(bbMesh* mesh,bbEntity* parent=nullptr);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CountSurfaces">Online Help</a>
*/
int DLL_EXPORT CountSurfaces(bbMesh* mesh);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CountTriangles">Online Help</a>
*/
int DLL_EXPORT CountTriangles(bbSurface* surf);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CountVertices">Online Help</a>
*/
int DLL_EXPORT CountVertices(bbSurface* surf);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateBrush">Online Help</a>
*/
bbBrush* DLL_EXPORT CreateBrush(float r,float g,float b);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateCamera">Online Help</a>
*/
bbCamera* DLL_EXPORT CreateCamera(bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateCone">Online Help</a>
*/
bbMesh* DLL_EXPORT CreateCone(int segments,int solid,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateCylinder">Online Help</a>
*/
bbMesh* DLL_EXPORT CreateCylinder(int segments,int solid,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateCube">Online Help</a>
*/
bbMesh* DLL_EXPORT CreateCube(bbEntity* parent);

bbTerrain* DLL_EXPORT CreateGeosphere(int size, bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateMesh">Online Help</a>
*/
bbMesh* DLL_EXPORT CreateMesh(bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateLight">Online Help</a>
*/
bbLight* DLL_EXPORT CreateLight(int light_type,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreatePivot">Online Help</a>
*/
bbPivot* DLL_EXPORT CreatePivot(bbEntity* parent);

bbMesh* DLL_EXPORT CreatePlane(int divisions,bbEntity* parent);

bbMesh* DLL_EXPORT CreateQuad(bbEntity* parent);

bbShadowObject* DLL_EXPORT CreateShadow(bbMesh* parent, char Static);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateSphere">Online Help</a>
*/
bbMesh* DLL_EXPORT CreateSphere(int segments,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateSprite">Online Help</a>
*/
bbSprite* DLL_EXPORT CreateSprite(bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateSurface">Online Help</a>
*/
bbSurface* DLL_EXPORT CreateSurface(bbMesh* mesh,bbBrush* brush);

/*
*/
bbStencil* DLL_EXPORT CreateStencil();

/*
*/
void DLL_EXPORT FreeStencil(bbStencil *stencil);

/*
*/
bbTerrain* DLL_EXPORT CreateTerrain(int size, bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=CreateTexture">Online Help</a>
*/
bbTexture* DLL_EXPORT CreateTexture(int width,int height,int flags,int frames);

/*
*/
bbVoxelSprite* DLL_EXPORT CreateVoxelSprite(int slices, bbEntity* parent);


/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=DeltaPitch">Online Help</a>
*/
float DLL_EXPORT DeltaPitch(bbEntity* ent1,bbEntity* ent2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=DeltaYaw">Online Help</a>
*/
float DLL_EXPORT DeltaYaw(bbEntity* ent1,bbEntity* ent2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityAlpha">Online Help</a>
*/
void DLL_EXPORT EntityAlpha(bbEntity* ent,float alpha);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityAutoFade">Online Help</a>
*/
void DLL_EXPORT EntityAutoFade(bbEntity* ent,float near,float far);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityBlend">Online Help</a>
*/
void DLL_EXPORT EntityBlend(bbEntity* ent, int blend);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityBox">Online Help</a>
*/
void DLL_EXPORT EntityBox(bbEntity* ent,float x,float y,float z,float w,float h,float d);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityClass">Online Help</a>
*/
const char* DLL_EXPORT EntityClass(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityCollided">Online Help</a>
*/
bbEntity* DLL_EXPORT EntityCollided(bbEntity* ent,int type_no);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityColor">Online Help</a>
*/
void DLL_EXPORT EntityColor(bbEntity* ent,float red,float green,float blue);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityDistance">Online Help</a>
*/
float DLL_EXPORT EntityDistance(bbEntity* ent1,bbEntity* ent2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityFX">Online Help</a>
*/
void DLL_EXPORT EntityFX(bbEntity* ent,int fx);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityInView">Online Help</a>
*/
int DLL_EXPORT EntityInView(bbEntity* ent,bbCamera* cam);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityName">Online Help</a>
*/
const char* DLL_EXPORT EntityName(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityOrder">Online Help</a>
*/
void DLL_EXPORT EntityOrder(bbEntity* ent,int order);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityParent">Online Help</a>
*/
void DLL_EXPORT EntityParent(bbEntity* ent,bbEntity* parent_ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityPick">Online Help</a>
*/
bbEntity* DLL_EXPORT EntityPick(bbEntity* ent,float range);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityPickMode">Online Help</a>
*/
void DLL_EXPORT EntityPickMode(bbEntity* ent,int pick_mode,int obscurer);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityPitch">Online Help</a>
*/
float DLL_EXPORT EntityPitch(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityRadius">Online Help</a>
*/
void DLL_EXPORT EntityRadius(bbEntity* ent,float radius_x,float radius_y);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityRoll">Online Help</a>
*/
float DLL_EXPORT EntityRoll(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityShininess">Online Help</a>
*/
void DLL_EXPORT EntityShininess(bbEntity* ent,float shine);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityTexture">Online Help</a>
*/
void DLL_EXPORT EntityTexture(bbEntity* ent,bbTexture* tex,int frame,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityType">Online Help</a>
*/
void DLL_EXPORT EntityType(bbEntity* ent,int type_no,int recursive);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityVisible">Online Help</a>
*/
int DLL_EXPORT EntityVisible(bbEntity* src_ent,bbEntity* dest_ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityX">Online Help</a>
*/
float DLL_EXPORT EntityX(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityY">Online Help</a>
*/
float DLL_EXPORT EntityY(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityYaw">Online Help</a>
*/
float DLL_EXPORT EntityYaw(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=EntityZ">Online Help</a>
*/
float DLL_EXPORT EntityZ(bbEntity* ent,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ExtractAnimSeq">Online Help</a>
*/
int DLL_EXPORT ExtractAnimSeq(bbEntity* ent,int first_frame,int last_frame,int seq);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FindChild">Online Help</a>
*/
bbEntity* DLL_EXPORT FindChild(bbEntity* ent,char* child_name);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FindSurface">Online Help</a>
*/
bbSurface* DLL_EXPORT FindSurface(bbMesh* mesh,bbBrush* brush);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FitMesh">Online Help</a><p>
*/
void DLL_EXPORT FitMesh(bbMesh* mesh,float x,float y,float z,float width,float height,float depth,int uniform);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FlipMesh">Online Help</a>
*/
void DLL_EXPORT FlipMesh(bbMesh* mesh);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FreeBrush">Online Help</a>
*/
void DLL_EXPORT FreeBrush(bbBrush* brush);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FreeEntity">Online Help</a>
*/
void DLL_EXPORT FreeEntity(bbEntity* ent);

void DLL_EXPORT FreeShadow(bbShadowObject* shad);


/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=FreeTexture">Online Help</a>
*/
void DLL_EXPORT FreeTexture(bbTexture* tex);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetBrushTexture">Online Help</a>
*/
bbTexture* DLL_EXPORT GetBrushTexture(bbBrush* brush,int index);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetChild">Online Help</a>
*/
bbEntity* DLL_EXPORT GetChild(bbEntity* ent,int child_no);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetEntityBrush">Online Help</a>
*/
bbBrush* DLL_EXPORT GetEntityBrush(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetEntityType">Online Help</a>
*/
int DLL_EXPORT GetEntityType(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ResetEntity">Online Help</a>
*/
float DLL_EXPORT GetMatElement(bbEntity* ent,int row,int col);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetParent">Online Help</a>
*/
bbEntity* DLL_EXPORT GetParentEntity(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetSurface">Online Help</a>
*/
bbSurface* DLL_EXPORT GetSurface(bbMesh* mesh,int surf_no);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=GetSurfaceBrush">Online Help</a>
*/
bbBrush* DLL_EXPORT GetSurfaceBrush(bbSurface* surf);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=Graphics3D">Online Help</a>
*/
void DLL_EXPORT Graphics3D(int width,int height,int depth=0,int mode=0,int rate=60);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=HandleSprite">Online Help</a>
*/
void DLL_EXPORT HandleSprite(bbSprite* sprite,float h_x,float h_y);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=HideEntity">Online Help</a>
*/
void DLL_EXPORT HideEntity(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LightColor">Online Help</a>
*/
void DLL_EXPORT LightColor(bbLight* light,float red,float green,float blue);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LightConeAngles">Online Help</a>
*/
void DLL_EXPORT LightConeAngles(bbLight* light,float inner_ang,float outer_ang);

/*

bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LightRange">Online Help</a>
*/
void DLL_EXPORT LightRange(bbLight* light,float range);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LinePick">Online Help</a>
*/
bbEntity* DLL_EXPORT LinePick(float x,float y,float z,float dx,float dy,float dz,float radius);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadAnimMesh">Online Help</a>
*/
bbMesh* DLL_EXPORT LoadAnimMesh(char* file,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadAnimTexture">Online Help</a>
*/
bbTexture* DLL_EXPORT LoadAnimTexture(char* file,int flags,int frame_width,int frame_height,int first_frame,int frame_count);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadBrush">Online Help</a>
*/
bbBrush* DLL_EXPORT LoadBrush(char *file,int flags,float u_scale,float v_scale);

bbTerrain* DLL_EXPORT LoadGeosphere(char* file,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadMesh">Online Help</a>
*/
bbMesh* DLL_EXPORT LoadMesh(char* file,bbEntity* parent);


bbTerrain* DLL_EXPORT LoadTerrain(char* file,bbEntity* parent);


/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadTexture">Online Help</a>
*/
bbTexture* DLL_EXPORT LoadTexture(char* file,int flags);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=LoadSprite">Online Help</a>
*/
bbSprite* DLL_EXPORT LoadSprite(char* tex_file,int tex_flag,bbEntity* parent);

/*
*/
bbMesh* DLL_EXPORT MeshCSG(bbMesh* m1, bbMesh* m2, int method);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=MeshDepth">Online Help</a>
*/
float DLL_EXPORT MeshDepth(bbMesh* mesh);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=MeshesIntersect">Online Help</a>
*/
int DLL_EXPORT MeshesIntersect(bbMesh* mesh1,bbMesh* mesh2);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=MeshHeight">Online Help</a>
*/
float DLL_EXPORT MeshHeight(bbMesh* mesh);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=MeshWidth">Online Help</a>
*/
float DLL_EXPORT MeshWidth(bbMesh* mesh);

void DLL_EXPORT ModifyGeosphere(bbGeosphere* geo, int x, int z, float new_height);

void DLL_EXPORT ModifyTerrain(bbTerrain* terr, int x, int z, float new_height);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=MoveEntity">Online Help</a>
*/
void DLL_EXPORT MoveEntity(bbEntity* ent,float x,float y,float z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=NameEntity">Online Help</a>
*/
void DLL_EXPORT NameEntity(bbEntity* ent,char* name);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PaintEntity">Online Help</a>
*/
void DLL_EXPORT PaintEntity(bbEntity* ent,bbBrush* brush);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PaintMesh">Online Help</a>
*/
void DLL_EXPORT PaintMesh(bbMesh* mesh,bbBrush* brush);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PaintSurface">Online Help</a>
*/
void DLL_EXPORT PaintSurface(bbSurface* surf,bbBrush* brush);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedEntity">Online Help</a>
*/
bbEntity* DLL_EXPORT PickedEntity();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedNX">Online Help</a>
*/
float DLL_EXPORT PickedNX();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedNY">Online Help</a>
*/
float DLL_EXPORT PickedNY();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedNZ">Online Help</a>
*/
float DLL_EXPORT PickedNZ();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedSurface">Online Help</a>
*/
bbSurface* DLL_EXPORT PickedSurface();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedTime">Online Help</a>
*/
float DLL_EXPORT PickedTime();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedTriangle">Online Help</a>
*/
int DLL_EXPORT PickedTriangle();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedX">Online Help</a>
*/
float DLL_EXPORT PickedX();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedY">Online Help</a>
*/
float DLL_EXPORT PickedY();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PickedZ">Online Help</a>
*/
float DLL_EXPORT PickedZ();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PointEntity">Online Help</a>
*/
void DLL_EXPORT PointEntity(bbEntity* ent,bbEntity* target_ent,float roll);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PositionEntity">Online Help</a>
*/
void DLL_EXPORT PositionEntity(bbEntity* ent,float x,float y,float z,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PositionMesh">Online Help</a>
*/
void DLL_EXPORT PositionMesh(bbMesh* mesh,float px,float py,float pz);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=PositionTexture">Online Help</a>
*/
void DLL_EXPORT PositionTexture(bbTexture* tex,float u_pos,float v_pos);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ProjectedX">Online Help</a>
*/
float DLL_EXPORT ProjectedX();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ProjectedY">Online Help</a>
*/
float DLL_EXPORT ProjectedY();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ProjectedZ">Online Help</a>
*/
float DLL_EXPORT ProjectedZ();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RenderWorld">Online Help</a>
*/
void DLL_EXPORT RenderWorld();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RepeatMesh">Online Help</a>
*/
bbMesh* DLL_EXPORT RepeatMesh(bbMesh* mesh,bbEntity* parent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ResetEntity">Online Help</a>
*/
void DLL_EXPORT ResetEntity(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RotateEntity">Online Help</a>
*/
void DLL_EXPORT RotateEntity(bbEntity* ent,float x,float y,float z,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RotateMesh">Online Help</a>
*/
void DLL_EXPORT RotateMesh(bbMesh* mesh,float pitch,float yaw,float roll);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RotateSprite">Online Help</a>
*/
void DLL_EXPORT RotateSprite(bbSprite* sprite,float ang);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=RotateTexture">Online Help</a>
*/
void DLL_EXPORT RotateTexture(bbTexture* tex,float ang);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ScaleEntity">Online Help</a>
*/
void DLL_EXPORT ScaleEntity(bbEntity* ent,float x,float y,float z,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ScaleMesh">Online Help</a>
*/
void DLL_EXPORT ScaleMesh(bbMesh* mesh,float sx,float sy,float sz);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ScaleSprite">Online Help</a>
*/
void DLL_EXPORT ScaleSprite(bbSprite* sprite,float s_x,float s_y);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ScaleTexture">Online Help</a>
*/
void DLL_EXPORT ScaleTexture(bbTexture* tex,float u_scale,float v_scale);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=SetAnimTime">Online Help</a>
*/
void DLL_EXPORT SetAnimTime(bbEntity* ent,float time,int seq);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=SetCubeFace">Online Help</a>
*/
void DLL_EXPORT SetCubeFace(bbTexture* tex,int face);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=SetCubeMode">Online Help</a>
*/
void DLL_EXPORT SetCubeMode(bbTexture* tex,int mode);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=ShowEntity">Online Help</a>
*/
void DLL_EXPORT ShowEntity(bbEntity* ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=SpriteViewMode">Online Help</a>
*/
void DLL_EXPORT SpriteViewMode(bbSprite* sprite,int mode);

/*
*/
void DLL_EXPORT StencilAlpha(bbStencil* stencil, float a);

/*
*/
void DLL_EXPORT StencilClsColor(bbStencil* stencil, float r,float g,float b);

/*
*/
void DLL_EXPORT StencilClsMode(bbStencil* stencil,int cls_depth,int cls_zbuffer);

/*
*/
void DLL_EXPORT StencilMesh(bbStencil* stencil, bbMesh* mesh, int mode);

/*
*/
void DLL_EXPORT StencilMode(bbStencil* stencil, int m, int o);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TerrainHeight">Online Help</a>
*/
float DLL_EXPORT TerrainHeight (bbTerrain* terr, int x, int z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TerrainX">Online Help</a>
*/
float DLL_EXPORT TerrainX(bbTerrain* terr, float x, float y, float z);

/*

bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TerrainY">Online Help</a>
*/
float DLL_EXPORT TerrainY(bbTerrain* terr, float x, float y, float z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TerrainZ">Online Help</a>
*/
float DLL_EXPORT TerrainZ(bbTerrain* terr, float x, float y, float z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureBlend">Online Help</a>
*/
void DLL_EXPORT TextureBlend(bbTexture* tex,int blend);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureCoords">Online Help</a>
*/
void DLL_EXPORT TextureCoords(bbTexture* tex,int coords);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureHeight">Online Help</a>
*/
int DLL_EXPORT TextureHeight(bbTexture* tex);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureFilter">Online Help</a>
*/
void DLL_EXPORT TextureFilter(char* match_text,int flags);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureName">Online Help</a>
*/
const char* DLL_EXPORT TextureName(bbTexture* tex);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TextureWidth">Online Help</a>
*/
int DLL_EXPORT TextureWidth(bbTexture* tex);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormedX">Online Help</a>
*/
float DLL_EXPORT TFormedX();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormedY">Online Help</a>
*/
float DLL_EXPORT TFormedY();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormedZ">Online Help</a>
*/
float DLL_EXPORT TFormedZ();

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormNormal">Online Help</a>
*/
void DLL_EXPORT TFormNormal(float x,float y,float z,bbEntity* src_ent,bbEntity* dest_ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormPoint">Online Help</a>
*/
void DLL_EXPORT TFormPoint(float x,float y,float z,bbEntity* src_ent,bbEntity* dest_ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TFormVector">Online Help</a>
*/
void DLL_EXPORT TFormVector(float x,float y,float z,bbEntity* src_ent,bbEntity* dest_ent);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TranslateEntity">Online Help</a>
*/
void DLL_EXPORT TranslateEntity(bbEntity* ent,float x,float y,float z,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TriangleVertex">Online Help</a>
*/
int DLL_EXPORT TriangleVertex(bbSurface* surf,int tri_no,int corner);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=TurnEntity">Online Help</a>
*/
void DLL_EXPORT TurnEntity(bbEntity* ent,float x,float y,float z,int glob);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=UpdateNormals">Online Help</a>
*/
void DLL_EXPORT UpdateNormals(bbMesh* mesh);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=UpdateNormals">Online Help</a>
*/
void DLL_EXPORT UpdateTexCoords(bbSurface* surf);


/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=UpdateWorld">Online Help</a>
*/
void DLL_EXPORT UpdateWorld(float anim_speed);

/*
*/
void DLL_EXPORT UseStencil(bbStencil* stencil);
/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VectorPitch">Online Help</a>
*/
float DLL_EXPORT VectorPitch(float vx,float vy,float vz);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VectorYaw">Online Help</a>
*/
float DLL_EXPORT VectorYaw(float vx,float vy,float vz);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexAlpha">Online Help</a>
*/
float DLL_EXPORT VertexAlpha(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexBlue">Online Help</a>
*/
float DLL_EXPORT VertexBlue(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexColor">Online Help</a>
*/
void DLL_EXPORT VertexColor(bbSurface* surf,int vid,float r,float g,float b,float a);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexCoords">Online Help</a>
*/
void DLL_EXPORT VertexCoords(bbSurface* surf,int vid,float x,float y,float z);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexGreen">Online Help</a>
*/
float DLL_EXPORT VertexGreen(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexNormal">Online Help</a>
*/
void DLL_EXPORT VertexNormal(bbSurface* surf,int vid,float nx,float ny,float nz);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexNX">Online Help</a>
*/
float DLL_EXPORT VertexNX(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexNY">Online Help</a>
*/
float DLL_EXPORT VertexNY(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexNZ">Online Help</a>
*/
float DLL_EXPORT VertexNZ(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexRed">Online Help</a>
*/
float DLL_EXPORT VertexRed(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexTexCoords">Online Help</a>
*/
void DLL_EXPORT VertexTexCoords(bbSurface* surf,int vid,float u,float v,float w,int coord_set);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexU">Online Help</a>
*/
float DLL_EXPORT VertexU(bbSurface* surf,int vid,int coord_set);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexV">Online Help</a>
*/
float DLL_EXPORT VertexV(bbSurface* surf,int vid,int coord_set);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexW">Online Help</a>
*/
float DLL_EXPORT VertexW(bbSurface* surf,int vid,int coord_set);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexX">Online Help</a>
*/
float DLL_EXPORT VertexX(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexY">Online Help</a>
*/
float DLL_EXPORT VertexY(bbSurface* surf,int vid);

/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=VertexZ">Online Help</a>
*/
float DLL_EXPORT VertexZ(bbSurface* surf,int vid);

/*
*/
void DLL_EXPORT VoxelSpriteMaterial(bbVoxelSprite* voxelspr, bbMaterial* mat);


/*
bbdoc: <a href="http://www.blitzbasic.com/b3ddocs/command.php?name=Wireframe">Online Help</a>
*/
void DLL_EXPORT Wireframe(int enable);


float DLL_EXPORT EntityScaleX(bbEntity* ent,int glob);

float DLL_EXPORT EntityScaleY(bbEntity* ent,int glob);

float DLL_EXPORT EntityScaleZ(bbEntity* ent,int glob);

bbShader* DLL_EXPORT LoadShader(char* ShaderName, char* VshaderFileName, char* FshaderFileName);

bbShader* DLL_EXPORT CreateShader(char* ShaderName, char* VshaderString, char* FshaderString);

void DLL_EXPORT ShadeSurface(bbSurface* surf, bbShader* material);

void DLL_EXPORT ShadeMesh(bbMesh* mesh, bbShader* material);

void DLL_EXPORT ShadeEntity(bbEntity* ent, bbShader* material);

void DLL_EXPORT ShaderTexture(bbShader* material, bbTexture* tex, char* name, int index);

void DLL_EXPORT SetFloat(bbShader* material, char* name, float v1);

void DLL_EXPORT SetFloat2(bbShader* material, char* name, float v1, float v2);

void DLL_EXPORT SetFloat3(bbShader* material, char* name, float v1, float v2, float v3);

void DLL_EXPORT SetFloat4(bbShader* material, char* name, float v1, float v2, float v3, float v4);

void DLL_EXPORT UseFloat(bbShader* material, char* name, float* v1);

void DLL_EXPORT UseFloat2(bbShader* material, char* name, float* v1, float* v2);

void DLL_EXPORT UseFloat3(bbShader* material, char* name, float* v1, float* v2, float* v3);

void DLL_EXPORT UseFloat4(bbShader* material, char* name, float* v1, float* v2, float* v3, float* v4);

void DLL_EXPORT SetInteger(bbShader* material, char* name, int v1);

void DLL_EXPORT SetInteger2(bbShader* material, char* name, int v1, int v2);

void DLL_EXPORT SetInteger3(bbShader* material, char* name, int v1, int v2, int v3);

void DLL_EXPORT SetInteger4(bbShader* material, char* name, int v1, int v2, int v3, int v4);

void DLL_EXPORT UseInteger(bbShader* material, char* name, int* v1);

void DLL_EXPORT UseInteger2(bbShader* material, char* name, int* v1, int* v2);

void DLL_EXPORT UseInteger3(bbShader* material, char* name, int* v1, int* v2, int* v3);

void DLL_EXPORT UseInteger4(bbShader* material, char* name, int* v1, int* v2, int* v3, int* v4);

void DLL_EXPORT UseSurface(bbShader* material, char* name, bbSurface* surf, int vbo);

void DLL_EXPORT UseMatrix(bbShader* material, char* name, int mode);

bbMaterial* DLL_EXPORT LoadMaterial(char* filename,int flags, int frame_width,int frame_height,int first_frame,int frame_count);

void DLL_EXPORT ShaderMaterial(bbShader* material, bbMaterial* tex, char* name, int index);

bbOcTree* DLL_EXPORT CreateOcTree(float w, float h, float d, bbEntity* parent_ent);

void DLL_EXPORT AddToOctree(bbOcTree* octree, bbMesh* mesh, int level, float X, float Y, float Z, float Near, float Far);

// ==========================================================================================================
// New additions
#define SHADEROBJECT_VERTEX		0
#define SHADEROBJECT_FRAGMENT	1

bbMesh* DLL_EXPORT LoadAnimMeshFromBuffer(const void *buffer,int len,int type,bbEntity *parent=nullptr);
void DLL_EXPORT SetPixbufReader(unsigned char *(*)(const char*,int*,int*),void(*)(unsigned char*));
bool DLL_EXPORT AddFileResource(const char *filename,int reserved=0);
bbShader* DLL_EXPORT CreateShaderProgram(const char *name);
bbShaderObject* DLL_EXPORT LoadShaderObject(int type,const char *filename);
bbShaderObject* DLL_EXPORT CreateShaderObject(int type,const char*source,const char *name);
void DLL_EXPORT AttachShaderObject(bbShader *shader,bbShaderObject *object);
void DLL_EXPORT DetachShaderObject(bbShader *shader,bbShaderObject *object);
void DLL_EXPORT FreeShaderObject(bbShaderObject *object);

#endif // FUNCTIONS_CPP_H

