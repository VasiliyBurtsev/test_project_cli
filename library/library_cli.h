#ifndef library_cli_h_
#define library_cli_h_
#define REST_API 
#define method 
#define NULL nullptr
#include <iostream>
#include "string.h"
#include <list>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
using namespace std;
using namespace web;
using namespace pplx;
struct package
{
    string name;
    int epoch;
    string version;
    string release;
    string arch;
    string disttag;
    int buildtime;
    string source;
};
struct list_json
{
    string branch;
    list<package> pack;
};
struct packbranch
{
    string branch;
    package pack;
};


extern list_json readJSONAPI(string url,string branch, string arch,string method);
extern void writeListJSON(string json_str,string branch);
extern bool IsReleaseMore(package pack1, package pack2);
extern bool IsElemNotInList(list<package> str_list, package pack);
extern list<packbranch> ResListPack (list_json lpack1,list_json lpack2);
#endif 
