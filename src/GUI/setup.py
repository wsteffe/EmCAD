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


from cx_Freeze import setup, Executable

build_exe_options = {"packages": ["os", "sys", "tarfile"], "optimize": 2}

setup(
        name = "tar_scripts",
        version = "1.0",
        description = "cx_Freeze script",
	options = {"build_exe": build_exe_options},
        executables = [
		       Executable("remove_dir.py"),
		       Executable("tar_create.py"),
		       Executable("tar_extract.py"),
		       ]
        )

