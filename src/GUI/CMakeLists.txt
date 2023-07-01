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

if(${useGmshTetrahedra} EQUAL 1)
  add_definitions(-DGMSH3D)
endif()

if(${QT_VERSION} EQUAL 4)
 include ( ${QT_USE_FILE} )
endif(${QT_VERSION} EQUAL 4)

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${OCC_INCLUDES}
	${QWT_INCLUDE_DIR}
	${PYTHON_INCLUDE_DIR}
	.
	../CAD
	../DataBase
	../PostProc
	)


set ( emcad_HDRS
	mainwindow.h
	projectData.h
	ViewWiget.h
	Timer.h
	TreeWidget.h
	)

set ( emcad_SRCS
	main.cpp
	mainwindow.cpp
	ViewWidget.cpp
	TreeWidget.cpp
	projectData_lexer.cpp
	projectData_parser.cpp

	)

set ( emcad_MOCS
	mainwindow.h
	ViewWidget.h
	TreeWidget.h
	)

if(${QT_VERSION} EQUAL 4)
 QT4_WRAP_CPP(MOCS ${emcad_MOCS})
endif(${QT_VERSION} EQUAL 4)
if(${QT_VERSION} EQUAL 5)
 QT5_WRAP_CPP(MOCS ${emcad_MOCS})
endif(${QT_VERSION} EQUAL 5)

if(WIN32)
# add_executable ( emcad WIN32 ${emcad_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} "emcad.rc")
# winres compilation is not properly set by cmake
 add_executable ( emcad WIN32 ${emcad_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
endif(WIN32)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 add_executable ( emcad ${emcad_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${QT_VERSION} EQUAL 4)
  target_link_libraries ( emcad 
	CAD OStools DataBase PostProc
	${QT_QTCORE_LIBRARY} 
	${QT_QTGUI_LIBRARY}  
	${QT_OPENGL_LIBRARY}
	${OCC_LIBRARIES}
	${OCC_3PARTIES_LIBRARIES}
	${QWT_LIBRARY}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
  )
endif(${QT_VERSION} EQUAL 4)

if(${QT_VERSION} EQUAL 5)
  if(MSVC)
    target_link_libraries (emcad 
	CAD OStools DataBase PostProc
	optimized ${OCC_LIBRARIES}
	debug ${OCC_LIBRARIES_DEB}
	${QWT_LIBRARY}
	${OCC_3PARTIES_LIBRARIES}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	Qt5::Core 
        Qt5::Gui 
        Qt5::OpenGL
	Qt5::Widgets
     )
  elseif(MINGW)
    target_link_libraries (emcad 
	CAD OStools DataBase PostProc
        "-Wl,--start-group" 
	${OCC_LIBRARIES}
	${QWT_LIBRARY}
	${OCC_3PARTIES_LIBRARIES}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	Qt5::Core 
        Qt5::Gui 
        Qt5::OpenGL
	Qt5::Widgets
        "-Wl,--end-group"
     )
  else(MSVC)
    target_link_libraries (emcad 
	CAD OStools DataBase PostProc
	${OCC_LIBRARIES}
	${QWT_LIBRARY}
	${OCC_3PARTIES_LIBRARIES}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	Qt5::Core 
        Qt5::Gui 
        Qt5::OpenGL
	Qt5::Widgets
     )
  endif(MSVC)
endif(${QT_VERSION} EQUAL 5)

if(MINGW)
 SET_TARGET_PROPERTIES(emcad PROPERTIES LINK_FLAGS ${MINGW_LINK_FLAGS})
endif()

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)

 FIND_PACKAGE(BISON)
 FIND_PACKAGE(FLEX)
 BISON_TARGET(projectDataParser ${CMAKE_CURRENT_SOURCE_DIR}/projectData.y ${CMAKE_CURRENT_SOURCE_DIR}/projectData_parser.cpp COMPILE_FLAGS "-p projectData")
 FLEX_TARGET(projectDataLexer ${CMAKE_CURRENT_SOURCE_DIR}/projectData.l ${CMAKE_CURRENT_SOURCE_DIR}/projectData_lexer.cpp COMPILE_FLAGS "-P projectData")
 ADD_FLEX_BISON_DEPENDENCY(projectDataLexer projectDataParser)

endif((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)


#SET_TARGET_PROPERTIES(emcad PROPERTIES LINK_FLAGS "-static")

install(TARGETS emcad  DESTINATION bin)
if(WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:emcad> TYPE BIN)
endif(WIN32)




