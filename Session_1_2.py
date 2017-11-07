### FAnToM Session
### API Version: 20140515
### Used core version:    2c6e4dc72b1c3dceb3b624eacc44d50301928abf (praktikum/visPraktikum2015)
### Used toolbox version: 72513120e56f15bb7fc83f9b6ed8f494a779bc98 (stable)

################################################################
###                  Reset GUI                               ###
################################################################
fantom.ui.setCamera( 0, fantom.ui.Camera( fantom.math.Vector3(21.5298, 16.2033, 91.2527), fantom.math.Vector3(21.2125, 16.0431, 90.318), fantom.math.Vector3(0.079411, 0.977674, -0.194544), 1, 3.46833e-310 ) )
fantom.ui.setCamera( 1, fantom.ui.Camera( fantom.math.Vector3(0, 3.8637, 0), fantom.math.Vector3(0, 2.8637, 0), fantom.math.Vector3(0, 0, 1), 0, 1 ) )
fantom.ui.setCamera( 2, fantom.ui.Camera( fantom.math.Vector3(0, 0, 3.8637), fantom.math.Vector3(0, 0, 2.8637), fantom.math.Vector3(0, 1, 0), 0, 1 ) )
fantom.ui.setCamera( 3, fantom.ui.Camera( fantom.math.Vector3(3.8637, 0, 0), fantom.math.Vector3(2.8637, 0, 0), fantom.math.Vector3(0, 0, 1), 0, 1 ) )

fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 0, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 1, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 2, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 3, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 4, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )
fantom.ui.setClippingPlane( fantom.ui.ClippingPlane( 5, fantom.math.Matrix4( (1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) ), False ) )

fantom.ui.setBackgroundColor( fantom.math.Color(0, 0, 0, 1) )

fantom.ui.setRotationCenter( fantom.ui.RotationCenter( fantom.math.Vector3(0, 0, 0), True, True, True ) )


################################################################
###                  Create algorithms                       ###
################################################################
VisPraktikum_Aufgabe1 = fantom.makeAlgorithm("VisPraktikum/Aufgabe 1")
VisPraktikum_Aufgabe1.setName( "VisPraktikum/Aufgabe 1" )
VisPraktikum_Aufgabe1.setOption("Number of Houses", 4)
VisPraktikum_Aufgabe1.setOption("Size", 3)
fantom.ui.setAlgorithmPosition(VisPraktikum_Aufgabe1, fantom.math.Vector2(0, 35))

VisPraktikum_Aufgabe2 = fantom.makeAlgorithm("VisPraktikum/Aufgabe 2")
VisPraktikum_Aufgabe2.setName( "VisPraktikum/Aufgabe 2" )
VisPraktikum_Aufgabe2.setOption("Color", fantom.math.Color(0.75, 0.75, 0, 1))
VisPraktikum_Aufgabe2.setOption("Show single cell", False)
VisPraktikum_Aufgabe2.setOption("Number of cell", 4)
fantom.ui.setAlgorithmPosition(VisPraktikum_Aufgabe2, fantom.math.Vector2(0, 121))
VisPraktikum_Aufgabe2.setVisualOutputVisible('City', True)



################################################################
###                     Make Connections                     ###
################################################################
VisPraktikum_Aufgabe1.connect("grid", VisPraktikum_Aufgabe2, "Grid")


################################################################
###                      Run algorithms                      ###
################################################################
fantom.scheduleAllNecessary()