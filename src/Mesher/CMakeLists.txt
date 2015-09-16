# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015  Walter Steffe`
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${OCC_INCLUDES}
	${GMSH_INCLUDES}
	.
	../DataBase
	../CAD)

set ( Mesher_HDRS
	mesher.h
	ocaf.h
	)


set ( Mesher_SRCS
	mesher.cpp
	output.cpp
	)

add_library ( Mesher STATIC ${Mesher_SRCS}  )


set ( EMCAD_MESH_SRCS
	   emcad_mesh.cpp
    )

add_executable ( emcad_mesh ${EMCAD_MESH_SRCS})

  if(MSVC)
     target_link_libraries ( 
	emcad_mesh
	Mesher CAD DataBase GEOMAlgo
	optimized ${GMSH_LIBRARY}
	debug ${GMSH_LIBRARY_DEB}
	optimized ${LAPACK_LIBRARIES}
	debug ${LAPACK_LIBRARIES_DEB}
	optimized ${GMSH_DEPS_LIBRARIES}
	debug ${GMSH_DEPS_LIBRARIES_DEB}
        optimized ${OCC_LIBRARIES}
	debug   ${OCC_LIBRARIES_DEB}
	${OCC_3PARTIES_LIBRARIES}
	${GMSH_EXTERNAL_LIBRARIES}
	${Boost_LIBRARIES}
      )
  elseif(MINGW)
     target_link_libraries ( 
	emcad_mesh 
	Mesher CAD DataBase GEOMAlgo
	${GMSH_LIBRARY}
	${LAPACK_LIBRARIES}
	${GMSH_DEPS_LIBRARIES}
        ${OCC_LIBRARIES}
	${OCC_3PARTIES_LIBRARIES}
	${GMSH_EXTERNAL_LIBRARIES}
	${MINGW_LIBRARIES}
	${Boost_LIBRARIES}
      )
  else(MSVC)
     target_link_libraries ( 
	emcad_mesh 
	Mesher CAD DataBase GEOMAlgo
	${GMSH_LIBRARY}
	${LAPACK_LIBRARIES}
	${GMSH_DEPS_LIBRARIES}
        ${OCC_LIBRARIES}
	${OCC_3PARTIES_LIBRARIES}
	${GMSH_EXTERNAL_LIBRARIES}
	${Boost_LIBRARIES}
      )
  endif(MSVC)

  if(MSVC)
   SET_TARGET_PROPERTIES(emcad_mesh PROPERTIES LINK_FLAGS ${MINGW_LINK_FLAGS})
  endif()

install(TARGETS emcad_mesh  DESTINATION bin)

