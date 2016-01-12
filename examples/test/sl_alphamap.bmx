' sl_alphamap.bmx
' using alpha maps for transparency

Strict

Framework Openb3d.B3dglgraphics

Graphics3D 800,600,0,2,60,GRAPHICS_BACKBUFFER|GRAPHICS_ALPHABUFFER|GRAPHICS_DEPTHBUFFER|GRAPHICS_STENCILBUFFER|GRAPHICS_ACCUMBUFFER|$40|$400


Local camera:TCamera=CreateCamera()

Local light:TLight=CreateLight()

Local cube:TMesh=CreateCube()
PositionEntity cube,-1.5,0,4

Local cube2:TMesh=CreateCube()
PositionEntity cube2,1.5,0,4

Local cone:TMesh=CreateCone()
PositionEntity cone,0,0,10
ScaleEntity cone,4,4,4

Local plane:TMesh=CreateCube()
ScaleEntity plane,10,0.1,10
MoveEntity plane,0,-1.5,0

Rem
' transparency - from two images
' note: more than one visible surface is needed for alpha to work
Local shader:TShader=LoadShader("","shaders/alphamap.vert.glsl","shaders/alphamap.frag.glsl")
ShaderTexture(shader,LoadTexture("media/colorkey.jpg"),"tex",0)
ShaderTexture(shader,LoadTexture("media/spark.png"),"alphatex",1)
ShadeEntity(cube,shader)
EntityFX(cube,32)

' tranlucency - from single image with alpha channel
Local shader2:TShader=LoadShader("","shaders/alphamap.vert.glsl","shaders/alphamap.frag.glsl")
ShaderTexture(shader2,LoadTexture("media/alpha_map.png"),"tex",0)
ShadeEntity(cube2,shader2)
EntityFX(cube2,32)
End Rem

Local shader:TShader=CreateShader( "shader" )
Local shader2:TShader=CreateShader( "shader2" )

Local vert:TShaderObject=LoadShaderObject( 0,"shaders/alphamap.vert.glsl" ) 'vert reference count=1
Local frag:TShaderObject=LoadShaderObject( 1,"shaders/alphamap.frag.glsl" ) 'frag reference count=1

AttachShaderObject( shader,vert ) 'vert reference count=2
AttachShaderObject( shader,frag ) 'frag reference count=2

ShaderTexture(shader,LoadTexture("media/colorkey.jpg"),"tex",0)
ShaderTexture(shader,LoadTexture("media/spark.png"),"alphatex",1)
ShadeEntity(cube,shader)
EntityFX(cube,32)

AttachShaderObject( shader2,vert ) 'vert reference count=3
AttachShaderObject( shader2,frag ) 'frag reference count=3

ShaderTexture(shader2,LoadTexture("media/alpha_map.png"),"tex",0)
ShadeEntity(cube2,shader2)
EntityFX(cube2,32)

Local efx%=1


While Not KeyDown(KEY_ESCAPE)

	' turn cubes
	If KeyDown(KEY_LEFT)
		TurnEntity cube,0,-0.5,0.1
		TurnEntity cube2,0,0.5,-0.1
	EndIf
	If KeyDown(KEY_RIGHT)
		TurnEntity cube,0,0.5,-0.1
		TurnEntity cube2,0,-0.5,0.1
	EndIf
	
	' alpha blending: alpha / nothing
	If KeyHit(KEY_B)
		efx=Not efx
		If efx
			EntityFX(cube,32) ; EntityFX(cube2,32)
		Else
			EntityFX(cube,0) ; EntityFX(cube2,0)
		EndIf
	EndIf
	
	RenderWorld
	
	Text 0,0,"Left/Right: turn cubes"+", B: alpha blending = "+efx
	
	Flip

Wend

FreeEntity light
FreeEntity camera

FreeEntity cube
FreeEntity cube2
FreeEntity cone
FreeEntity plane

'ShadeEntity_(cube,Null)
'ShadeEntity_(cube2,Null)
' Even if we free the shader objects here they
' will not be deleted until all references are deleted
FreeShaderObject vert 'vert reference count=2
FreeShaderObject frag 'frag reference count=2
FreeShader shader 'vert & frag reference count=1
FreeShader shader2 'ShaderObject(s) will be finally deleted here (vert & frag reference count=0!)
End
