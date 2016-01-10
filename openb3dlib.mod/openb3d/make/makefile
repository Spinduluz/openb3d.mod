CCMPILER = g++
CC=g++ -Wall -O3 -std=c++11 

OBJECTS = model.o 			\
		  string_helper.o 	\
		  surface.o bank.o 	\
		  bone.o 			\
		  brush.o 			\
		  collision.o 		\
		  entity.o 			\
		  functions.o 		\
		  geom.o 			\
		  global.o 			\
		  light.o 			\
		  matrix.o 			\
		  pick.o 			\
		  pivot.o 			\
		  project.o 		\
		  sprite.o 			\
		  tilt.o 			\
		  touch.o 			\
		  texture_filter.o 	\
		  tree.o 			\
		  animation.o 		\
		  animation_keys.o 	\
		  collision2.o 		\
		  maths_helper.o 	\
		  quaternion.o 		\
		  sprite_batch.o 	\
		  camera.o 			\
		  file.o 			\
		  mesh.o 			\
		  texture.o 		\
		  terrain.o 		\
		  stb_image.o 		\
		  3ds.o 			\
		  collidetri.o 		\
		  shadow.o 			\
		  material.o 		\
		  stencil.o 		\
		  csg.o 			\
		  voxel.o 			\
		  x.o 				\
		  octree.o 			\
		  geosphere.o 		\
		  md2.o 			\
		  isosurface.o 		\
		  particle.o 		\
		  physics.o 		\
		  actions.o 		\
		  dds.o 			\
		  fileresource.o	\
		  programobject.o	\
		  shaderobject.o	\
		  unzip.o 			\
		  ioapi.o

CFLAGS=-Iglew/GL -Ilib -I../src -Iglew -DGLEW_STATIC -DBUILD_DLL
	
SRCDIR=../src/

main:$(OBJECTS) 
ifeq ($(shell uname), Linux)
	$(CC) -shared -o libOpenB3D.so $(OBJECTS) -lGL -lGLU
else
	$(CC) -shared -o openb3d.dll $(OBJECTS) -Llib/win32 -lz -lglew32 -lopengl32 -lglu32 -lGDI32
endif


3ds.o  : $(SRCDIR)3ds.cpp $(SRCDIR)3ds.h
	$(CC) -c $(SRCDIR)3ds.cpp -o 3ds.o $(CFLAGS)

md2.o  : $(SRCDIR)md2.cpp $(SRCDIR)md2.h
	$(CC) -c $(SRCDIR)md2.cpp -o md2.o $(CFLAGS)

actions.o  : $(SRCDIR)actions.cpp $(SRCDIR)actions.h
	$(CC) -c $(SRCDIR)actions.cpp -o actions.o $(CFLAGS)

bank.o : $(SRCDIR)bank.cpp $(SRCDIR)bank.h
	$(CC) -c $(SRCDIR)bank.cpp -o bank.o $(CFLAGS)

bone.o : $(SRCDIR)bone.cpp $(SRCDIR)bone.h
	$(CC) -c $(SRCDIR)bone.cpp -o bone.o $(CFLAGS)

brush.o : $(SRCDIR)brush.cpp $(SRCDIR)brush.h
	$(CC) -c $(SRCDIR)brush.cpp -o brush.o $(CFLAGS)

camera.o : $(SRCDIR)camera.cpp $(SRCDIR)camera.h
	$(CC) -c $(SRCDIR)camera.cpp -o camera.o $(CFLAGS)

collision.o : $(SRCDIR)collision.cpp $(SRCDIR)collision.h
	$(CC) -c $(SRCDIR)collision.cpp -o collision.o $(CFLAGS)

entity.o : $(SRCDIR)entity.cpp $(SRCDIR)entity.h
	$(CC) -c $(SRCDIR)entity.cpp -o entity.o $(CFLAGS)

file.o : $(SRCDIR)file.cpp $(SRCDIR)file.h
	$(CC) -c $(SRCDIR)file.cpp -o file.o $(CFLAGS)

functions.o : $(SRCDIR)functions.cpp $(SRCDIR)functions.h
	$(CC) -c $(SRCDIR)functions.cpp -o functions.o	 $(CFLAGS)

