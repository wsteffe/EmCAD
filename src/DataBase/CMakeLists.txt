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
	.
	../GUI
	../CAD
	)
set ( DataBase_HDRS
      Message.h	
      DataBase.h	
      Buffer.h	
     )

set ( DataBase_SRCS
	DataBase.cpp
	List.cpp
	Tree.cpp
	MallocUtils.cpp
	Message.cpp
	# Tools.cpp
	avl.cpp
	model_lexer.cpp
	model_parser.cpp
	)

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)

# Create target for the parser

FIND_PACKAGE(BISON)
FIND_PACKAGE(FLEX)
BISON_TARGET(modelParser ${CMAKE_CURRENT_SOURCE_DIR}/model.y ${CMAKE_CURRENT_SOURCE_DIR}/model_parser.cpp COMPILE_FLAGS "-p model")
FLEX_TARGET(modelLexer ${CMAKE_CURRENT_SOURCE_DIR}/model.l ${CMAKE_CURRENT_SOURCE_DIR}/model_lexer.cpp COMPILE_FLAGS "-P model")
ADD_FLEX_BISON_DEPENDENCY(modelLexer modelParser)

endif((${CMAKE_SYSTEM_NAME} MATCHES "Linux") OR MSYS)

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
