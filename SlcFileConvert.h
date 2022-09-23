#pragma once
#undef __EDG__ // EE Bentley

#include<Mstn/MdlApi/MdlApi.h>

#include <iostream>
#include <fstream>

class SlcFileConvert
{
private:
    WCharCP file_name_out;
    WCharCP file_name_in;
public:
    SlcFileConvert(WCharCP file_name_out, WCharCP file_name_in, Bentley::LangCodePage code_page)
        :file_name_out(file_name_out)
        , file_name_in(file_name_in)
    {};
    virtual ~SlcFileConvert()
    {};
    bool Convert();
};