geom.o : $(SRCDIR)geom.cpp $(SRCDIR)geom.h
	$(CC) -c $(SRCDIR)geom.cpp -o geom.o $(CFLAGS)

geosphere.o : $(SRCDIR)geosphere.cpp $(SRCDIR)geosphere.h
	$(CC) -c $(SRCDIR)geosphere.cpp -o geosphere.o $(CFLAGS)

global.o : $(SRCDIR)global.cpp $(SRCDIR)global.h
	$(CC) -c $(SRCDIR)global.cpp -o global.o $(CFLAGS)

light.o : $(SRCDIR)light.cpp $(SRCDIR)light.h
	$(CC) -c $(SRCDIR)light.cpp -o light.o $(CFLAGS)

matrix.o : $(SRCDIR)matrix.cpp $(SRCDIR)matrix.h
	$(CC) -c $(SRCDIR)matrix.cpp -o matrix.o $(CFLAGS)

mesh.o : $(SRCDIR)mesh.cpp $(SRCDIR)mesh.h
	$(CC) -c $(SRCDIR)mesh.cpp -o mesh.o $(CFLAGS)

model.o : $(SRCDIR)model.cpp $(SRCDIR)model.h
	$(CC) -c $(SRCDIR)model.cpp -o model.o $(CFLAGS)

pick.o : $(SRCDIR)pick.cpp $(SRCDIR)pick.h
	$(CC) -c $(SRCDIR)pick.cpp -o pick.o $(CFLAGS)

pivot.o : $(SRCDIR)pivot.cpp $(SRCDIR)pivot.h
	$(CC) -c $(SRCDIR)pivot.cpp -o pivot.o $(CFLAGS)

project.o : $(SRCDIR)project.cpp $(SRCDIR)project.h
	$(CC) -c $(SRCDIR)project.cpp -o project.o $(CFLAGS)

sprite.o : $(SRCDIR)sprite.cpp $(SRCDIR)sprite.h
	$(CC) -c $(SRCDIR)sprite.cpp -o sprite.o $(CFLAGS)

surface.o : $(SRCDIR)surface.cpp $(SRCDIR)surface.h
	$(CC) -c $(SRCDIR)surface.cpp -o surface.o $(CFLAGS)

terrain.o : $(SRCDIR)terrain.cpp $(SRCDIR)terrain.h $(SRCDIR)stb_image.c
	$(CC) -c $(SRCDIR)terrain.cpp -o terrain.o $(CFLAGS)

texture.o : $(SRCDIR)texture.cpp $(SRCDIR)texture.h $(SRCDIR)stb_image.c $(SRCDIR)dds.cpp
	$(CC) -c $(SRCDIR)texture.cpp -o texture.o $(CFLAGS)

tilt.o : $(SRCDIR)tilt.cpp $(SRCDIR)tilt.h
	$(CC) -c $(SRCDIR)tilt.cpp -o tilt.o $(CFLAGS)

touch.o : $(SRCDIR)touch.cpp $(SRCDIR)touch.h
	$(CC) -c $(SRCDIR)touch.cpp -o touch.o $(CFLAGS)

string_helper.o : $(SRCDIR)string_helper.cpp $(SRCDIR)string_helper.h
	$(CC) -c $(SRCDIR)string_helper.cpp -o string_helper.o $(CFLAGS)

texture_filter.o : $(SRCDIR)texture_filter.cpp $(SRCDIR)texture_filter.h
	$(CC) -c $(SRCDIR)texture_filter.cpp -o texture_filter.o $(CFLAGS)

tree.o : $(SRCDIR)tree.cpp $(SRCDIR)tree.h
	$(CC) -c $(SRCDIR)tree.cpp -o tree.o $(CFLAGS)

animation.o : $(SRCDIR)animation.cpp $(SRCDIR)animation.h
	$(CC) -c $(SRCDIR)animation.cpp -o animation.o $(CFLAGS)

animation_keys.o : $(SRCDIR)animation_keys.cpp $(SRCDIR)animation_keys.h
	$(CC) -c $(SRCDIR)animation_keys.cpp -o animation_keys.o $(CFLAGS)

