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

import sys

build_exe_options = { "packages": ["os", "sys", "time", "fnmatch", "json", "boto", "email"], 
                      "include_files":[(sys.prefix+'/Lib/site-packages/boto/cacerts/cacerts.txt', 'cacerts.txt')],
		      "optimize": 2}
setup(
        name = "aws_scripts",
        version = "0.1",
        description = "cx_Freeze script",
	options = {"build_exe": build_exe_options},
        executables = [
		       Executable("aws_analyse.py"),
		       Executable("aws_filtermap.py"),
		       Executable("aws_symmfilter.py"),
		       Executable("aws_getcredit.py"),
		       Executable("aws_modelize1.py"),
		       Executable("aws_modelize1_get.py"),
		       Executable("aws_modelize1_send.py"),
		       Executable("aws_modelizeb.py"),
		       Executable("aws_modelizeb_get.py"),
		       Executable("aws_modelizeb_send.py"),
		       Executable("aws_modelizep.py"),
		       Executable("aws_modelizep_get.py"),
		       Executable("aws_modelizep_send.py"),
		       Executable("aws_reduce.py")
		       ]
        )

