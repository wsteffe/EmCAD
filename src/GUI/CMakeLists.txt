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


if(${QT_VERSION} EQUAL 4)
 include ( ${QT_USE_FILE} )
endif(${QT_VERSION} EQUAL 4)

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${OCC_INCLUDES}
	${QWT_INCLUDE_DIR}
	${PYTHON_INCLUDE_PATH}
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
	CAD DataBase PostProc GEOMAlgo
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
	CAD DataBase PostProc GEOMAlgo
	optimized ${OCC_LIBRARIES}
	debug ${OCC_LIBRARIES_DEB}
	optimized ${QWT_LIBRARY}
	debug ${QWT_LIBRARY_DEB}
	${OCC_3PARTIES_LIBRARIES}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	Qt5::Core 
        Qt5::Gui 
        Qt5::OpenGL
     )
  else(MSVC)
    target_link_libraries (emcad 
	CAD DataBase PostProc GEOMAlgo
	${OCC_LIBRARIES}
	${QWT_LIBRARY}
	${OCC_3PARTIES_LIBRARIES}
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	Qt5::Core 
        Qt5::Gui 
        Qt5::OpenGL
     )
  endif(MSVC)
endif(${QT_VERSION} EQUAL 5)

if(MSVC)
 SET_TARGET_PROPERTIES(emcad PROPERTIES LINK_FLAGS ${MINGW_LINK_FLAGS})
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Create target for the parser
 ADD_CUSTOM_TARGET(prjdataparser echo "Creating projectData parser")

# Create custom command for flex/lex (note the outputs)
 ADD_CUSTOM_COMMAND(
   SOURCE  projectData.l
   COMMAND ${FLEX_EXECUTABLE} 
   ARGS -o ${CMAKE_CURRENT_SOURCE_DIR}/projectData_lexer.cpp
	-P projectData
         ${CMAKE_CURRENT_SOURCE_DIR}/projectData.l
   TARGET ModelLexer
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/projectData_lexer.cpp
 )

# Create custom command for bison/yacc (note the DEPENDS)
 ADD_CUSTOM_COMMAND(
   SOURCE projectData.y
   COMMAND ${BISON_EXECUTABLE}
   ARGS -d ${CMAKE_CURRENT_SOURCE_DIR}/projectData.y
        --output ${CMAKE_CURRENT_SOURCE_DIR}/projectData_parser.cpp
        -p projectData
   TARGET ModelParser
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/projectData_lexer.cpp
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/projectData_parser.cpp
 )

 SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/projectData_parser.cpp GENERATED)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


#SET_TARGET_PROPERTIES(emcad PROPERTIES LINK_FLAGS "-static")

install(TARGETS emcad  DESTINATION bin)


if(WIN32)
add_custom_target(tar_utils ALL
   SOURCES  setup.py
   COMMAND C:/Python34/python setup.py build
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(UTILS_EXE_FILES
	build/exe.win-amd64-3.4/remove_dir.exe
	build/exe.win-amd64-3.4/tar_extract.exe
	build/exe.win-amd64-3.4/tar_create.exe
)
set(UTILS_PYD_FILES
	build/exe.win-amd64-3.4/_bz2.pyd
	build/exe.win-amd64-3.4/_hashlib.pyd
	build/exe.win-amd64-3.4/_lzma.pyd
	build/exe.win-amd64-3.4/unicodedata.pyd
)
set(UTILS_ZIP_FILES
	build/exe.win-amd64-3.4/library.zip
)
set(UTILS_DLL_FILES
	build/exe.win-amd64-3.4/python34.dll
)
foreach(EXE ${UTILS_EXE_FILES})
install(PROGRAMS  ${EXE}
       	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(EXE)

foreach(PYD ${UTILS_PYD_FILES})
install(PROGRAMS  ${PYD}
       	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(PYD)

foreach(DLL ${UTILS_DLL_FILES})
install(PROGRAMS  ${DLL}
	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(DLL)

foreach(ZIP ${UTILS_ZIP_FILES})
install(PROGRAMS  ${ZIP}
       	DESTINATION utils
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(ZIP)


else(WIN32)

install(PROGRAMS tar_extract.py
       	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  tar_create.py
       	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS remove_dir.py
       	DESTINATION utils
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

endif(WIN32)

