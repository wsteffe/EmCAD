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
	.
	../GUI
	../CAD
	)
set ( DataBase_HDRS
      DataBase.h	
     )

set ( DataBase_SRCS
	DataBase.cpp
	ListUtils.cpp
	MallocUtils.cpp
	Message.cpp
	TreeUtils.cpp
	# Tools.cpp
	avl.cpp
	model_lexer.cpp
	model_parser.cpp
	)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# Create target for the parser
 ADD_CUSTOM_TARGET(modelparser echo "Creating model parser")

# Create custom command for flex/lex (note the outputs)
 ADD_CUSTOM_COMMAND(
   SOURCE  model.l
   COMMAND ${FLEX_EXECUTABLE} 
   ARGS -o ${CMAKE_CURRENT_SOURCE_DIR}/model_lexer.cpp
	-P model
         ${CMAKE_CURRENT_SOURCE_DIR}/model.l
   TARGET ModelLexer
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/model_lexer.cpp
 )

# Create custom command for bison/yacc (note the DEPENDS)
 ADD_CUSTOM_COMMAND(
   SOURCE model.y
   COMMAND ${BISON_EXECUTABLE}
   ARGS -d ${CMAKE_CURRENT_SOURCE_DIR}/model.y
        --output ${CMAKE_CURRENT_SOURCE_DIR}/model_parser.cpp
        -p model
   TARGET ModelParser
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/model_lexer.cpp
   OUTPUTS ${CMAKE_CURRENT_SOURCE_DIR}/model_parser.cpp
 )

 SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/model_parser.cpp GENERATED)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

add_library ( DataBase STATIC 
	${DataBase_SRCS} 
	)

# BISON_TARGET(modelParser model.y   ${CMAKE_CURRENT_BINARY_DIR}/model_parser.cpp COMPILE_FLAGS "-p model")
# FLEX_TARGET(modelScanner model.l   ${CMAKE_CURRENT_BINARY_DIR}/model_lexer.cpp COMPILE_FLAGS "-P model")
# ADD_FLEX_BISON_DEPENDENCY(modelScanner modelParser)

#add_library ( DataBase STATIC 
#	${BISON_modelParser_OUTPUTS}
#        ${FLEX_modelScanner_OUTPUTS}
#	${DataBase_SRCS} 
#	)
