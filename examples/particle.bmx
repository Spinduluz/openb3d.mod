' particle.bmx

Strict

Framework Openb3d.B3dglgraphics
Import Brl.Random

Graphics3D 800,600


ClearTextureFilters

Local camera:TCamera=CreateCamera()
CameraClsColor camera,100,150,200

Local light:TLight=CreateLight()
TurnEntity light,45,45,0

Local pivot:TPivot=CreatePivot()
PositionEntity pivot,0,2,0

Local t_sphere:TMesh=CreateSphere(8)
EntityShininess t_sphere,0.2

Local spheres:TEntity[]=New TEntity[20]
Local scount=0;

For Local sp:TEntity=EachIn spheres
	sp=Null
Next

Local lastsphere:TEntity
For Local t%=0 To 359 Step 36
	Local sphere:TEntity=CopyEntity(t_sphere,pivot)
	EntityColor sphere,Rnd(255),Rnd(255),Rnd(255)
	TurnEntity sphere,0,t,0
	MoveEntity sphere,0,2,10
	spheres[scount]=sphere
	lastsphere=sphere
	scount:+1
Next
'FreeEntity t_sphere
DebugLog "Count="+scount

Local cube:TMesh=CreateCube()
PositionEntity cube,0,7,0
ScaleEntity cube,2,2,2
EntityColor cube,Rnd(255),Rnd(255),Rnd(255)

'Local ground:TMesh=CreatePlane(128)
Local ground:TMesh=CreateCube()
ScaleEntity ground,1000,1,1000
Local ground_tex:TTexture=LoadTexture("media/sand.bmp")
ScaleTexture ground_tex,0.01,0.01 ' scale uvs
EntityTexture ground,ground_tex

Local cone:TMesh=CreateCone()
PositionEntity cone,25,3,25

Local sprite:TSprite=CreateSprite()
EntityColor sprite,250,250,150
'ScaleSprite sprite,1.5,1.5
EntityAlpha sprite,0.2
Local noisetex:TTexture=LoadTexture("media/smoke.png",1+2)
'Local noisetex:TTexture=CreateTexture(1,1)
EntityTexture sprite,noisetex

'SpriteRenderMode sprite,3 ' 1: normal, 2: sprite batch, 3: particle batch?
'ParticleTrail sprite,20 ' particle trail for particle
'ParticleColor sprite,1.0,0,0,1.0 ' vertex color?

Local p:TParticleEmitter=CreateParticleEmitter(sprite)
MoveEntity p,25,5,25
EmitterRate p,1.0 ' rate between each emission (1.0/r) smaller is less
EmitterParticleLife p,175 ' lifespan of particle, in frames
EmitterVariance p,0.07 ' randomness of emission, direction and speed
EmitterParticleSpeed p,0.01 ' initial speed?
EmitterVector p,0.001,0.001,0 ' emission direction and speed?

Local p2:TParticleEmitter=TParticleEmitter( CopyEntity(p) )

PositionEntity sprite,-25,6,25
PositionEntity camera,0,7,0
MoveEntity camera,0,0,-20


While Not KeyHit(KEY_ESCAPE)
	
	' control camera
	MoveEntity camera,KeyDown(KEY_D)-KeyDown(KEY_A),0,KeyDown(KEY_W)-KeyDown(KEY_S)
	TurnEntity camera,KeyDown(KEY_DOWN)-KeyDown(KEY_UP),KeyDown(KEY_LEFT)-KeyDown(KEY_RIGHT),0
	
	TurnEntity cube,0.1,0.2,0.3
	TurnEntity pivot,0,1,0
	
	PositionEntity p2,EntityX(lastsphere,1),5,EntityZ(lastsphere,1)

	UpdateWorld ' update particles
	
	RenderWorld
	
	Flip
Wend

FreeEntity t_sphere
For Local sp:TMesh=EachIn spheres
	If sp FreeEntity sp
Next
DebugLog "Pivot" 	; FreeEntity pivot
DebugLog "Sprite" 	; FreeEntity sprite
DebugLog "Camera" 	; FreeEntity camera
DebugLog "light" 	; FreeEntity light
DebugLog "Cube"		; FreeEntity cube
DebugLog "Cone"		; FreeEntity cone
DebugLog "Ground"	; FreeEntity ground
DebugLog "Particle" ; FreeEntity p2
DebugLog "Particle" ; FreeEntity p

FreeTexture noisetex
FreeTexture ground_tex
