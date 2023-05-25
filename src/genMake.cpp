/**
 * Created Date: Thursday May 11th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Thursday May 25th 2023 11:06:54 am
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

#include <vector>
#include <json.h>
#include <misc.h>
#include <file.h>
#include <gSystem.h>

namespace make
{
    std::vector<std::string> incMake;

    std::string CC;
    std::string CXX;
    std::string AS;
    std::string LD;

    void genMakeMod(json& mod)
    {
        //
        //generate makefile for external build
        //
        incMake.push_back(std::string(::bconfig["makedir"])+mod.data+".module.mak");
        file Fexternal(std::string(::bconfig["makedir"])+mod.data+".module.mak");

        std::string fileCollector = "";
        std::string include = "";
        std::string rules = "";
        std::string srcdir = std::string(mod["spath"])+"src/";
        std::string builddir = std::string(mod["opath"]);

        //collect args
        std::string ARGS_CXX = "";
        std::string ARGS_ASM = "";
        std::string ARGS_LD = "";
        std::string ARGS_C = "";
        for(json& I : mod["args"])
        {
            json& args = ::bconfig["args"][I];
            for(json& arg : args["as"])
                ARGS_ASM+=' '+std::string(arg);
            for(json& arg : args["c++"])
                ARGS_CXX+=' '+std::string(arg);
            for(json& arg : args["ld"])
                ARGS_LD+=' '+std::string(arg);
            for(json& arg : args["c"])
                ARGS_C+=' '+std::string(arg);
        }
        //get include args
        for(json& I : mod["inc"])
        {
            for(json& inc : ::bconfig["include"][I])
            {
                ARGS_CXX+=" -I"+std::string(inc);
                ARGS_C+=" -I"+std::string(inc);
            }
        }
        ARGS_CXX+=" -Imodules/"+mod.data+"/inc";
        ARGS_C+=" -Imodules/"+mod.data+"/inc";

        std::string gsystem = std::string(GS_ARCH)+"-"+std::string(GS_OS);
        if(::bconfig["args"]["system"].hasEntry(gsystem))
        {
            json& args = ::bconfig["args"]["system"][gsystem];
            for(json& arg : args["as"])
                ARGS_ASM+=' '+std::string(arg);
            for(json& arg : args["c++"])
                ARGS_CXX+=' '+std::string(arg);
            for(json& arg : args["ld"])
                ARGS_LD+=' '+std::string(arg);
            for(json& arg : args["c"])
                ARGS_C+=' '+std::string(arg);
        }
        else
        {
            std::cout << "unsupport system: " << GS_ARCH-GS_OS << std::endl;
            exit(-3);
        }
        //
        //generate file collector
        //
        std::vector<std::string> objlists;
        for(json& stype : ::bconfig["srctypes"])
        {
            for(json& ext : stype)
            {
                fileCollector+=std::string(mod.data)+"_SOURCE_"+stype.data+"_"+std::string(ext)+"=$(wildcard "+srcdir+"*."+std::string(ext)+")\n";
                fileCollector+=std::string(mod.data)+"_OBJECTS_"+stype.data+"_"+std::string(ext)+"=$(patsubst "+srcdir+"%."+std::string(ext)+","+builddir+"%.o,$("+std::string(mod.data)+"_SOURCE_"+stype.data+"_"+std::string(ext)+"))\n";
                fileCollector+=std::string(mod.data)+"_DEPFILES_"+stype.data+"_"+std::string(ext)+"=$(patsubst "+srcdir+"%."+std::string(ext)+","+builddir+"%.d,$("+std::string(mod.data)+"_SOURCE_"+stype.data+"_"+std::string(ext)+"))\n";
                include+="-include $("+std::string(mod.data)+"_DEPFILES_"+stype.data+"_"+std::string(ext)+")\n";

                objlists.push_back(std::string(mod.data)+"_OBJECTS_"+stype.data+"_"+std::string(ext));

                rules+=builddir+"%.o: "+srcdir+"%."+std::string(ext)+'\n';
                if(stype.data == "cpp")
                    rules+="\t@"+CXX+ARGS_CXX+" -c -o $@ $<\n\t$(info  \tC++\t$<)\n";
                else if(stype.data == "asm")
                    rules+="\t@"+AS+ARGS_ASM+" -c -o $@ $<\n\t$(info  \tAS\t$<)\n";
                else if(stype.data == "c")
                    rules+="\t@"+CC+ARGS_C+" -c -o $@ $<\n\t$(info  \tCC\t$<)\n";
            }
        }
        //
        //generate clean rule
        //
        rules+="m-"+mod.data+"-clean:\n";
        rules+="\t@-rm -r "+builddir+"*.o\n\t$(info  \tDELETE\t"+builddir+"*.o)\n";
        rules+="\t@-rm -r "+builddir+"*.d\n\t$(info  \tDELETE\t"+builddir+"*.d)\n";
        //
        //generate module build rule
        //
        std::string OUTPUT = mod["output"];
        rules+="m-"+mod.data+": "+OUTPUT+"\n";
        rules+=OUTPUT+": ";
        for(std::string I : objlists)
            rules+=" $("+I+')';
        rules+='\n';

        //write to file
        Fexternal.clear();
        Fexternal << autocrmk + fileCollector + rules + include;

        //
        //generate makefile for internal build
        //
        file Finternal(std::string(((json&)mod)["spath"])+"makefile");
        std::string internal = autocrmk;
        internal+="all: \n\t$(MAKE) -C "+std::string(::bconfig["PDMO"])+" m-"+mod.data+'\n';
        internal+="clean: \n\t$(MAKE) -C "+std::string(::bconfig["PDMO"])+" m-"+mod.data+"-clean\n";
        Finternal.clear();
        Finternal << internal;

        memset((void*)fileCollector.c_str(),0,fileCollector.size());
        memset((void*)rules.c_str(),0,rules.size());
        memset((void*)include.c_str(),0,include.size());
    }

    void genMakeMods()
    {
        incMake.push_back(std::string(::bconfig["makedir"])+"modules.mak");
        file GMMF(std::string(::bconfig["makedir"])+"modules.mak");
        std::string gmmfc = autocrmk;
        gmmfc+="m-all:";
        for(json& I : ::bconfig["kmods"])
        {
            gmmfc+=" m-"+I.data;
            std::cout << "\t\t\t" << I.data << "..." << std::endl;
            genMakeMod(I);
        }
        gmmfc+="\n";

        //write to file
        GMMF.clear();
        GMMF << gmmfc;
    }

    void genMakeInc()
    {
        file minc(std::string(::bconfig["makedir"])+"include.mak");
        std::string content = autocrmk;
        for(std::string I : incMake)
            content+="include "+I+"\n";

        minc.clear();
        minc << content;
    }

    void genMakeUtil()
    {
        CC = bconfig["util"]["cc"];
        CXX = bconfig["util"]["cxx"];
        AS = bconfig["util"]["as"];
        LD = bconfig["util"]["ld"];
    }

    void genMakeModInc()
    {
        incMake.push_back(std::string(::bconfig["makedir"])+"modinclude.mak");
        file modinc(std::string(::bconfig["makedir"])+"modinclude.mak");

        std::string mincat = "modinc-all: ";
        std::string modinccontent = autocrmk;
        for(json& I : ::bconfig["kmods"])
        {
            if(I.data == "klib")
                continue; //skip this for klib (modules/klib/inc is part of the include dirs)
            modinccontent+=std::string(::bconfig["incdir"])+I.data+": "+"modules/"+I.data+"/inc/"+I.data+".h\n\t cp $< $@\n\t$(info  \tCOPY\t$<)\n";
            mincat+=std::string(::bconfig["incdir"])+I.data+" ";
        }
        mincat+="\n";
        modinccontent+=mincat;

        modinc.clear();
        modinc << modinccontent;
    }
}

void genMake()
{
    std::cout << "\t\tGathering basic config..." << std::endl;
    make::genMakeUtil();
    std::cout << "\t\tGenerating Module related make files..." << std::endl;
    make::genMakeMods();
    std::cout << "\t\tGenerating modinclude.mak..." << std::endl;
    make::genMakeModInc();
    std::cout << "\t\tGenerating include.mak..." << std::endl;
    make::genMakeInc();
}