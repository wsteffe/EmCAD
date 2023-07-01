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


add_definitions ( -Wall )
if(${QT_VERSION} EQUAL 4)
 include ( ${QT_USE_FILE} )
endif(${QT_VERSION} EQUAL 4)

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QWT_INCLUDE_DIR}
	.
	../DataBase
	../GUI
	../CAD)
set ( PostProc_HDRS
	plotter.h
	plotData.h
	projectData.h
	)

set ( PostProc_SRCS
	plotter.cpp
	readSZP.cpp
        touchstone_lexer.cpp
        touchstone_parser.cpp
	)

set ( PostProc_MOCS
	plotter.h
	)


if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)
BISON_TARGET(touchstoneParser ${CMAKE_CURRENT_SOURCE_DIR}/touchstone.y ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_parser.cpp COMPILE_FLAGS "-p touchstone")
FLEX_TARGET(touchstoneLexer ${CMAKE_CURRENT_SOURCE_DIR}/touchstone.l ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_lexer.cpp COMPILE_FLAGS "-P touchstone")
ADD_FLEX_BISON_DEPENDENCY(touchstoneLexer touchstoneParser)

endif((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)

if(${QT_VERSION} EQUAL 4)
 QT4_WRAP_CPP(MOCS ${PostProc_MOCS})
endif(${QT_VERSION} EQUAL 4)
if(${QT_VERSION} EQUAL 5)
 QT5_WRAP_CPP(MOCS ${PostProc_MOCS})
endif(${QT_VERSION} EQUAL 5)


add_library ( PostProc STATIC ${PostProc_SRCS}
	${UIS} ${RSCS} ${TRS} ${MOCS}
       	)


if(${QT_VERSION} EQUAL 4)
target_link_libraries (PostProc
	              ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_LIBRARIES} ${QT_QTOPENGL_LIBRARY} 
	              )
endif(${QT_VERSION} EQUAL 4)


if(${QT_VERSION} EQUAL 5)
target_link_libraries (PostProc	
	               Qt5::Core 
		       Qt5::Widgets 
		       Qt5::OpenGL)
endif(${QT_VERSION} EQUAL 5)


set (filtmat2jc_SRCS
	filtmat2jc.cpp
      )
add_executable ( filtmat2jc ${filtmat2jc_SRCS})
target_link_libraries (filtmat2jc OStools ${Boost_LIBRARIES} )


set(updateFiltmat_SRCS
    updateFiltmat.cpp
    )
add_executable(updateFiltmat ${updateFiltmat_SRCS})
target_link_libraries (updateFiltmat OStools ${Boost_LIBRARIES} )


#if(WIN32)
 # install(TARGETS filtmat2jc updateFiltmat RUNTIME_DEPENDENCY_SET PostProcDep DESTINATION bin)
 # install(RUNTIME_DEPENDENCY_SET PostProcDep  DESTINATION bin PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" POST_EXCLUDE_REGEXES ".*system32/.*\\.dll")
#endif(WIN32)

install(TARGETS filtmat2jc updateFiltmat DESTINATION bin)
if(WIN32)
 install(FILES $<TARGET_RUNTIME_DLLS:updateFiltmat> TYPE BIN)
 install(FILES $<TARGET_RUNTIME_DLLS:filtmat2jc> TYPE BIN)
endif(WIN32)




#BISON_TARGET(touchstoneParser touchstone.y   ${CMAKE_CURRENT_BINARY_DIR}/touchstone_parser.cpp COMPILE_FLAGS "-p touchstone")
#FLEX_TARGET(touchstoneScanner touchstone.l   ${CMAKE_CURRENT_BINARY_DIR}/touchstone_lexer.cpp COMPILE_FLAGS "-P touchstone")
#ADD_FLEX_BISON_DEPENDENCY(touchstoneScanner touchstoneParser)


#add_library ( PostProc STATIC ${PostProc_SRCS}
#	${BISON_touchstoneParser_OUTPUTS}
#        ${FLEX_touchstoneScanner_OUTPUTS}
#	${UIS} ${RSCS} ${TRS} ${MOCS}
#       	)
