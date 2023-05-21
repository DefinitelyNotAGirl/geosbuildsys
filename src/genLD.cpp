/**
 * Created Date: Tuesday May 9th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Tuesday May 9th 2023 4:07:30 pm
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
#include <ld.h>

void linker::script::write(std::string File)
{
    file f(File);

    f.clear();
    f << autocr;
    if(this->format == oformat::BINARY)
        f << "OUTPUT_FORMAT(\"binary\")\n";
    
    f << "ENTRY("+this->entry+")\n";
    f << "SECTIONS {\n";
    std::vector<std::string> secdone;
    for(linker::section I : this->sections)
    {
        for(std::string sd : secdone)
            if(sd == I.name)
                goto skipLoop;
        {
            std::string sec;
            if(I.start != "")
                sec += "\t. = "+I.start+";\n";
            sec += "\t"+I.name+" : {\n";
            for(std::string II : I.input)
                sec+="\t\t"+II+"\n";
            sec+="\t}\n";
            f << sec;
            secdone.push_back(I.name);
        }
        skipLoop: ;
    }
    f << "}\n";
};

static std::vector<linker::section> getMMS(json& mod)
{
    std::string prefix = mod["prefix"];
    std::string suffix = mod["suffix"];

    std::vector<linker::section> res;
    json& ms = mod["msec"];

    std::string opath = bconfig["kmods"][mod.data]["opath"];

    //text
    linker::section text;
    text.name = prefix+"text"+suffix;
    text.input.push_back(opath+"*.o (.text)");
    text.start = ms["text"];

    //data
    linker::section data;
    data.name = prefix+"data"+suffix;
    data.input.push_back(opath+"*.o (.data)");
    data.start = ms["data"];

    //rodata
    linker::section rodata;
    rodata.name = prefix+"rodata"+suffix;
    rodata.input.push_back(opath+"*.o (.rodata)");
    rodata.start = ms["rodata"];

    //bss
    linker::section bss;
    bss.name = prefix+"bss"+suffix;
    bss.input.push_back(opath+"*.o (.bss)");
    bss.start = ms["bss"];

    res.push_back(text);
    res.push_back(data);
    res.push_back(rodata);
    res.push_back(bss);

    //extra
    for(json& m : mod["extra"]["mods"])
        for(linker::section I : getMMS(ldconfig["kmods"][m]))
            res.push_back(I);

    for(json& m : mod["extra"]["msec"])
        for(linker::section I : linker::sex)
            if(I.name == std::string(m))
                res.push_back(I);

    return res;
}

static void genLD_MOD(json& mod)
{
    linker::script script;

    script.entry = "init";
    script.format = linker::oformat::BINARY;
    script.sections = getMMS(mod);

    script.write(bconfig["kmods"][mod.data]["lds"]);
}

void genLD()
{
    linker::PAGESIZE = ldconfig["PAGESIZE"];

    //gather misc memory sections
    for(json& sec : ldconfig["msec"])
    {
        linker::section s;
        s.name = sec.data;
        s.start = sec["base"];
        for(json& I : sec["input"])
            s.input.push_back(I);

        linker::sex.push_back(s);
    }

    //go through modules
    for(json& I : ldconfig["kmods"])
    {
        std::cout << "\t\t" << I.data <<"..."<< std::endl;
        genLD_MOD(I);
    }
}