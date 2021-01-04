# This file is part of the EmCAD program which constitutes the client
# side of an electromagnetic modeler delivered as a cloud based service.
# 
# Copyright (C) 2015-2020  Walter Steffe
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
   COMMAND  python3 setup.py build
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set(PYUTILS_EXE_FILES
	build/exe.mingw-3.8/api_utils.exe
	build/exe.mingw-3.8/api_download.exe
	build/exe.mingw-3.8/api_submit.exe
	build/exe.mingw-3.8/filter_tune.exe
	build/exe.mingw-3.8/remove_dir.exe
	build/exe.mingw-3.8/tar_extract.exe
	build/exe.mingw-3.8/tar_create.exe
	build/exe.mingw-3.8/tar_component.exe
	build/exe.mingw-3.8/touchstone2to1.exe
	build/exe.mingw-3.8/roughSurfaceFit.exe
)
set(PYUTILS_DLL_FILES
	build//exe.mingw-3.8/libpython3.8.dll
	build//exe.mingw-3.8/libwinpthread-1.dll
)
foreach(EXE ${PYUTILS_EXE_FILES})
install(PROGRAMS  ${EXE}
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(EXE)

foreach(DLL ${PYUTILS_DLL_FILES})
install(PROGRAMS  ${DLL}
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
endforeach(DLL)

install(DIRECTORY "build/exe.mingw-3.8/lib" 
	DESTINATION bin
        )

else(WIN32)

########### install files ###############

install(PROGRAMS  api_utils.py
       	DESTINATION bin
	PERMISSIONS OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  api_download.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  api_submit.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  filter_tune.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS tar_extract.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  tar_create.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS remove_dir.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS touchstone2to1.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS roughSurfaceFit.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS vectfit.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )
install(PROGRAMS  tar_component.py
       	DESTINATION bin
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  splitdxf.py
       	DESTINATION bin
	RENAME splitdxf
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

install(PROGRAMS  cleandxf.py
       	DESTINATION bin
	RENAME cleandxf
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ
        )

endif(WIN32)
