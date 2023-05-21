#
# Created Date: Thursday May 4th 2023
# Author: DefinitelyNotAGirl@github
# -----
# Last Modified: Thursday May 4th 2023 1:48:32 am
# Modified By: DefinitelyNotAGirl@github (definitelynotagirl115169@gmail.com)
# -----
# Copyright (c) 2023 DefinitelyNotAGirl@github
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

ARGS := -Wno-c++11-compat-deprecated-writable-strings -Wno-writable-strings
all:
	c++ -std=c++20 $(ARGS) src/configure.cpp -o configure -Iinclude
	c++ -std=c++20 $(ARGS) src/createModule.cpp -o createModule -Iinclude
	cp configure ../geos/configure
	cp createModule ../geos/createModule

clean:
	rm build/*