#ifndef library_cli_h_
#define library_cli_h_
#define REST_API "https://rdb.altlinux.org"
#define method "export/branch_binary_packages"
#include <iostream>
#include "string.h"
#include <locale>
#include <vector>
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
using namespace std;
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
constexpr auto k_max_buffer_size = 4096;

extern list_json readJSONAPI(string url,string branch, string arch,string method);
extern void writeListJSON(string json_str,string branch);
extern bool IsReleaseMore(package pack1, package pack2);
extern bool IsElemNotInList(list<package> str_list, package pack);
extern list<packbranch> ResListPack (list_json lpack1,list_json lpack2);
#endif 
