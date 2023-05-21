/**
 * Created Date: Thursday May 4th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Thursday May 4th 2023 12:27:18 pm
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

json::json(std::string n)
    :data(n){}

json::json()
    :data(""){}

json::json(std::string name,std::string value)
    :data(name),strv(value),mode(jsonmode::SINGLE){}

json::json(std::string name,std::vector<json> values)
    :data(name),list(values),mode(jsonmode::LIST){}

json::json(std::string name,std::vector<std::string> values)
    :data(name),mode(jsonmode::LIST)
{
    for(std::string I : values)
    {
        this->list.push_back(json(I));
        this->list.back().mode = jsonmode::EMPTY;
    }
}

void jsonLoadTxt(json* output, char** content)
{
    char contentMode = **content;
    //std::cout << **content;

    if(contentMode == '{')
    {
        //std::cout << "list json element started" << std::endl;
        output->mode = jsonmode::LIST;
        rst_123: ;
        //contentMode = {}
        std::string working = "";
        bool rd = false;
        do
        {
            skipChar_123: ;
            (*content)++;
            //std::cout << **content;
            if((**content == ' ' || **content == '\n') && !rd)
                goto skipChar_123;
            else if(**content == '"' && !rd)
                rd = true;
            else if((**content == '}' || **content == ']') && !rd)
                goto ret_123;
            else if(**content == '"' && rd)
                rd = false;
            else if(rd && (**content == '\\'))
            {
                (*content)++;
                switch(**content)
                {
                    case('t'):
                        working.push_back('\t');
                        break;
                    case('n'):
                        working.push_back('\n');
                        break;
                    default:
                        working.push_back(**content);
                }
            }
            else if(rd)
                working.push_back(**content);
            else
                error("(0) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        } while(rd);
        (*content)++;
        while(**content == ' ' || **content == ':')
        {
            //std::cout << **content;
            (*content)++;
        }
        while(**content == ' ')
        {
            //std::cout << **content;
            (*content)++;
        }
        if(**content == '{' || **content == '[' || **content == '"')
        {
            json d(working);
            output->list.push_back(d);
            jsonLoadTxt(&(output->list.back()),content);
            while(**content == ' ' || **content == '\n')
                (*content)++;
            if(**content == ',')
                goto rst_123;
            else if(**content == '}')
            {
                (*content)++;
                return;
            }
            else
                error("(1) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        }
        else
            error("(2) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        ret_123: ;
        while(**content == ' ' || **content == '\n')
            (*content)++;
        if(**content != '}')
            error("(10) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        (*content)++;
    }
    else if(contentMode == '[')
    {
        //std::cout << "single list json element started" << std::endl;
        //contentMode = []
        output->mode = jsonmode::LIST;
        rst_91: ;
        bool rd = false;
        bool fo = true;
        while(fo || **content == ',')
        {
            if(fo)
                fo = false;
            std::string working = "";
            do
            {
                skipChar_91: ;
                (*content)++;
                //std::cout << **content;
                if((**content == ' ' || **content == '\n') && !rd)
                    goto skipChar_91;
                else if(**content == '"' && !rd)
                    rd = true;
                else if(**content == ']' && !rd)
                {
                    (*content)++;
                    return;
                }
                else if(**content == '"' && rd)
                    rd = false;
                else if(rd && (**content == '\\'))
                {
                    (*content)++;
                    switch(**content)
                    {
                        case('t'):
                            working.push_back('\t');
                            break;
                        case('n'):
                            working.push_back('\n');
                            break;
                        default:
                            working.push_back(**content);
                    }
                }
                else if(rd)
                    working.push_back(**content);
                else
                    error("(8) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
            } while(rd);
            json d;
            d.data = working;
            d.mode = jsonmode::EMPTY;
            output->list.push_back(d);
            (*content)++;
        }
        while(**content == ' ' || **content == '\n')
            (*content)++;
        if(**content != ']')
            error("(9) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        (*content)++;
    }
    else if(contentMode == '"')
    {
        //std::cout << "single json element started" << std::endl;
        //contentMode = ""
        output->mode = jsonmode::SINGLE;
        bool rd = true;
        std::string working = "";
        do
        {
            skipChar_34: ;
            (*content)++;
            //std::cout << **content;
            if((**content == ' ' || **content == '\n') && !rd)
                goto skipChar_34;
            else if(**content == '"' && !rd)
                rd = true;
            else if(**content == '"' && rd)
                rd = false;
            else if(rd && (**content == '\\'))
            {
                (*content)++;
                switch(**content)
                {
                    case('t'):
                        working.push_back('\t');
                        break;
                    case('n'):
                        working.push_back('\n');
                        break;
                    default:
                        working.push_back(**content);
                }
            }
            else if(rd)
                working.push_back(**content);
            else
                error("(6) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
        } while(rd);
        output->strv = working;
        (*content)++;
    }
    else
        error("(7) json syntax error, unexpected '"+std::string(1,*content[0])+"' ("+std::to_string((int8_t)**content)+")"+"("+std::to_string((uint64_t)*content)+")");
}

json jsonLoad(std::string path)
{
    file f(path);
    char* data;
    uint64_t size = f >> &data;

    char* dataBackup = data;

    json master("");
    jsonLoadTxt(&master,&data);
    memset(dataBackup,0,size);
    free(dataBackup);
    return master;
}

void json::saveSub(std::stringstream& out,int indent)
{
    if(this->mode != jsonmode::EMPTY)
        for(uint64_t I = 0;I<indent;I++)out << OUT_TAB;
    if(!this->data.empty())
    {
        if(this->mode == jsonmode::LIST)
        {
            if(this->list.empty() || this->list.back().mode == jsonmode::EMPTY)
                out << "\"" << this->data << "\"" << " : [";
            else
                out << "\"" << this->data << "\"" << " : {" << std::endl;
        }
        else if(this->mode == jsonmode::SINGLE)
            out << "\"" << this->data << "\"" << " : ";
        else if(this->mode == jsonmode::EMPTY)
            out << "\"" << this->data << "\"";
        else
            error("invalid json mode");
    }
    else
    {
        out << "{" << std::endl;
    }
    if(this->mode == jsonmode::LIST)
    {
        for(json& I : this->list)
        {
            I.saveSub(out,indent+1);
            if(I.data != this->list.back().data)
            {
                out << ",";
            }
            if(I.mode != jsonmode::EMPTY)
                out << std::endl;
        }
        if(!this->list.empty() && this->list.back().mode != jsonmode::EMPTY)
            for(uint64_t I = 0;I<indent;I++)out << OUT_TAB;
        if(this->list.empty() || this->list.back().mode == jsonmode::EMPTY)
            out << "]";
        else
            out << "}";
    }
    else if(this->mode == jsonmode::SINGLE)
    {
        out << "\"" << this->strv << "\"";
    }
}

void json::save(std::string File)
{
    std::stringstream out;
    this->saveSub(out,0);

    file f(File);
    f.clear();
    f << out.str();
    out.clear();
}

void jsonPrint(json& d,uint64_t indent)
{
    for(uint64_t I = 0;I<indent;I++)std::cout << OUT_TAB;
    if(!d.data.empty())
    {
        if(d.mode == jsonmode::LIST)
            std::cout << "\"" << d.data << "\"" << " : {" << std::endl;
        else if(d.mode == jsonmode::SINGLE)
            std::cout << "\"" << d.data << "\"" << " : ";
        else if(d.mode == jsonmode::EMPTY)
            std::cout << "\"" << d.data << "\"" << std::endl;
        else
            error("invalid json mode");
    }
    else
    {
        std::cout << "{" << std::endl;
    }
    if(d.mode == jsonmode::LIST)
    {
        for(json& I : d.list)
            jsonPrint(I,indent+1);
        for(uint64_t I = 0;I<indent;I++)std::cout << OUT_TAB;
        std::cout << '}' << std::endl;
    }
    else if(d.mode == jsonmode::SINGLE)
    {
        std::cout << "\"" << d.strv << "\"" << std::endl;
    }
}

json& json::operator[](std::string index)
{
    if(this->mode != jsonmode::LIST)
    {
        std::cerr << "cant use index operators on non-list json object \"" << this->data << "\"" << std::endl;
        exit(-2);
    }

    for(json& I : this->list)
    {
        if(I.data == index)
            return I;
    }
    std::cerr << "object \"" << index << "\" not found in \"" << this->data << "\"" << std::endl;
    exit(-3);
}

json& json::operator[](uint64_t index)
{
    if(this->mode != jsonmode::LIST)
    {
        std::cerr << "cant use index operators on non-list json object \"" << this->data << "\"" << std::endl;
        exit(-2);
    }

    return this->list[index];
}

json::operator std::string()
{
    if(this->mode == jsonmode::SINGLE)
        return this->strv;
    if(this->mode == jsonmode::EMPTY)
        return this->data;
    std::cerr << "cant convert list json object \"" << this->data << "\" into string" << std::endl;
        exit(-4);
}

void json::operator<<(json j)
{
    this->list.push_back(j);
}

void json::operator<<(std::string v)
{
    if(this->mode != jsonmode::LIST)
    {
        std::cerr << "cant add to non-list json objects" << std::endl;
        exit(-1);
    }

    json d;
    d.mode = (!this->list.empty()) ? this->list.back().mode : jsonmode::EMPTY;
    if(d.mode == jsonmode::SINGLE)
        d.strv = v;
    else
        d.data = v;

    this->list.push_back(d);
}
void json::operator<<(uint64_t v){this->operator<<(std::to_string(v));}
void json::operator<<(int64_t v){this->operator<<(std::to_string(v));}
void json::operator<<(double v){this->operator<<(std::to_string(v));}
void json::operator<<(float v){this->operator<<(std::to_string(v));}

std::vector<json>::iterator json::begin(){return this->list.begin();}
std::vector<json>::iterator json::end(){return this->list.end();}