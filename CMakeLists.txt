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


cmake_minimum_required( VERSION 3.21.0 )
project(EmCAD )

set(EmCAD_VERSION_MAJOR 0)
set(EmCAD_VERSION_MINOR 7)
set(EmCAD_VERSION_PATCH 0)

set(EmCAD_SHORT_LICENSE "GNU General Public License")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)




add_definitions ( -Wall )

set(internalPython 0)
set(useSymbolicLinks 0)
set(useGmshTetrahedra 1)


set(CASROOT $ENV{CASROOT} CACHE STRING FILEPATH)
if(NOT ${CASROOT} STREQUAL "")
 string(REGEX REPLACE "\\\\" "\\\\\\\\" CASROOT  ${CASROOT})
endif(NOT ${CASROOT} STREQUAL "")


set(GMSHLIB $ENV{GMSHLIB} CACHE STRING FILEPATH)
if(NOT ${GMSHLIB} STREQUAL "")
 string(REGEX REPLACE "\\\\" "\\\\\\\\" GMSHLIB  ${GMSHLIB})
endif(NOT ${GMSHLIB} STREQUAL "")

set (CMAKE_CXX_STANDARD 11)

if(MSVC)
 SET(CMAKE_CXX_FLAGS  ${CMAKE_CXX_FLAGS} "/openmp")
 add_definitions(-D_USE_MATH_DEFINES -DNOMINMAX  -D_CRT_SECURE_NO_WARNINGS)
 add_definitions(-DWIN32)
 add_definitions(-DYY_NO_UNISTD_H)
else(MSVC)
 SET(CMAKE_CXX_FLAGS           "-fpermissive  -fopenmp")
 SET(CMAKE_CXX_FLAGS_RELEASE   "-O2")
 SET(CMAKE_CXX_FLAGS_DEBUG     "-O0  -g")
 SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif(MSVC)

if(MSVC)
  add_definitions(-D_MSVC)
  add_definitions(-DWNT)
elseif(MINGW)
  add_definitions(-D_MINGW)
  add_definitions(-DWNT)
endif(MSVC)

macro(find_all_libraries VARNAME LISTNAME PATH SUFFIX)
  set(${VARNAME})
  list(LENGTH ${LISTNAME} NUM_LIST)
  foreach(LIB ${${LISTNAME}})
    find_library(FOUND_LIB ${LIB} PATHS ${PATH} PATH_SUFFIXES ${SUFFIX})
    if(FOUND_LIB)
      list(APPEND ${VARNAME} ${FOUND_LIB})
    endif(FOUND_LIB)
    unset(FOUND_LIB CACHE) # only available in cmake >= 2.6.4
  endforeach(LIB)
  list(LENGTH ${VARNAME} NUM_FOUND_LIBRARIES)
  if(NUM_FOUND_LIBRARIES LESS NUM_LIST)
    set(${VARNAME})
  endif(NUM_FOUND_LIBRARIES LESS NUM_LIST)
endmacro(find_all_libraries)

macro(set_config_option VARNAME STRING)
  set(${VARNAME} TRUE)
  list(APPEND CONFIG_OPTIONS ${STRING})
  message(STATUS "Found " ${STRING})
endmacro(set_config_option)

INCLUDE (CheckTypeSize)
check_type_size("void*" SIZEOF_VOID_P)
if(SIZEOF_VOID_P EQUAL 8)
  set_config_option(HAVE_64BIT_SIZE_T "Have64BitSizeT")
endif(SIZEOF_VOID_P EQUAL 8)


find_package(PythonLibs 3 REQUIRED)
if(WIN32)
find_file(PYTHON_DLL NAMES libpython3.10.dll)
endif(WIN32)
message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_DLL = ${PYTHON_DLL}")

if(LINUX OR MSYS)
 find_package(BISON)
 find_package(FLEX)
# find_package (PythonLibs 2.7 REQUIRED)
endif(LINUX OR MSYS)

if((${CMAKE_SYSTEM_NAME} MATCHES "Linux"))
 add_definitions( -DHAVE_XLIB )
endif()


#find Boost
set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS ON)
if(WIN32)
 add_definitions( -DBOOST_ALL_NO_LIB )
endif(WIN32)
#add_definitions(-DBOOST_FILESYSTEM_VERSION=2)
FIND_PACKAGE( Boost COMPONENTS system filesystem graph date_time REQUIRED)
FIND_PACKAGE( Threads REQUIRED )

set(OCC_3PARTIES_LIBRARIES)
if(MSVC)
  set(OCC_3PARTIES_DLLS 
	"C:/occ3dparties/freeimage-3.16.0-vc12-64-opt/bin/FreeImage.dll"
	"C:/occ3dparties/freeimage-3.16.0-vc12-64-opt/bin/FreeImagePlus.dll"
        "C:/occ3dparties/freetype-2.5.3-vc12-64-opt/bin/freetype.dll"
	"C:/occ3dparties/gl2ps-1.3.8-vc12-64-opt/lib/gl2ps.dll"
  )
  set(OCC_3PARTIES_DLLS_DEB 
	"C:/occ3dparties/freeimage-3.16.0-vc12-64-deb/bin/FreeImage.dll"
	"C:/occ3dparties/freeimage-3.16.0-vc12-64-deb/bin/FreeImagePlus.dll"
        "C:/occ3dparties/freetype-2.5.3-vc12-64-opt/bin/freetype.dll"
	"C:/occ3dparties/gl2ps-1.3.8-vc12-64-deb/lib/gl2ps.dll"
  )
  set(OCC_Resources
    ${CASROOT}/src/StdResource
    ${CASROOT}/src/XSTEPResource
    ${CASROOT}/src/XmlOcafResource
  )