collision2.o : $(SRCDIR)collision2.cpp $(SRCDIR)collision2.h
	$(CC) -c $(SRCDIR)collision2.cpp -o collision2.o $(CFLAGS)

material.o : $(SRCDIR)material.cpp 
	$(CC) -c $(SRCDIR)material.cpp -o material.o $(CFLAGS)

maths_helper.o : $(SRCDIR)maths_helper.cpp $(SRCDIR)maths_helper.h
	$(CC) -c $(SRCDIR)maths_helper.cpp -o maths_helper.o $(CFLAGS)

particle.o : $(SRCDIR)particle.cpp $(SRCDIR)particle.h
	$(CC) -c $(SRCDIR)particle.cpp -o particle.o $(CFLAGS)

physics.o : $(SRCDIR)physics.cpp $(SRCDIR)physics.h
	$(CC) -c $(SRCDIR)physics.cpp -o physics.o $(CFLAGS)

quaternion.o : $(SRCDIR)quaternion.cpp $(SRCDIR)quaternion.h
	$(CC) -c $(SRCDIR)quaternion.cpp -o quaternion.o $(CFLAGS)

shadow.o : $(SRCDIR)shadow.cpp $(SRCDIR)shadow.h
	$(CC) -c $(SRCDIR)shadow.cpp -o shadow.o $(CFLAGS)

sprite_batch.o : $(SRCDIR)sprite_batch.cpp $(SRCDIR)sprite_batch.h
	$(CC) -c $(SRCDIR)sprite_batch.cpp -o sprite_batch.o $(CFLAGS)

stb_image.o	: $(SRCDIR)stb_image.c
	gcc -c $(SRCDIR)stb_image.c -o stb_image.o $(CFLAGS)

stencil.o : $(SRCDIR)stencil.cpp $(SRCDIR)stencil.h
	$(CC) -c $(SRCDIR)stencil.cpp -o stencil.o $(CFLAGS)

csg.o : $(SRCDIR)csg.cpp $(SRCDIR)csg.h
	$(CC) -c $(SRCDIR)csg.cpp -o csg.o $(CFLAGS)

isosurface.o : $(SRCDIR)isosurface.cpp $(SRCDIR)isosurface.h
	$(CC) -c $(SRCDIR)isosurface.cpp -o isosurface.o $(CFLAGS)

octree.o : $(SRCDIR)octree.cpp $(SRCDIR)octree.h
	$(CC) -c $(SRCDIR)octree.cpp -o octree.o $(CFLAGS)

voxel.o : $(SRCDIR)voxel.cpp $(SRCDIR)voxel.h
	$(CC) -c $(SRCDIR)voxel.cpp -o voxel.o $(CFLAGS)

x.o : $(SRCDIR)x.cpp $(SRCDIR)x.h
	$(CC) -c $(SRCDIR)x.cpp -o x.o $(CFLAGS)

collidetri.o : $(SRCDIR)collidetri.c
	gcc -c $(SRCDIR)collidetri.c -o collidetri.o $(CFLAGS)

fileresource.o : $(SRCDIR)fileresource.cpp $(SRCDIR)fileresource.h
	$(CC) -c $(SRCDIR)fileresource.cpp -o fileresource.o $(CFLAGS)

dds.o : $(SRCDIR)dds.cpp $(SRCDIR)dds.h
	$(CC) -c $(SRCDIR)dds.cpp -o dds.o $(CFLAGS)

programobject.o : $(SRCDIR)programobject.cpp $(SRCDIR)programobject.h
	$(CC) -c $(SRCDIR)programobject.cpp -o programobject.o $(CFLAGS)
	
shaderobject.o : $(SRCDIR)shaderobject.cpp $(SRCDIR)shaderobject.h
	$(CC) -c $(SRCDIR)shaderobject.cpp -o shaderobject.o $(CFLAGS)
	
unzip.o : $(SRCDIR)unzip.c $(SRCDIR)unzip.h
	gcc -c $(SRCDIR)unzip.c -o unzip.o $(CFLAGS)
	
ioapi.o : $(SRCDIR)ioapi.c $(SRCDIR)ioapi.h
	gcc -c $(SRCDIR)ioapi.c -o ioapi.o $(CFLAGS)

