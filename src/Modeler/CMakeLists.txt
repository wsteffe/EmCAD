# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015  Walter Steffe
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


if(WIN32)

add_custom_target(modeler ALL
   SOURCES  setup.py
   COMMAND C:/Python27/python setup.py build
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(MODELER_EXE_FILES
	build/exe.win-amd64-2.7/aws_analyse.exe
	build/exe.win-amd64-2.7/aws_filtermap.exe
	build/exe.win-amd64-2.7/aws_symmfiltermap.exe
	build/exe.win-amd64-2.7/aws_getcredit.exe
	build/exe.win-amd64-2.7/aws_modelize1.exe
	build/exe.win-amd64-2.7/aws_modelize1_get.exe
	build/exe.win-amd64-2.7/aws_modelize1_send.exe
	build/exe.win-amd64-2.7/aws_modelizeb.exe
	build/exe.win-amd64-2.7/aws_modelizeb_get.exe
	build/exe.win-amd64-2.7/aws_modelizeb_send.exe
	build/exe.win-amd64-2.7/aws_modelizep.exe
	build/exe.win-amd64-2.7/aws_modelizep_get.exe
	build/exe.win-amd64-2.7/aws_modelizep_send.exe
	build/exe.win-amd64-2.7/aws_reduce.exe
)
set(MODELER_PYD_FILES
	build/exe.win-amd64-2.7/bz2.pyd
	build/exe.win-amd64-2.7/_ctypes.pyd
	build/exe.win-amd64-2.7/_hashlib.pyd
	build/exe.win-amd64-2.7/pyexpat.pyd
	build/exe.win-amd64-2.7/select.pyd
	build/exe.win-amd64-2.7/_socket.pyd
	build/exe.win-amd64-2.7/_ssl.pyd
	build/exe.win-amd64-2.7/unicodedata.pyd
)
set(MODELER_ZIP_FILES
	build/exe.win-amd64-2.7/library.zip
)
set(MODELER_DLL_FILES
	build/exe.win-amd64-2.7/python27.dll
)
foreach(EXE ${MODELER_EXE_FILES})
install(PROGRAMS  ${EXE}
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(EXE)

foreach(PYD ${MODELER_PYD_FILES})
install(PROGRAMS  ${PYD}
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(PYD)

foreach(DLL ${MODELER_DLL_FILES})
install(PROGRAMS  ${DLL}
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(DLL)

install(FILES build/exe.win-amd64-2.7/cacerts.txt DESTINATION aws)

foreach(ZIP ${MODELER_ZIP_FILES})
install(PROGRAMS  ${ZIP}
       	DESTINATION aws
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(ZIP)

else(WIN32)

########### install files ###############

install(PROGRAMS  aws_modelize1.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelize1_send.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelize1_get.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizeb.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizeb_send.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizeb_get.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizep.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizep_send.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_modelizep_get.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_reduce.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_analyse.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  aws_filtermap.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  aws_symmfiltermap.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  aws_getcredit.py
       	DESTINATION aws
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )


endif(WIN32)
