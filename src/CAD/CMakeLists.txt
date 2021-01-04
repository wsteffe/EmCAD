# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015-2020  Walter Steffe`
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
	${Boost_INCLUDE_DIR}
	.
	../DataBase
	../GUI
	)
set ( CAD_HDRS
          ocaf.h
          InputOutput.h
	  projectSetting.h
	)

set ( CAD_SRCS
	ocaf.cpp
	InputOutput.cpp
	gnulib_flock.cpp
	)

set ( OStools_SRCS
	OStools.cpp
	)

add_library ( CAD STATIC  ${CAD_SRCS}  )
add_library ( OStools STATIC  ${OStools_SRCS}  )



#-------------------------


set ( EMCAD_IMPRINT_SRCS
	emcad_imprint.cpp
	)

add_executable ( emcad_imprint ${EMCAD_IMPRINT_SRCS})

  if(MSVC)
      target_link_libraries ( 
	emcad_imprint
	CAD OStools DataBase GEOMAlgo ShHealOper
        optimized ${OCC_LIBRARIES}
	debug   ${OCC_LIBRARIES_DEB}
	${OCC_3PARTIES_LIBRARIES}
	${Boost_LIBRARIES}
       )
  else(MSVC)
      target_link_libraries ( 
	emcad_imprint 
	CAD OStools DataBase 
        ${OCC_LIBRARIES}
	${OCC_3PARTIES_LIBRARIES}
	${Boost_LIBRARIES}
       )
  endif(MSVC)


#SET_TARGET_PROPERTIES(emcad_mesher PROPERTIES LINK_FLAGS "-static")

install(TARGETS emcad_imprint  DESTINATION bin)

if(MSVC)
 SET_TARGET_PROPERTIES(emcad_imprint PROPERTIES LINK_FLAGS ${MINGW_LINK_FLAGS})
endif()


set ( EMCAD_SPLIT_SRCS
	emcad_split.cpp
    )

add_executable ( emcad_split ${EMCAD_SPLIT_SRCS})

if(MSVC)
   target_link_libraries ( 
    emcad_split CAD OStools DataBase
    optimized ${OCC_LIBRARIES}
    debug   ${OCC_LIBRARIES_DEB}
    ${OCC_3PARTIES_LIBRARIES}
    ${Boost_LIBRARIES}
   )
else(MSVC)
   target_link_libraries ( 
    emcad_split CAD OStools DataBase
    ${OCC_LIBRARIES}
    ${OCC_3PARTIES_LIBRARIES}
    ${Boost_LIBRARIES}
   )
endif(MSVC)

if(MSVC)
 SET_TARGET_PROPERTIES(emcad_split PROPERTIES LINK_FLAGS ${MINGW_LINK_FLAGS})
endif()

#SET_TARGET_PROPERTIES(emcad_mesher PROPERTIES LINK_FLAGS "-static")

install(TARGETS emcad_split  DESTINATION bin)


