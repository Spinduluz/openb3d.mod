Strict

Framework Openb3d.B3dglgraphics

Graphics3D 800,600,0,2


Local camera:TCamera=CreateCamera()
PositionEntity camera,0,2,0
MoveEntity camera,0,0,-10

Local light:TLight=CreateLight()
TurnEntity light,45,45,0

Local dxt1:TMesh=CreateCube()
Local dxt3:TMesh=CreateCube()
Local dxt5:TMesh=CreateCube()

PositionEntity dxt1,-5,0,0
PositionEntity dxt3, 0,0,0
PositionEntity dxt5, 5,0,0

Local dxt1_tex:TTexture=LoadTexture("media/tex2_dxt1.dds")
Local dxt3_tex:TTexture=LoadTexture("media/tex2_dxt3.dds")
Local dxt5_tex:TTexture=LoadTexture("media/tex2_dxt5.dds")

EntityTexture dxt1,dxt1_tex
EntityTexture dxt3,dxt3_tex
EntityTexture dxt5,dxt5_tex

While Not KeyHit(KEY_ESCAPE)

	' turn camera
	'If KeyDown(KEY_LEFT) TurnEntity pivot,0,3,0
	'If KeyDown(KEY_RIGHT) TurnEntity pivot,0,-3,0
	
	TurnEntity dxt1,0,1,0
	TurnEntity dxt3,0,1,0
	TurnEntity dxt5,0,1,0
	
	UpdateWorld
	RenderWorld
	
	Text 0,0,"Left/Right: turn boxes"
	
	Flip
Wend
