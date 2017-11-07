### FAnToM Session
### API Version: 20140515
### Used core version:    2c6e4dc72b1c3dceb3b624eacc44d50301928abf (praktikum/visPraktikum2015)
### Used toolbox version: 72513120e56f15bb7fc83f9b6ed8f494a779bc98 (stable)

################################################################
###                  Reset GUI                               ###
################################################################
fantom.ui.setCamera( 0, fantom.ui.Camera( fantom.math.Vector3(-19.1839, -2.7048, 93.1547), fantom.math.Vector3(-18.8907, -2.67039, 92.1993), fantom.math.Vector3(0.0478408, -0.998628, -0.0212857), 1, 3.45959e-310 ) )
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
Load_VTK.setOption("Input File", "/home/visprak11/fantom/TestData/streamTest2.vtk")
Load_VTK.setOption("Big Endian", True)
Load_VTK.setOption("Dimension", "2D if third component is zero")
Load_VTK.setOption("Time List", "")
fantom.ui.setAlgorithmPosition(Load_VTK, fantom.math.Vector2(0, 35))

VisPraktikum_Aufgabe4 = fantom.makeAlgorithm("VisPraktikum/Aufgabe 4")
VisPraktikum_Aufgabe4.setName( "VisPraktikum/Aufgabe 4" )
VisPraktikum_Aufgabe4.setOption("Method", "Euler")
VisPraktikum_Aufgabe4.setOption("Stepwidth", 1)
VisPraktikum_Aufgabe4.setOption("Color", fantom.math.Color(0.75, 0.75, 0, 1))
VisPraktikum_Aufgabe4.setOption("Startline_start_x", -1)
VisPraktikum_Aufgabe4.setOption("Startline_start_y", 1)
VisPraktikum_Aufgabe4.setOption("Startline_start_z", 6)
VisPraktikum_Aufgabe4.setOption("Startline_end_x", -1)
VisPraktikum_Aufgabe4.setOption("Startline_end_y", 1)
VisPraktikum_Aufgabe4.setOption("Startline_end_z", 0)
VisPraktikum_Aufgabe4.setOption("Startline_3D_x", -5)
VisPraktikum_Aufgabe4.setOption("Startline_3D_y", 5)
VisPraktikum_Aufgabe4.setOption("Startline_3D_z", 6)
VisPraktikum_Aufgabe4.setOption("Number", 10)
fantom.ui.setAlgorithmPosition(VisPraktikum_Aufgabe4, fantom.math.Vector2(0, 163))
VisPraktikum_Aufgabe4.setVisualOutputVisible('Streamlines', True)

Grid_ShowGrid = fantom.makeAlgorithm("Grid/Show Grid")
Grid_ShowGrid.setName( "Grid/Show Grid" )
Grid_ShowGrid.setOption("Line color", fantom.math.Color(0, 0, 1, 1))
Grid_ShowGrid.setOption("Random jittering of color", True)
Grid_ShowGrid.setOption("Random seed", 0)
Grid_ShowGrid.setOption("Line width", 1.5)
fantom.ui.setAlgorithmPosition(Grid_ShowGrid, fantom.math.Vector2(0, 249))
Grid_ShowGrid.setVisualOutputVisible('grid', False)



################################################################
###                     Make Connections                     ###
################################################################
Load_VTK.connect("Grid", Grid_ShowGrid, "Grid")
Load_VTK.connect("Fields", VisPraktikum_Aufgabe4, "Field")


################################################################
###                      Run algorithms                      ###
################################################################
fantom.scheduleAllNecessary()