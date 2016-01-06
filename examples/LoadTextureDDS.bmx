Strict

Framework Openb3d.B3dglgraphics

Graphics3D 800,600,0,2


Local camera:TCamera=CreateCamera()
PositionEntity camera,0,2,0
MoveEntity camera,0,0,-7

Local light:TLight=CreateLight()
TurnEntity light,45,45,0

Local dxt1:TMesh=CreateCube()
Local dxt3:TMesh=CreateCube()
Local dxt5:TMesh=CreateCube()
Local rgba:TMesh=CreateCube()

PositionEntity dxt1,-5,0,0
PositionEntity dxt3, 0,0,0
PositionEntity dxt5, 5,0,0
PositionEntity rgba, 0,5,0

Local dxt1_tex:TTexture=LoadTexture("media/dxt1.dds")
Local dxt3_tex:TTexture=LoadTexture("media/dxt3.dds")
Local dxt5_tex:TTexture=LoadTexture("media/dxt5.dds")
Local rgba_tex:TTexture=LoadTexture("media/dds_rgba.dds") 'Unomcpressed DDS

EntityTexture dxt1,dxt1_tex
EntityTexture dxt3,dxt3_tex
EntityTexture dxt5,dxt5_tex
EntityTexture rgba,rgba_tex

While Not KeyHit(KEY_ESCAPE)
	TurnEntity dxt1,0,1,0
	TurnEntity dxt3,0,1,0
	TurnEntity dxt5,0,1,0
	TurnEntity rgba,0,1,0
	
	UpdateWorld
	RenderWorld

	Flip
Wend
End

