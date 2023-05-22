/*
 * Created Date: Thursday May 4th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Thursday May 4th 2023 1:50:48 am
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
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

enum class jsonmode
{
    UNINITIALIZED,LIST,SINGLE,EMPTY
};

class json
{
private:
    void saveSub(std::stringstream& out,int indent);
public:
    json();
    json(std::string n);
    json(std::string name,std::string value);
    json(std::string name,std::vector<std::string> values);
    json(std::string name,std::vector<json> values);
    std::vector<json> list;//filled on LIST mode
    std::string strv;//filled on SINGLE mode
    std::string data;//filled on all modes
    jsonmode mode = jsonmode::UNINITIALIZED;

    void save(std::string File);

    json& operator[](std::string index);
	json& operator[](uint64_t index);

    std::vector<json>::iterator end();
    std::vector<json>::iterator begin();

    operator std::string();

    void operator<<(json j);
    void operator<<(std::string v);
    void operator<<(uint64_t v);
    void operator<<(int64_t v);
    void operator<<(double v);
    void operator<<(float v);
};

void jsonPrint(json& d,uint64_t indent = 0);
json jsonLoad(std::string path);
void jsonLoadTxt(json* output, char** content);
