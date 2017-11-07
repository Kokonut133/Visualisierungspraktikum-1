### FAnToM Session
### API Version: 20140515
### Used core version:    2c6e4dc72b1c3dceb3b624eacc44d50301928abf (praktikum/visPraktikum2015)
### Used toolbox version: 72513120e56f15bb7fc83f9b6ed8f494a779bc98 (stable)

################################################################
###                  Reset GUI                               ###
################################################################
fantom.ui.setCamera( 0, fantom.ui.Camera( fantom.math.Vector3(-92.33, 14.3506, 17.9543), fantom.math.Vector3(-91.3825, 14.1976, 17.6737), fantom.math.Vector3(0.148427, 0.988205, -0.0376872), 1, 3.45959e-310 ) )
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
Load_VTK = fantom.makeAlgorithm("Load/VTK")
Load_VTK.setName( "Load/VTK" )
Load_VTK.setOption("Input File", "/home/visprak11/fantom/TestData/markPosScalar1.vtk")
Load_VTK.setOption("Big Endian", True)
Load_VTK.setOption("Dimension", "2D if third component is zero")
Load_VTK.setOption("Time List", "")
fantom.ui.setAlgorithmPosition(Load_VTK, fantom.math.Vector2(0, 35))

VisPraktikum_Aufgabe3 = fantom.makeAlgorithm("VisPraktikum/Aufgabe 3")
VisPraktikum_Aufgabe3.setName( "VisPraktikum/Aufgabe 3" )
VisPraktikum_Aufgabe3.setOption("Color", fantom.math.Color(0.75, 0.75, 0, 1))
VisPraktikum_Aufgabe3.setOption("Threshold", 0.0008)
VisPraktikum_Aufgabe3.setOption("Radius", 0.15)
VisPraktikum_Aufgabe3.setOption("Scale", True)
fantom.ui.setAlgorithmPosition(VisPraktikum_Aufgabe3, fantom.math.Vector2(0, 163))
VisPraktikum_Aufgabe3.setVisualOutputVisible('Spheres', True)



################################################################
###                     Make Connections                     ###
################################################################
Load_VTK.connect("Fields", VisPraktikum_Aufgabe3, "Field")


################################################################
###                      Run algorithms                      ###
################################################################
fantom.scheduleAllNecessary()