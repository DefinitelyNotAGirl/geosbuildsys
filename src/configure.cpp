/**
 * Created Date: Wednesday May 3rd 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Monday May 22nd 2023 4:03:46 am
 * Modified By: DefinitelyNotAGirl@github (definitelynotagirl115199@gmail.com)
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

#ifdef __linux__
#include <cstring>
#endif

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <stdio.h>
#include <time.h>

//get headers
#include <global>
#include <file.h>
#include <json.h>
#include <misc.h>

//fetch implementation
#include "genMake.cpp"
#include "genLD.cpp"
#include "genhead.cpp"
#include "file.cpp"
#include "json.cpp"
#include "error.cpp"

int main(int argc,char** argv)
{
    std::cout << "starting GEOS build..." << std::endl;//entry msg

    //get version number
    file VERSION_FILE("VERSION_FILE");
    char* ver;
    VERSION_FILE >> &ver;
    BUILD_NUM = std::stoull(ver);
    VERSION_FILE.clear(); 
    VERSION_FILE << std::to_string(BUILD_NUM+1);
    free(ver);

    //get date
    time_t timer;
    char buffer[11];
    struct tm* tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(buffer, 10, "%Y-%m-%d", tm_info);
    DATE = buffer;

    //generate headers
    std::cout << "\tgenerating headers..." << std::endl;
    std::cout << "\t\tBUILD.h..." << std::endl;   
    genHeader_BUILD();

    //reading json
    ldconfig = jsonLoad("ld.json");
    bconfig = jsonLoad("build.json");

    //generate linker scripts
    std::cout << "\tgenerating linker scripts..." << std::endl;
    genLD();

    //generate make files
    std::cout << "\tgenerating make files..." << std::endl;
    genMake();

    //done
    std::cout << "GEOS build finished" << std::endl;
    return 0;
}
