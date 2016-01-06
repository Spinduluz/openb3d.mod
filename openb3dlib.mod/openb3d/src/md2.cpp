#include "model.h"
#include "entity.h"
#include "bone.h"
#include "mesh.h"
#include "texture.h"
#include "file.h"
#include <list>

namespace loadMD2{

const float md2_anorms[][3]={
{ -0.525731f,  0.000000f,  0.850651f }, 
{ -0.442863f,  0.238856f,  0.864188f }, 
{ -0.295242f,  0.000000f,  0.955423f }, 
{ -0.309017f,  0.500000f,  0.809017f }, 
{ -0.162460f,  0.262866f,  0.951056f }, 
{  0.000000f,  0.000000f,  1.000000f }, 
{  0.000000f,  0.850651f,  0.525731f }, 
{ -0.147621f,  0.716567f,  0.681718f }, 
{  0.147621f,  0.716567f,  0.681718f }, 
{  0.000000f,  0.525731f,  0.850651f }, 
{  0.309017f,  0.500000f,  0.809017f }, 
{  0.525731f,  0.000000f,  0.850651f }, 
{  0.295242f,  0.000000f,  0.955423f }, 
{  0.442863f,  0.238856f,  0.864188f }, 
{  0.162460f,  0.262866f,  0.951056f }, 
{ -0.681718f,  0.147621f,  0.716567f }, 
{ -0.809017f,  0.309017f,  0.500000f }, 
{ -0.587785f,  0.425325f,  0.688191f }, 
{ -0.850651f,  0.525731f,  0.000000f }, 
{ -0.864188f,  0.442863f,  0.238856f }, 
{ -0.716567f,  0.681718f,  0.147621f }, 
{ -0.688191f,  0.587785f,  0.425325f }, 
{ -0.500000f,  0.809017f,  0.309017f }, 
{ -0.238856f,  0.864188f,  0.442863f }, 
{ -0.425325f,  0.688191f,  0.587785f }, 
{ -0.716567f,  0.681718f, -0.147621f }, 
{ -0.500000f,  0.809017f, -0.309017f }, 
{ -0.525731f,  0.850651f,  0.000000f }, 
{  0.000000f,  0.850651f, -0.525731f }, 
{ -0.238856f,  0.864188f, -0.442863f }, 
{  0.000000f,  0.955423f, -0.295242f }, 
{ -0.262866f,  0.951056f, -0.162460f }, 
{  0.000000f,  1.000000f,  0.000000f }, 
{  0.000000f,  0.955423f,  0.295242f }, 
{ -0.262866f,  0.951056f,  0.162460f }, 
{  0.238856f,  0.864188f,  0.442863f }, 
{  0.262866f,  0.951056f,  0.162460f }, 
{  0.500000f,  0.809017f,  0.309017f }, 
{  0.238856f,  0.864188f, -0.442863f }, 
{  0.262866f,  0.951056f, -0.162460f }, 
{  0.500000f,  0.809017f, -0.309017f }, 
{  0.850651f,  0.525731f,  0.000000f }, 
{  0.716567f,  0.681718f,  0.147621f }, 
{  0.716567f,  0.681718f, -0.147621f }, 
{  0.525731f,  0.850651f,  0.000000f }, 
{  0.425325f,  0.688191f,  0.587785f }, 
{  0.864188f,  0.442863f,  0.238856f }, 
{  0.688191f,  0.587785f,  0.425325f }, 
{  0.809017f,  0.309017f,  0.500000f }, 
{  0.681718f,  0.147621f,  0.716567f }, 
{  0.587785f,  0.425325f,  0.688191f }, 
{  0.955423f,  0.295242f,  0.000000f }, 
{  1.000000f,  0.000000f,  0.000000f }, 
{  0.951056f,  0.162460f,  0.262866f }, 
{  0.850651f, -0.525731f,  0.000000f }, 
{  0.955423f, -0.295242f,  0.000000f }, 
{  0.864188f, -0.442863f,  0.238856f }, 
{  0.951056f, -0.162460f,  0.262866f }, 
{  0.809017f, -0.309017f,  0.500000f }, 
{  0.681718f, -0.147621f,  0.716567f }, 
{  0.850651f,  0.000000f,  0.525731f }, 
{  0.864188f,  0.442863f, -0.238856f }, 
{  0.809017f,  0.309017f, -0.500000f }, 
{  0.951056f,  0.162460f, -0.262866f }, 
{  0.525731f,  0.000000f, -0.850651f }, 
{  0.681718f,  0.147621f, -0.716567f }, 
{  0.681718f, -0.147621f, -0.716567f }, 
{  0.850651f,  0.000000f, -0.525731f }, 
{  0.809017f, -0.309017f, -0.500000f }, 
{  0.864188f, -0.442863f, -0.238856f }, 
{  0.951056f, -0.162460f, -0.262866f }, 
{  0.147621f,  0.716567f, -0.681718f }, 
{  0.309017f,  0.500000f, -0.809017f }, 
{  0.425325f,  0.688191f, -0.587785f }, 
{  0.442863f,  0.238856f, -0.864188f }, 
{  0.587785f,  0.425325f, -0.688191f }, 
{  0.688191f,  0.587785f, -0.425325f }, 
{ -0.147621f,  0.716567f, -0.681718f }, 
{ -0.309017f,  0.500000f, -0.809017f }, 
{  0.000000f,  0.525731f, -0.850651f }, 
{ -0.525731f,  0.000000f, -0.850651f }, 
{ -0.442863f,  0.238856f, -0.864188f }, 
{ -0.295242f,  0.000000f, -0.955423f }, 
{ -0.162460f,  0.262866f, -0.951056f }, 
{  0.000000f,  0.000000f, -1.000000f }, 
{  0.295242f,  0.000000f, -0.955423f }, 
{  0.162460f,  0.262866f, -0.951056f }, 
{ -0.442863f, -0.238856f, -0.864188f }, 
{ -0.309017f, -0.500000f, -0.809017f }, 
{ -0.162460f, -0.262866f, -0.951056f }, 
{  0.000000f, -0.850651f, -0.525731f }, 
{ -0.147621f, -0.716567f, -0.681718f }, 
{  0.147621f, -0.716567f, -0.681718f }, 
{  0.000000f, -0.525731f, -0.850651f }, 
{  0.309017f, -0.500000f, -0.809017f }, 
{  0.442863f, -0.238856f, -0.864188f }, 
{  0.162460f, -0.262866f, -0.951056f }, 
{  0.238856f, -0.864188f, -0.442863f }, 
{  0.500000f, -0.809017f, -0.309017f }, 
{  0.425325f, -0.688191f, -0.587785f }, 
{  0.716567f, -0.681718f, -0.147621f }, 
{  0.688191f, -0.587785f, -0.425325f }, 
{  0.587785f, -0.425325f, -0.688191f }, 
{  0.000000f, -0.955423f, -0.295242f }, 
{  0.000000f, -1.000000f,  0.000000f }, 
{  0.262866f, -0.951056f, -0.162460f }, 
{  0.000000f, -0.850651f,  0.525731f }, 
{  0.000000f, -0.955423f,  0.295242f }, 
{  0.238856f, -0.864188f,  0.442863f }, 
{  0.262866f, -0.951056f,  0.162460f }, 
{  0.500000f, -0.809017f,  0.309017f }, 
{  0.716567f, -0.681718f,  0.147621f }, 
{  0.525731f, -0.850651f,  0.000000f }, 
{ -0.238856f, -0.864188f, -0.442863f }, 
{ -0.500000f, -0.809017f, -0.309017f }, 
{ -0.262866f, -0.951056f, -0.162460f }, 
{ -0.850651f, -0.525731f,  0.000000f }, 
{ -0.716567f, -0.681718f, -0.147621f }, 
{ -0.716567f, -0.681718f,  0.147621f }, 
{ -0.525731f, -0.850651f,  0.000000f }, 
{ -0.500000f, -0.809017f,  0.309017f }, 
{ -0.238856f, -0.864188f,  0.442863f }, 
{ -0.262866f, -0.951056f,  0.162460f }, 
{ -0.864188f, -0.442863f,  0.238856f }, 
{ -0.809017f, -0.309017f,  0.500000f }, 
{ -0.688191f, -0.587785f,  0.425325f }, 
{ -0.681718f, -0.147621f,  0.716567f }, 
{ -0.442863f, -0.238856f,  0.864188f }, 
{ -0.587785f, -0.425325f,  0.688191f }, 
{ -0.309017f, -0.500000f,  0.809017f }, 
{ -0.147621f, -0.716567f,  0.681718f }, 
{ -0.425325f, -0.688191f,  0.587785f }, 
{ -0.162460f, -0.262866f,  0.951056f }, 
{  0.442863f, -0.238856f,  0.864188f }, 
{  0.162460f, -0.262866f,  0.951056f }, 
{  0.309017f, -0.500000f,  0.809017f }, 
{  0.147621f, -0.716567f,  0.681718f }, 
{  0.000000f, -0.525731f,  0.850651f }, 
{  0.425325f, -0.688191f,  0.587785f }, 
{  0.587785f, -0.425325f,  0.688191f }, 
{  0.688191f, -0.587785f,  0.425325f }, 
{ -0.955423f,  0.295242f,  0.000000f }, 
{ -0.951056f,  0.162460f,  0.262866f }, 
{ -1.000000f,  0.000000f,  0.000000f }, 
{ -0.850651f,  0.000000f,  0.525731f }, 
{ -0.955423f, -0.295242f,  0.000000f }, 
{ -0.951056f, -0.162460f,  0.262866f }, 
{ -0.864188f,  0.442863f, -0.238856f }, 
{ -0.951056f,  0.162460f, -0.262866f }, 
{ -0.809017f,  0.309017f, -0.500000f }, 
{ -0.864188f, -0.442863f, -0.238856f }, 
{ -0.951056f, -0.162460f, -0.262866f }, 
{ -0.809017f, -0.309017f, -0.500000f }, 
{ -0.681718f,  0.147621f, -0.716567f }, 
{ -0.681718f, -0.147621f, -0.716567f }, 
{ -0.850651f,  0.000000f, -0.525731f }, 
{ -0.688191f,  0.587785f, -0.425325f }, 
{ -0.587785f,  0.425325f, -0.688191f }, 
{ -0.425325f,  0.688191f, -0.587785f }, 
{ -0.425325f, -0.688191f, -0.587785f }, 
{ -0.587785f, -0.425325f, -0.688191f }, 
{ -0.688191f, -0.587785f, -0.425325f }
};

Mesh* LoadMD2(FilePtr file,Entity* parent_ent){
	if (file->ReadInt()!=844121161) return 0;
	if (file->ReadInt()!=8) return 0;

	Mesh* mesh=Mesh::CreateMesh(parent_ent);
	Surface* surf = mesh->CreateSurface();

	int skinwidth=file->ReadInt(); 
	int skinheight=file->ReadInt();
	//int framesize=
	file->ReadInt();
	//int num_skins=
	file->ReadInt();
	int num_vertices=file->ReadInt();
	int num_st=file->ReadInt();
	int num_tris=file->ReadInt();
	//int num_glcmds=
	file->ReadInt();
	int num_frames=file->ReadInt();
	//int offset_skins=
	file->ReadInt();
	int offset_st=file->ReadInt();
	int offset_tris=file->ReadInt();
	int offset_frames=file->ReadInt();
	//int offset_glcmds=
	file->ReadInt();
	//int offset_end=
	file->ReadInt();
	//string skin[num_skins];
	float* st=new float[num_st*2];
	surf->vert_tex_coords0.reserve(num_vertices*2);
	surf->no_verts=num_vertices;

	Surface* anim_surf=new Surface();

	mesh->anim_surf_list.push_back(anim_surf);
	anim_surf->no_verts=surf->no_verts;

	anim_surf->vert_coords.resize(num_vertices*3,0);

	/*unsigned short tri[num_tris*6];
	Stream->SeekFile(stream, offset_skins);
	For Local i=0 To num_skins-1
	skin[i]=ReadString(stream,64)
	Next*/
	file->SeekFile(offset_st);
	for(int i=0;i<num_st;i++){
		st[(i*2)+0]=(float)file->ReadShort()/(float)skinwidth;
		st[(i*2)+1]=(float)file->ReadShort()/(float)skinheight;
	}

	file->SeekFile(offset_tris);
	for(int i=0;i<num_tris;i++){
		int v0=file->ReadShort();
		int v1=file->ReadShort();
		int v2=file->ReadShort();
		int tc0=file->ReadShort();
		int tc1=file->ReadShort();
		int tc2=file->ReadShort();

		surf->AddTriangle(v2,v1,v0);
		surf->VertexTexCoords(v0,st[(tc0*2)+0],st[(tc0*2)+1]);
		surf->VertexTexCoords(v1,st[(tc1*2)+0],st[(tc1*2)+1]);
		surf->VertexTexCoords(v2,st[(tc2*2)+0],st[(tc2*2)+1]);
	}


	file->SeekFile(offset_frames);
	//mesh._animator=New TVertexAnimator	
	for(int i=0;i<num_frames;i++){

		float sx=file->ReadFloat();
		float sy=file->ReadFloat();
		float sz=file->ReadFloat();

		float tx=file->ReadFloat();
		float ty=file->ReadFloat();
		float tz=file->ReadFloat();

		for(int i2=1;i2<=16;i2++){file->ReadByte();}

		for(int v=0;v<num_vertices;v++){
			float x=(float)((unsigned char)file->ReadByte())*sx+tx;
			float y=(float)((unsigned char)file->ReadByte())*sy+ty;
			float z=(float)((unsigned char)file->ReadByte())*sz+tz;

			int ni=file->ReadByte();
			surf->vert_coords.push_back(x);
			surf->vert_coords.push_back(y);
			surf->vert_coords.push_back(-z);

			surf->vert_norm.push_back(md2_anorms[ni][2]);
			surf->vert_norm.push_back(md2_anorms[ni][1]);
			surf->vert_norm.push_back(-md2_anorms[ni][0]);

			surf->vert_col.push_back(1.0);
			surf->vert_col.push_back(1.0);
			surf->vert_col.push_back(1.0);
			surf->vert_col.push_back(1.0);

			surf->vert_tex_coords1.push_back(0.0);
			surf->vert_tex_coords1.push_back(0.0);


		}
		anim_surf->vert_weight4.push_back(i);
	}
	if (num_frames>1) mesh->anim=2;
	mesh->anim_seqs_first[0]=0;
	mesh->anim_seqs_last[0]=num_frames-1;
	mesh->no_surfs=-1;


	delete st;
	return mesh;
}

Mesh* LoadMD2(string filename,Entity* parent_ent){
	FilePtr file;
	file=File::ReadFile(filename);
	if (!file) return NULL;

	return LoadMD2(file,parent_ent);
}

} // namespace loadMd2
