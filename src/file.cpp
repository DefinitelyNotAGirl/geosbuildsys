/**
 * Created Date: Thursday May 4th 2023
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

#include <file.h>

static inline bool fileExists (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

file::file(std::string path)
    :path(path){}

void file::clear()
{
    std::ofstream f(this->path);
    f << "";
    f.close();
}

uint64_t file::operator>>(char** buffer)
{
    FILE* f = fopen(this->path.c_str(), "r");

    // Determine file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    *buffer = (char*)malloc(size);

    //std::cout << "# operator>>" << std::endl;
    //std::cout << "size: "<< size << std::endl;

    rewind(f);
    fread(*buffer, sizeof(char), size, f);
    fclose(f);

    //std::cout << "path: "<< this->path << std::endl;
    //std::cout << "buffer: \n"<< *buffer << std::endl;
    return size;
}

void file::operator<<(std::string content_)
{
    FILE* f = fopen(this->path.c_str(), "r");
	if(f == NULL)
	{
		std::cout << "fopen call failed" << std::endl;
		std::cout << "path: " << this->path.c_str() << std::endl;
	}

    // Determine file size
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    char* buffer = (char*)malloc(size);

    rewind(f);
    fread(buffer, sizeof(char), size, f);
    fclose(f);

    const char* content = content_.c_str();
    uint64_t nsize = strlen(content);
    char* _content = (char*)malloc(size+nsize+1);
    memcpy(_content,buffer,size);
    memcpy(_content+size,content,nsize);
    *((char*)(_content+size+nsize)) = 0x00;

    std::ofstream ofs(this->path);
    ofs << _content;
    ofs.close();

    //if this function gets called in loop not clearing the content may end up causing issues such as left over content in memory that then ends up in the file
    memset(buffer,0,size);
    memset(_content,0,size+nsize);

    free(buffer);
    free(_content);
}