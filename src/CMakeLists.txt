set(useTetMesher 0)

#add_subdirectory( dxflib )
#add_subdirectory( dxflib/test )
add_subdirectory( CAD )
add_subdirectory( DataBase )
if(${useTetMesher} EQUAL 1)
 add_subdirectory( MeshSurface )
 add_subdirectory( Triangle )
 add_subdirectory( Mefisto2 )
 add_subdirectory( Tetgen1.5 )
 add_subdirectory( TetMesher )
endif()
add_subdirectory( Mesher )
add_subdirectory( PostProc )
add_subdirectory( GUI )
add_subdirectory( pyUtils )



