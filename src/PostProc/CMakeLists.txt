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

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Create target for the parser
 ADD_CUSTOM_TARGET(touchstoneparser echo "Creating touchstone parser")

# Create custom command for flex/lex (note the outputs)
 ADD_CUSTOM_COMMAND(
   SOURCE  touchstone.l
   COMMAND ${FLEX_EXECUTABLE} 
   ARGS -o ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_lexer.cpp
	-P touchstone
         ${CMAKE_CURRENT_SOURCE_DIR}/touchstone.l
   TARGET TouchstoneLexer
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_lexer.cpp
 )

# Create custom command for bison/yacc (note the DEPENDS)
 ADD_CUSTOM_COMMAND(
   SOURCE touchstone.y
   COMMAND ${BISON_EXECUTABLE}
   ARGS -d ${CMAKE_CURRENT_SOURCE_DIR}/touchstone.y
        --output ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_parser.cpp
        -p touchstone
   TARGET TouchstoneParser
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_lexer.cpp
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/touchstone_parser.cpp
 )

 SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/touchstone_parser.cpp GENERATED)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

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




#BISON_TARGET(touchstoneParser touchstone.y   ${CMAKE_CURRENT_BINARY_DIR}/touchstone_parser.cpp COMPILE_FLAGS "-p touchstone")
#FLEX_TARGET(touchstoneScanner touchstone.l   ${CMAKE_CURRENT_BINARY_DIR}/touchstone_lexer.cpp COMPILE_FLAGS "-P touchstone")
#ADD_FLEX_BISON_DEPENDENCY(touchstoneScanner touchstoneParser)


#add_library ( PostProc STATIC ${PostProc_SRCS}
#	${BISON_touchstoneParser_OUTPUTS}
#        ${FLEX_touchstoneScanner_OUTPUTS}
#	${UIS} ${RSCS} ${TRS} ${MOCS}
#       	)
