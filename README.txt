This is EmCAD, the client side of side of an electromagnetic 
modeler which is delivered as a cloud based service.

EmCAD is copyright (C) 2015 Walter Steffe` and is distributed under 
the terms of the GNU General Public License, Version 2 or
later. See LICENSE.txt and CREDITS.txt for more information.

For documentation see the reference manual located in doc/.

--------------------------------
Dependencies:

* Building EmCAD from its source code on a Linux platform requires
a C++ compiler, CMake (http://cmake.org) and the following installed
softwares:
-Qt library >=4.8
-Opencascade library >=6.8 or OCE library >=0.17
-Gmsh library >= 2.9.3 (patched)
-Boost library >=1.54
-Qwt library =6.1.2

* Building EmCAD from its source code on a Windows platform requires
the minGW C++ compiler, CMake (http://cmake.org) and the following
installed softwares:
-Qt library >=4.8
-OCE library >=0.17
-Gmsh library >= 2.9.3 (patched)
-Boost library >=1.54
-Qwt library =6.1.2
-python 2.7 with cxfreeze and boto modules
-python 3.4 with cxfreeze module

* Running EmCAD client on a Linux platform requires and the following preinstalled softwares:
-python 2.7
-python-boto

--------------------------------
Building EmCAD from the command line on a Linux platform:

1) Create a build directory, for example as a subdirectory of Gmsh's source
  directory:

    mkdir build

2) Run cmake from within the build directory, pointing to EmCAD's source
  directory:

    cd build
    cmake ..

3) Adjust the build options by running "ccmake" from within the build directory
  and pointing to EmCAD's source:

    ccmake ..

  set the variable CMAKE_INSTALL_PREFIX to the desired install directory
  For example you can set:  CMAKE_INSTALL_PREFIX=/opt/local.

4) Enter "c" command (configure)

  At this point cmake will generate error messages if some of the required 
  libraries or related include files have not been found.  
  This problem may be originated because the library (or the include file)
  is not present at all or because it is located in a folder that is not included 
  in the search paths of cmake. In the first case you should install the missing
  library.  In the latter you may solve the problem by adding the folder path to
  the environment variable CMAKE_PREFIX_PATH.

  The "c" command must be run again after having fixed the problem.

* Enter "g" command (generate)

* Build and install EmCAD by typing:

    make
    make install

* To see a detailed compilation log use

    make VERBOSE=1



