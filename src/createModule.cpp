/**
 * Created Date: Thursday May 11th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Thursday May 11th 2023 11:00:27 pm
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
#include <iostream>
#include <string>
#include <vector>

#include <global>
#include <json.h>
#include <file.h>
#include <c.h>

void genDirectorystructure(std::string modname)
{
    system(("mkdir -p modules/" + modname).c_str());
    system(("mkdir -p modules/" + modname+"/src").c_str());
    system(("mkdir -p modules/" + modname+"/inc").c_str());
    system(("mkdir -p build/" + modname).c_str());
}

void genDefaultInc(std::string modname)
{
    std::string filename = "modules/"+modname+"/inc/"+modname+".h";
    std::string content = COPYRIGHT;
    content += "#pragma once\n\n";
    content += "extern \"C\" int init();\n";

    file f(filename);
    f.clear();
    f << content;
}

void genDefaultSrc(std::string modname)
{
    std::string filename = "modules/"+modname+"/src/main.cpp";
    std::string content = COPYRIGHT;
    content += "#include <"+modname+".h>\n\n";
    content += "extern \"C\" int init()\n{\n\treturn 0;\n}\n";

    file f(filename);
    f.clear();
    f << content;
}

int main(int argc,char** argv)
{
    if(argc != 2)
    {
        std::cout << "createModule takes 1 argument, the name of the module to be created!" << std::endl;
        exit(-1);
    }

    std::string modName = argv[1];

    json ldconfig = jsonLoad("ld.json");
    json bconfig = jsonLoad("build.json");

    json bNew(modName);
    bNew.mode = jsonmode::LIST;
    bNew << json("output","out/"+modName+".elf64");
    bNew << json("opath","build/"+modName+"/");
    bNew << json("spath","modules/"+modName+"/");
    bNew << json("lds","LDS/"+modName+".ld");
    json inc("inc");
    json args("args");
    inc.mode = jsonmode::LIST;
    args.mode = jsonmode::LIST;
    bNew << inc;
    bNew << args;

    json ldNew(modName);
    json msec("msec");
    json extra("extra");
    json mods("mods");
    json emsec("msec");
    msec.mode = jsonmode::LIST;
    extra.mode = jsonmode::LIST;
    mods.mode = jsonmode::LIST;
    emsec.mode = jsonmode::LIST;
    ldNew.mode = jsonmode::LIST;
    msec << json("text","0x0000000000000000");
    msec << json("data","0x0000000000000000");
    msec << json("rodata","0x0000000000000000");
    msec << json("bss","0x0000000000000000");
    emsec << "/DISCARD/";
    ldNew << json("prefix","");
    ldNew << json("suffix","");

    ldNew << msec;
    extra << mods;
    extra << emsec;
    ldNew << extra;

    ldconfig["kmods"] << ldNew;
    bconfig["kmods"] << bNew;

    ldconfig.save("ld.json");
    bconfig.save("build.json");

    genDirectorystructure(modName);
    genDefaultInc(modName);
    genDefaultSrc(modName);
}

//get implementation
#include "file.cpp"
#include "json.cpp"
#include "error.cpp"