endif(MSVC)

if(MINGW)
  set(OCC_3PARTIES_LIBRARIES_REQUIRED
	  #        "freetype"
  )
  set(OCC_3PARTIES_DLLS)
  set(OCC_3PARTIES_DLLS_DEB)
  foreach(LIB_REQ ${OCC_3PARTIES_LIBRARIES_REQUIRED})
    set(DLL "DLL-NOTFOUND")
    find_file(DLL ${LIB_REQ}.dll PATH_SUFFIXES bin)
    if(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
       list(APPEND OCC_3PARTIES_DLLS ${DLL})
    else(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
       message(SEND_ERROR ${LIB_REQ}.dll " not Found")
    endif(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
    find_library(LIB ${LIB_REQ})
    if(NOT ${LIB} STREQUAL "DLL-NOTFOUND")
       list(APPEND OCC_3PARTIES_LIBRARIES ${LIB})
    else(NOT ${LIB} STREQUAL "DLL-NOTFOUND")
       message(SEND_ERROR "library ${LIB_REQ} not Found")
    endif(NOT ${LIB} STREQUAL "DLL-NOTFOUND")
    unset(DLL CACHE)
    unset(LIB CACHE)
  endforeach(LIB_REQ ${OCC_3PARTIES_LIBRARIES_REQUIRED})
  set(OCC_Resources
    ${CASROOT}/src/StdResource
    ${CASROOT}/src/XSTEPResource
    ${CASROOT}/src/XmlOcafResource
  )
endif(MINGW)


#Find OpenCascade
if(WIN32 OR MINGW)
    if(HAVE_64BIT_SIZE_T)
      set(OCC_SYS_NAME win64)
    else(HAVE_64BIT_SIZE_T)
      set(OCC_SYS_NAME win32)
    endif(HAVE_64BIT_SIZE_T)
  else(WIN32 OR MINGW)
    set(OCC_SYS_NAME ${CMAKE_SYSTEM_NAME})
  endif(WIN32 OR MINGW)
  set(OCC_LIBRARIES_REQUIRED
	   TKBin
	   TKBinL
	   TKBinXCAF
	   TKBO
	   TKBool
	   TKBRep
	   TKCAF
	   TKCDF
	   TKernel
	   TKFeat
	   TKFillet
	   TKG2d
	   TKG3d
	   TKGeomAlgo
	   TKGeomBase
	   TKHLR
	   TKIGES
	   TKLCAF
	   TKMath
	   TKMesh
	   TKMeshVS
	   TKOffset
	   TKOpenGl
	   TKPrim
	   TKService
  	   TKShHealing
  	   TKStd
	   TKStdL
	   TKSTEP
           TKSTEP209
	   TKSTEPAttr
	   TKSTEPBase
	   TKSTL
	   TKTopAlgo
	   TKV3d
	   TKVRML
	   TKXCAF
	   TKVCAF
	   TKXDEIGES
	   TKXDESTEP
	   TKXml
	   TKXmlL
	   TKXmlXCAF
	   TKXSBase
  )

  list(LENGTH OCC_LIBRARIES_REQUIRED NUM_OCC_LIBRARIES_REQUIRED)
  set(OCC_LIBRARIES)
  set(OCC_LIBRARIES_DEB)
  foreach(OCC ${OCC_LIBRARIES_REQUIRED})
    set(OCC_LIB "OCC_LIB-NOTFOUND")
    set(OCC_LIB_DEB "OCC_LIB_DEB-NOTFOUND")
    if(MSVC)
      find_library(OCC_LIB ${OCC} PATHS ${CASROOT}/${OCC_SYS_NAME}/vc12/lib  ${CASROOT}/${OCC_SYS_NAME}/vc10/lib NO_DEFAULT_PATH)
      find_library(OCC_LIB_DEB ${OCC} PATHS ${CASROOT}/${OCC_SYS_NAME}/vc12/libd  ${CASROOT}/${OCC_SYS_NAME}/vc10/libd NO_DEFAULT_PATH)
    elseif(MINGW)
      find_library(OCC_LIB ${OCC} PATHS ${CASROOT}/${OCC_SYS_NAME}/gcc PATH_SUFFIXES lib bin)
    else(MSVC)
      find_library(OCC_LIB ${OCC} PATH_SUFFIXES oce/lib)
      if(${OCC_LIB} STREQUAL "OCC_LIB-NOTFOUND")
       find_library(OCC_LIB ${OCC} PATH_SUFFIXES lib) 
      endif()
    endif(MSVC) 
    if(NOT ${OCC_LIB} STREQUAL "OCC_LIB-NOTFOUND")
      list(APPEND OCC_LIBRARIES ${OCC_LIB})
      message(STATUS "Found  ${OCC} = ${OCC_LIB}")
    else(NOT ${OCC_LIB} STREQUAL "OCC_LIB-NOTFOUND")
      message(SEND_ERROR "${OCC} not Found")
    endif(NOT ${OCC_LIB} STREQUAL "OCC_LIB-NOTFOUND")
    if(MSVC)
    if(NOT ${OCC_LIB_DEB} STREQUAL "OCC_LIB_DEB-NOTFOUND")
      list(APPEND OCC_LIBRARIES_DEB ${OCC_LIB_DEB})
    else(NOT ${OCC_LIB_DEB} STREQUAL "OCC_LIB-NOTFOUND")
      list(APPEND OCC_LIBRARIES_DEB ${OCC_LIB})
      message(WARNING ${OCC} " not Found Debug Version")
    endif(NOT ${OCC_LIB_DEB} STREQUAL "OCC_LIB_DEB-NOTFOUND")
    unset(OCC_LIB_DEB CACHE) # only available in cmake >= 2.6.4
    endif(MSVC) 
    unset(OCC_LIB CACHE) # only available in cmake >= 2.6.4
  endforeach(OCC)

  if(WIN32)
   set(OCC_DLLS)
   set(OCC_DLLS_DEB)
   foreach(OCC ${OCC_LIBRARIES_REQUIRED})
    set(OCC_DLL "OCC_DLL-NOTFOUND")
    if(MSVC)
     find_file(OCC_DLL ${OCC}.dll PATHS ${CASROOT}/${OCC_SYS_NAME}/vc12/bin  ${CASROOT}/${OCC_SYS_NAME}/vc10/bin NO_DEFAULT_PATH)
    elseif(MINGW)
     find_file(OCC_DLL lib${OCC}.dll PATHS ${CASROOT}/${OCC_SYS_NAME}/gcc PATH_SUFFIXES bin)
    endif()
    if(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
      list(APPEND OCC_DLLS ${OCC_DLL})
    else(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
      message(SEND_ERROR ${OCC} " not Found")
    endif(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
    if(MSVC)
    set(OCC_DLL "OCC_DLL-NOTFOUND")
     find_file(OCC_DLL ${OCC}.dll PATHS ${CASROOT}/${OCC_SYS_NAME}/vc12/bind  ${CASROOT}/${OCC_SYS_NAME}/vc10/bind NO_DEFAULT_PATH)
     if(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
      list(APPEND OCC_DLLS_DEB ${OCC_DLL})
     else(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
      message(STATUS "Debug version of" ${OCC} " not Found")
     endif(NOT ${OCC_DLL} STREQUAL "OCC_DLL-NOTFOUND")
    endif(MSVC)
    unset(OCC_DLL CACHE) # only available in cmake >= 2.6.4
   endforeach(OCC ${OCC_LIBRARIES_REQUIRED})
  endif(WIN32)

  list(LENGTH OCC_LIBRARIES NUM_OCC_LIBRARIES)
  set(OCC_INCLUDES)
  if(NUM_OCC_LIBRARIES EQUAL NUM_OCC_LIBRARIES_REQUIRED)
  if(MINGW)
    find_path(OCC_INC "BRep_Tool.hxx" PATHS ${CASROOT} PATH_SUFFIXES include/opencascade)
  elseif(MSVC)
    find_path(OCC_INC "BRep_Tool.hxx" PATHS ${CASROOT} PATH_SUFFIXES include/opencascade)
  else(MINGW)
    find_path(OCC_INC "BRep_Tool.hxx" PATHS ${CASROOT} PATH_SUFFIXES include/opencascade)
  endif(MINGW)
    if(OCC_INC)
      list(APPEND OCC_INCLUDES ${OCC_INC})
      set_config_option(HAVE_OCC "OpenCascade")
      #      ADD_DEFINITIONS(-DCSFDB)
      if(HAVE_64BIT_SIZE_T)
        add_definitions(-D_OCC72)
      endif(HAVE_64BIT_SIZE_T)
      find_path(OCC_CONFIG_H "config.h" PATHS ${OCC_INC} ${CASROOT} NO_DEFAULT_PATH)
      if(${OCC_CONFIG_H} STREQUAL "OCC_CONFIG_H-NOTFOUND")
        add_definitions(-DHAVE_NO_OCC_CONFIG_H)
      else(${OCC_CONFIG_H} STREQUAL "OCC_CONFIG_H-NOTFOUND")
        list(APPEND OCC_INCLUDES ${OCC_CONFIG_H})
	ADD_DEFINITIONS(-DHAVE_OCC_CONFIG_H)
      endif(${OCC_CONFIG_H} STREQUAL "OCC_CONFIG_H-NOTFOUND")
    endif(OCC_INC)
  endif(NUM_OCC_LIBRARIES EQUAL NUM_OCC_LIBRARIES_REQUIRED)



#Find gmsh
  set(GMSH_INCLUDES)
  set(GMSH_LIBRARY "GMSH_LIBRARY-NOTFOUND")
  set(GMSH_LIBRARY_DEB "GMSH_LIBRARY_DEB-NOTFOUND")
  if(MSVC)
    find_library(GMSH_LIBRARY_DEB gmshd PATHS ${GMSHLIB} PATH_SUFFIXES Debug)
    find_library(GMSH_LIBRARY gmsh PATHS ${GMSHLIB} PATH_SUFFIXES Release)
  elseif(MINGW)
    find_library(GMSH_LIBRARY gmsh PATHS ${GMSHLIB} PATH_SUFFIXES lib)
  else(MSVC)
    find_library(GMSH_LIBRARY gmsh PATHS ${GMSHLIB} )
  endif(MSVC)
  if(${GMSH_LIBRARY} STREQUAL "GMSH_LIBRARY-NOTFOUND")
    message( SEND_ERROR "GMSH_LIBRARY NOTFOUND" )
  else(${GMSH_LIBRARY} STREQUAL "GMSH_LIBRARY-NOTFOUND")
    message(STATUS "Found " ${GMSH_LIBRARY})
    find_path(GMSH_INCLUDE  NAMES "gmsh.h" PATHS ${GMSHLIB}/include)
    if(${GMSH_INCLUDE} STREQUAL "GMSH_INCLUDE-NOTFOUND")
      message( SEND_ERROR " gmsh.h NOTFOUND")
    else(${GMSH_INCLUDE} STREQUAL "GMSH_INCLUDE-NOTFOUND")
      message(STATUS " Found gmsh.h")
      list(APPEND GMSH_INCLUDES ${GMSH_INCLUDE})
    endif(${GMSH_INCLUDE} STREQUAL "GMSH_INCLUDE-NOTFOUND")
    find_path(GMSH_SOURCES "Geo/GModel.h" PATHS $ENV{GMSHSOURCES})
    if(${GMSH_SOURCES} STREQUAL "GMSH_SOURCES-NOTFOUND")
      message( SEND_ERROR " GModel.h NOTFOUND")
    else(${GMSH_SOURCES} STREQUAL "GMSH_SOURCES-NOTFOUND")
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/src/common)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/src/geo)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/src/mesh)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/src/numeric)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/contrib)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/contrib/ANN)
      list(APPEND GMSH_INCLUDES ${GMSH_SOURCES}/src/post)
    endif(${GMSH_SOURCES} STREQUAL "GMSH_SOURCES-NOTFOUND")
  endif(${GMSH_LIBRARY} STREQUAL "GMSH_LIBRARY-NOTFOUND")
  if(WIN32)
    list(APPEND GMSH_EXTERNAL_LIBRARIES winmm wsock32 ws2_32)
  endif(WIN32)
  if(CYGWIN OR MINGW)
    list(APPEND MINGW_LIBRARIES psapi)
    #    set(GMSH_LINK_FLAGS "-Wl,--stack,16777216 -mwindows -Wl,--enable-auto-import -Wl,--export-all-symbols,--output-def")
    set(MINGW_LINK_FLAGS "-static-libgcc -static-libstdc++")
  endif(CYGWIN OR MINGW)


  if(MSVC AND ${GMSH_LIBRARY_DEB} STREQUAL "GMSH_LIBRARY_DEB-NOTFOUND")
    message( STATUS "DEBUG VERSION OF GMSH_LIBRARY NOTFOUND" )
    set(GMSH_LIBRARY_DEB ${GMSH_LIBRARY})
  endif()


  if(MINGW)
  set(MINGW_DLLS_REQUIRED
    libgfortran-5.dll
    libgomp-1.dll
    libgcc_s_seh-1.dll
    libstdc++-6.dll
    libwinpthread-1.dll
    libquadmath-0.dll
  )
#    libobjc-4.dll
#    libssp-0.dll

  set(MINGW_DLLS)
  foreach(DLL_REQ ${MINGW_DLLS_REQUIRED})
    set(DLL "DLL-NOTFOUND")
    find_file(DLL ${DLL_REQ})
    if(${DLL} STREQUAL "DLL-NOTFOUND")
      message(SEND_ERROR ${DLL_REQ} " not Found")
    else(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
      list(APPEND MINGW_DLLS ${DLL})
      message(STATUS "${DLL_REQ} = ${DLL}")
    endif(${DLL} STREQUAL "DLL-NOTFOUND")
    unset(DLL CACHE)
  endforeach(DLL_REQ ${MINGW_DLLS_REQUIRED})
  endif(MINGW)


#Find BLAS LAPACK (needed by gmsh)
  if(MSVC)
    set(LAPACK_LIBS_REQUIRED lapack blas libf2c)
    set(LAPACK_LIBS_REQUIRED_DEB lapackd blasd libf2cd)
    find_all_libraries(LAPACK_LIBRARIES LAPACK_LIBS_REQUIRED "" "")
    find_all_libraries(LAPACK_LIBRARIES_DEB LAPACK_LIBS_REQUIRED_DEB "" "")
    if(LAPACK_LIBRARIES)
       set_config_option(HAVE_BLAS "cblas with libf2c")
       set_config_option(HAVE_LAPACK "clapack with libf2c)")
    endif(LAPACK_LIBRARIES)
    message(STATUS "LAPACK_LIBRARIES=" ${LAPACK_LIBRARIES})
  elseif(MINGW)
    set(LAPACK_LIBS_REQUIRED lapack blas)
    find_all_libraries(LAPACK_LIBRARIES LAPACK_LIBS_REQUIRED "" "")
    if(LAPACK_LIBRARIES)
        set_config_option(HAVE_BLAS "blas")
        set_config_option(HAVE_LAPACK "lapack)")
    endif(LAPACK_LIBRARIES)
    message(STATUS "LAPACK_LIBRARIES=" ${LAPACK_LIBRARIES})
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # on Linux try to find ATLAS without a Fortran compiler,
    # because cmake ships with a buggy FindBLAS e.g. on Ubuntu Lucid
    # Lynx
    set(ATLAS_LIBS_REQUIRED lapack f77blas cblas atlas)
    find_all_libraries(LAPACK_LIBRARIES ATLAS_LIBS_REQUIRED "" "")
    if(LAPACK_LIBRARIES)
      set_config_option(HAVE_BLAS "Blas(ATLAS)")
      set_config_option(HAVE_LAPACK "Lapack(ATLAS)")
    endif(LAPACK_LIBRARIES)
  endif(MSVC)
  if(NOT HAVE_BLAS OR NOT HAVE_LAPACK)
    # if we haven't found blas and lapack without using the standard
    # cmake tests, do it (this requires a working Fortran compiler)
    enable_language(Fortran)
    find_package(BLAS)
    if(BLAS_FOUND)
      set_config_option(HAVE_BLAS "Blas")
      find_package(LAPACK)
      if(LAPACK_FOUND)
        set_config_option(HAVE_LAPACK "Lapack")
      else(LAPACK_FOUND)
        set(LAPACK_LIBRARIES ${BLAS_LIBRARIES})
      endif(LAPACK_FOUND)
      if(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
        if(CMAKE_Fortran_COMPILER MATCHES "gfortran")
          list(APPEND LAPACK_LIBRARIES gfortran)
        elseif(CMAKE_Fortran_COMPILER MATCHES "f95")
          list(APPEND LAPACK_LIBRARIES gfortran)
        elseif(CMAKE_Fortran_COMPILER MATCHES "g77")
          list(APPEND LAPACK_LIBRARIES g2c)
        endif(CMAKE_Fortran_COMPILER MATCHES "gfortran")
      endif(CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
    endif(BLAS_FOUND)
  endif(NOT HAVE_BLAS OR NOT HAVE_LAPACK)
  if(NOT HAVE_BLAS OR NOT HAVE_LAPACK)
    message( SEND_ERROR " LAPACK_LIBRARIES NOTFOUND\n Please set CMAKE_PREFIX_PATH pointing to the location of BLAS/LAPACK libraries")
  endif(NOT HAVE_BLAS OR NOT HAVE_LAPACK)


set (GMSH_DEPS_LIBRARIES)
if(WIN32)
#  set (GMSH_DEPS_LIBRARIES_REQUIRED fltk fltk_gl fltk_images fltk_jpeg fltk_png fltk_z hdf5)
  set(GMSH_DEPS_LIBRARIES_REQUIRED "hdf5")
  set(GMSH_DEPS_LIBRARIES)
  if(MSVC)
   set(GMSH_DEPS_LIBRARIES_DEB)
  endif(MSVC)
  foreach(LIB_REQ ${GMSH_DEPS_LIBRARIES_REQUIRED})
    set(LIB "LIB-NOTFOUND")
    find_library(LIB ${LIB_REQ} )
    if(${LIB} STREQUAL "LIB-NOTFOUND")
      message(SEND_ERROR ${LIB_REQ}" NOT FOUND\n Please set CMAKE_LIBRARY_PATH pointing to its location ")
    else(${LIB} STREQUAL "LIB-NOTFOUND")
      list(APPEND GMSH_DEPS_LIBRARIES ${LIB})
    endif(${LIB} STREQUAL "LIB-NOTFOUND")
    unset(LIB CACHE)
  if(MSVC)
    set(LIBD "LIB-NOTFOUND")
    find_library(LIBD "${LIB_REQ}d")
    if(${LIBD} STREQUAL "LIBD-NOTFOUND")
      list(APPEND GMSH_DEPS_LIBRARIES_DEB ${LIBD})
    else(${LIBD} STREQUAL "LIBD-NOTFOUND")
      message( STATUS ${LIB_REQ}d" NOT FOUND ")
      list(APPEND GMSH_DEPS_LIBRARIES_DEB ${LIB})
    endif(${LIBD} STREQUAL "LIBD-NOTFOUND")
    unset(LIBD CACHE)
  endif(MSVC)
  endforeach(LIB_REQ)
else(WIN32)
  set(GMSH_DEPS_LIBRARIES "gmp")
endif(WIN32)


if(WIN32)
 set(QT_VERSION 5)
else()
 set(QT_VERSION 5)
endif()

if(${QT_VERSION} EQUAL 4)
  find_package (Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED )
  message(STATUS "QT_QTCORE_LIBRARY=" ${QT_QTCORE_LIBRARY})
  message(STATUS "QT_QTGUI_LIBRARY=" ${QT_QTGUI_LIBRARY})
  message(STATUS "QT_QTOPENGL_LIBRARY=" ${QT_QTOPENGL_LIBRARY})
endif(${QT_VERSION} EQUAL 4)

if(${QT_VERSION} EQUAL 5) 
 find_package(Qt5Core REQUIRED)
 find_package(Qt5Widgets REQUIRED)
 find_package(Qt5Gui REQUIRED)
 find_package(Qt5OpenGL REQUIRED)
 find_package(Qt5PrintSupport REQUIRED)
 get_target_property(Qt5Core_location_Release Qt5::Core LOCATION_Release)
 get_target_property(Qt5Core_location_Debug Qt5::Core LOCATION_Debug)
 get_target_property(Qt5Gui_location_Release Qt5::Gui LOCATION_Release)
 get_target_property(Qt5Gui_location_Debug Qt5::Gui LOCATION_Debug)
 get_target_property(Qt5Widgets_location_Release Qt5::Widgets LOCATION_Release)
 get_target_property(Qt5Widgets_location_Debug Qt5::Widgets LOCATION_Debug)
 get_target_property(Qt5OpenGL_location_Release Qt5::OpenGL LOCATION_Release)
 get_target_property(Qt5OpenGL_location_Debug Qt5::OpenGL LOCATION_Debug)
 get_target_property(Qt5PrintSupport_location_Release Qt5::PrintSupport LOCATION_Release)
 get_target_property(Qt5PrintSupport_location_Debug Qt5::PrintSupport LOCATION_Debug)

 message(STATUS "QWT_LIBRARY=" ${QWT_LIBRARY})
 message(STATUS "Qt5Core_location_Release=" ${Qt5Core_location_Release})
 message(STATUS "Qt5Core_location_Debug=" ${Qt5Core_location_Debug})
 message(STATUS "Qt5Widgets_location_Release=" ${Qt5Widgets_location_Release})
 message(STATUS "Qt5Widgets_location_Debug=" ${Qt5Widgets_location_Debug})
 message(STATUS "Qt5Gui_location_Release=" ${Qt5Gui_location_Release})
 message(STATUS "Qt5Gui_location_Debug=" ${Qt5Gui_location_Debug})
 message(STATUS "Qt5OpenGL_location_Release=" ${Qt5OpenGL_location_Release})
 message(STATUS "Qt5OpenGL_location_Debug=" ${Qt5OpenGL_location_Debug})

 if(WIN32) 
   set(QT_DLLS_REQUIRED_REL
	 # "icudt54.dll"
	 # "icuin54.dll"
	 # "icuuc54.dll"
	 "Qt5Core.dll"
	 "Qt5Gui.dll"
	 "Qt5OpenGL.dll"
	 "Qt5Widgets.dll"
	 "Qt5Svg.dll"
   )
   set(QT_DLLS_REQUIRED_DEB
	 # "icudt54.dll"
	 # "icuin54.dll"
	 # "icuuc54.dll"
   )
  set(QT_PLATFORMS_PLUGINS_REQUIRED
      Qt5::QWindowsIntegrationPlugin
  )
  foreach(plugin ${Qt5Gui_PLUGINS})
    get_target_property(_loc ${plugin} LOCATION_Debug)
    message(STATUS "Plugin ${plugin} is at location ${_loc}")
    get_target_property(_loc ${plugin} LOCATION_Release)
    message(STATUS "Plugin ${plugin} is at location ${_loc}")
  endforeach()
  foreach(plugin ${Qt5Widgets_PLUGINS})
    get_target_property(_loc ${plugin} LOCATION_Debug)
    message(STATUS "Plugin ${plugin} is at location ${_loc}")
    get_target_property(_loc ${plugin} LOCATION_Release)
    message(STATUS "Plugin ${plugin} is at location ${_loc}")
  endforeach()

  set(QT_PLATFORMS_PLUGINS_DEB)
  set(QT_PLATFORMS_PLUGINS_REL)
  foreach(plugin ${QT_PLATFORMS_PLUGINS_REQUIRED})
    get_target_property(_loc ${plugin} LOCATION_Debug)
    list(APPEND QT_PLATFORMS_PLUGINS_DEB ${_loc})
    get_target_property(_loc ${plugin} LOCATION_Release)
    list(APPEND QT_PLATFORMS_PLUGINS_REL ${_loc})
  endforeach()
 
#        "qwindowsd.dll"
   set(QT_DLLS_REL)
   foreach(DLL_REQ ${QT_DLLS_REQUIRED_REL})
    set(DLL "DLL-NOTFOUND")
    find_file(DLL ${DLL_REQ})
    if(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
	    list(APPEND QT_DLLS_REL ${DLL})
    else(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
      message(SEND_ERROR ${DLL_REQ} " not Found")
    endif(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
    unset(DLL CACHE)
   endforeach(DLL_REQ ${QT_DLLS_REQUIRED_REL})
   set(QT_DLLS_DEB)
   foreach(DLL_REQ ${QT_DLLS_REQUIRED_DEB})
    set(DLL "DLL-NOTFOUND")
    find_file(DLL ${DLL_REQ})
    if(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
      list(APPEND QT_DLLS_DEB ${DLL})
    else(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
      message(SEND_ERROR ${DLL_REQ} " not Found")
    endif(NOT ${DLL} STREQUAL "DLL-NOTFOUND")
    unset(DLL CACHE)
   endforeach(DLL_REQ ${QT_DLLS_REQUIRED_DEB})
 endif(WIN32)

endif(${QT_VERSION} EQUAL 5)

message (STATUS "Qt5Widgets_VERSION = ${Qt5Widgets_VERSION_STRING}" )




#Find Qwt
if(WIN32)
  find_library (QWT_LIBRARY qwt-qt5)
  find_file (QWT_DLL NAMES qwt-qt5.dll)
  find_path (QWT_INCLUDE_DIR qwt_plot.h PATH_SUFFIXES qwt-qt5)
else(WIN32)
  find_library(QWT_LIBRARY qwt-qt5 REQUIRED)
  find_path (QWT_INCLUDE_DIR qwt_plot.h PATHS /usr/include PATH_SUFFIXES qwt)
endif(WIN32)

if(MINGW)
 set (BUNDLEDLLS)
 if (EXISTS "${PROJECT_SOURCE_DIR}/mingw-bundledlls.txt")
  FILE(STRINGS "${PROJECT_SOURCE_DIR}/mingw-bundledlls.txt" BUNDLEDLLS)
  foreach(DLL ${BUNDLEDLLS})
    message(STATUS ${DLL} "found by mingw-bundledlls")
  endforeach(DLL ${BUNDLEDLLS})
 endif (EXISTS "${PROJECT_SOURCE_DIR}/mingw-bundledlls.txt")
endif(MINGW)


if(${internalPython} EQUAL 1)
 add_definitions(-DINTERNAL_PYTHON)
endif(${internalPython} EQUAL 1)


if(WIN32) 
  if(MSVC)
    if(${QT_VERSION} EQUAL 5) 
     INSTALL(FILES 
		    ${QtCore_location_Release} 
		    ${QtGui_location_Release} 
		    ${QtWidgets_location_Release}
		    ${QtPrintSupport_location_Release} 
	            ${Qt5OpenGL_location_Release} 
		    ${QWT_DLL} 
                    CONFIGURATIONS Release
	            DESTINATION bin)
     INSTALL(FILES 
	            ${QtCore_location_Debug} 
		    ${QtGui_location_Debug} 
	            ${QtWidgets_location_Debug}
	            ${QtPrintSupport_location_Debug} 
		    ${Qt5OpenGL_location_Debug} 
		    ${QWT_DLL_DEB} 
		    CONFIGURATIONS Debug
	            DESTINATION bin)
    endif(${QT_VERSION} EQUAL 5)
    if(${internalPython} EQUAL 1)
       INSTALL(FILES ${PYTHON_DLL} DESTINATION bin)
    endif(${internalPython} EQUAL 1)
    foreach(DLL ${OCC_DLLS})
      INSTALL(FILES ${DLL} CONFIGURATIONS Release DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${OCC_3PARTIES_DLLS})
       INSTALL(FILES ${DLL} CONFIGURATIONS Release DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${QT_DLLS_REL})
       INSTALL(FILES ${DLL} CONFIGURATIONS Release DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${QT_PLATFORMS_PLUGINS_REL})
       INSTALL(FILES ${DLL} CONFIGURATIONS Release DESTINATION bin/platforms)
    endforeach(DLL)
    foreach(DLL ${OCC_DLLS_DEB})
      INSTALL(FILES ${DLL} CONFIGURATIONS Debug DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${OCC_3PARTIES_DLLS_DEB})
       INSTALL(FILES ${DLL} CONFIGURATIONS Debug DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${QT_DLLS_DEB})
       INSTALL(FILES ${DLL} CONFIGURATIONS Debug DESTINATION bin)
    endforeach(DLL)
    foreach(DLL ${QT_PLATFORMS_PLUGINS_DEB})
       INSTALL(FILES ${DLL} CONFIGURATIONS Debug DESTINATION bin/platforms)
    endforeach(DLL)
  elseif(MINGW)
 #    if(${QT_VERSION} EQUAL 5) 
 #     INSTALL(FILES 
 #		 ${QtCore_location_Release} 
 #		 ${QtGui_location_Release} 
 #		 ${QtWidgets_location_Release}
 #		 ${QtPrintSupport_location_Release} 
 #	         ${Qt5OpenGL_location_Release} 
 #		 ${QWT_DLL} 
 #	         DESTINATION bin)
 #    endif(${QT_VERSION} EQUAL 5)
 #    foreach(DLL ${OCC_DLLS})
 #      INSTALL(FILES ${DLL} DESTINATION bin)
 #    endforeach()
 #    foreach(DLL ${OCC_3PARTIES_DLLS})
 #       INSTALL(FILES ${DLL} DESTINATION bin)
 #    endforeach()
 #    foreach(DLL ${QT_DLLS_REL})
 #       INSTALL(FILES ${DLL} DESTINATION bin)
 #    endforeach(DLL)
 #    foreach(DLL ${MINGW_DLLS})
 #       INSTALL(FILES ${DLL} DESTINATION bin)
 #    endforeach()
    foreach(DLL ${QT_PLATFORMS_PLUGINS_REL})
        INSTALL(FILES ${DLL} DESTINATION bin/platforms)
    endforeach()
    foreach(DLL ${BUNDLEDLLS})
       FILE(TO_CMAKE_PATH "${DLL}" DLLCM)
       INSTALL(FILES ${DLLCM} DESTINATION bin)
    endforeach()
  endif(MSVC)
#  foreach(DIR ${OCC_Resources})
#    INSTALL(DIRECTORY ${DIR} DESTINATION Resource)
#  endforeach(DIR)

#    foreach(DLL ${GMSH_DEPS_LIBRARIES})
#        INSTALL(FILES ${DLL} CONFIGURATIONS Release DESTINATION bin)
#    endforeach(DLL)
#    foreach(DLL ${GMSH_DEPS_LIBRARIES_DEB})
#	    INSTALL(FILES ${DLL} CONFIGURATIONS Debug DESTINATION bin)
#    endforeach(DLL)
 
 #***  Removing Administrator level from NSIS.template.in:
 set(CMAKE_NSIS_TEMPLATE "C:/msys64/mingw64/share/cmake/Modules/Internal/CPack/NSIS.template.in")
 set(PROJECT_NSIS_TEMPLATE "${CMAKE_BINARY_DIR}/_CPack_Packages/${CPACK_TOPLEVEL_TAG}/${CPACK_GENERATOR}/NSIS.template.in")
 file(READ ${CMAKE_NSIS_TEMPLATE} FILE_CONTENT)
 set (SEARCH_REGEX "Require administrator access")
 set (REPLACEMENT_TEXT "")
 string(REGEX REPLACE "${SEARCH_REGEX}" "${REPLACEMENT_TEXT}" MODIFIED_FILE_CONTENT "${FILE_CONTENT}")
 set (SEARCH_REGEX "RequestExecutionLevel admin")
 set (REPLACEMENT_TEXT "RequestExecutionLevel user")
 string(REGEX REPLACE "${SEARCH_REGEX}" "${REPLACEMENT_TEXT}" FILE_CONTENT "${MODIFIED_FILE_CONTENT}")
 file(WRITE ${PROJECT_NSIS_TEMPLATE} "${FILE_CONTENT}")
 set(CPACK_MODULE_PATH "${CMAKE_BINARY_DIR}/_CPack_Packages/${CPACK_TOPLEVEL_TAG}/${CPACK_GENERATOR}")
 
 #INSTALL(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/redistributable/vcredist_x64.exe DESTINATION tmp)
endif(WIN32)


add_subdirectory( doc/manual )
add_subdirectory( src )

set(WELCOME_FILE ${CMAKE_CURRENT_SOURCE_DIR}/WELCOME.txt)
set(LICENSE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt)
set(CREDITS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/CREDITS.txt)

set(CPACK_PACKAGE_VENDOR "Walter Steffe")
set(CPACK_PACKAGE_VERSION_MAJOR ${EmCAD_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${EmCAD_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${EmCAD_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${WELCOME_FILE})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY 
    "Client side of a client-server architecture for the modelization of electromagnetic problems")
set(CPACK_PACKAGE_FILE_NAME EmCAD-${EmCAD_VERSION_MAJOR}.${EmCAD_VERSION_MINOR}.${EmCAD_VERSION_PATCH}-${OCC_SYS_NAME})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "EmCAD")
set(CPACK_RESOURCE_FILE_LICENSE ${LICENSE_FILE})
set(CPACK_RESOURCE_FILE_README ${WELCOME_FILE})
set(CPACK_RESOURCE_FILE_WELCOME ${WELCOME_FILE})
set(CPACK_PACKAGE_EXECUTABLES emcad "EmCAD")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_SOURCE_PACKAGE_FILE_NAME emcad-${GMSH_VERSION}-source)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_CURRENT_BINARY_DIR}" "/CVS/" "/.svn" "~$")

if(UNIX)
  # make sure we remove previous installs before doing the next one
  # (on Mac for example "make package; make package_source" would lead
  # to huge file lists getting generated due to the 'Applications'
  # symlink in the bundle)
  set(CPACK_INSTALL_COMMANDS "rm -rf ${CMAKE_CURRENT_BINARY_DIR}/_CPack_Packages")
  set(CPACK_GENERATOR TGZ)
endif(UNIX)

if(WIN32)
 set(CPACK_GENERATOR NSIS)
 set(CPACK_NSIS_INSTALL_ROOT "C:")
 set(CPACK_NSIS_MODIFY_PATH ON)
 set(CPACK_NSIS_MUI_ICON    "${CMAKE_SOURCE_DIR}\\\\src\\\\GUI\\\\emcad.ico")
 set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}\\\\src\\\\GUI\\\\emcad.ico")
 set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
   CreateShortCut \\\"$DESKTOP\\\\EmCAD.lnk\\\" \\\"$INSTDIR\\\\bin\\\\emcad.exe\\\"
   ")

 set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS  
   " Delete \\\"$DESKTOP\\\\EmCAD.lnk\\\"
   ")

 set(CPACK_NSIS_CONTACT "walter.steffe@hierarchical-electromagnetics.it")


# set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
#  CreateShortCut \\\"$DESKTOP\\\\EmCAD.lnk\\\" \\\"$INSTDIR\\\\bin\\\\emcad.exe\\\"
#  nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_StandardDefaults \\\"$INSTDIR\\\\Resource\\\\StdResource\\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_XCAFDefaults \\\"$INSTDIR\\\\Resource\\\\StdResource\\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_PluginDefaults \\\"$INSTDIR\\\\Resource\\\\StdResource\\\"  '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_IGESDefaults \\\"$INSTDIR\\\\Resource\\\\XSTEPResource\\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_STEPDefaults \\\"$INSTDIR\\\\Resource\\\\XSTEPResource\\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_XmlOcafResource \\\"$INSTDIR\\\\Resource\\\\XmlOcafResource\\\" '
#   ")

#   ExecWait '$INSTDIR\\\\tmp\\\\vcredist_x64.exe'

# set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS  
#   " Delete \\\"$DESKTOP\\\\EmCAD.lnk\\\"
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_StandardDefaults \\\" \\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_XCAFDefaults \\\" \\\" '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_PluginDefaults \\\" \\\"  '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_IGESDefaults \\\" \\\"  '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_STEPDefaults \\\" \\\"  '
#   nsExec::ExecToStack /TIMEOUT=2000  '\\\"C:\\\\Windows\\\\System32\\\\setx.exe\\\"  CSF_XmlOcafResource \\\" \\\" '
#   ")

include(CPack)


endif(WIN32)

