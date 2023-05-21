/**
 * Created Date: Thursday May 4th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Thursday May 4th 2023 1:52:53 am
 * Modified By: DefinitelyNotAGirl@github (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <json.h>
#include <file.h>
#include <misc.h>

static std::string genDEFINE(std::string name,uint64_t value)
{
    return "#define "+name+" "+std::to_string(value)+"\n";
}

static std::string genDEFINE(std::string name,int64_t value)
{
    return "#define "+name+" "+std::to_string(value)+"\n";
}

static std::string genDEFINE(std::string name,std::string value)
{
    return "#define "+name+" \""+value+"\"\n";
}

void genHeader_BUILD()
{
    file f("inc/BUILD.h");
    f.clear();
    f << autocr;
    f << genDEFINE("GEOS_BUILD_NUM",BUILD_NUM);
    f << genDEFINE("GEOS_BUILD_DATE",DATE);
    f << genDEFINE("GEOS_BUILDER",BUILDER);
    f << "\n";
}