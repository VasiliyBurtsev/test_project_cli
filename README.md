# CLI_json_binary_packages
Development of a library and a test CLI utility that compares the lists of received binary packages of two proposed branches.

There is this REST API for the database: https://rdb.altlinux.org/api/
There is a method /export/branch_binary_packages/{branch} , which lists the binary packages of a particular branch as a JCON object ( as a brunch you can use: sisyphus, p10, p9, etc..)
The method requests two parameters 
  branch - branch name
  ar - architecture name of the given branch
If you do not specify an architecture name, then a list of binary packages for all architectures supported by the given branch will be displayed.
The data structure of the binary package is as follows
{
   "request_args": {},
   length: 0
   "packages": [
     {
       "name": "string",
       "epoch": 0,
       "version": "string",
       "release": "string",
       "arch": "string",
       "disttag": "string",
       buildtime: 0
       "source": "string"
     }
   ]
}

The shared library is compiled according to the data recommendations:
https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html
And contains the following
library_cli.h
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

The library_sli.сpp library code contains functions that read, compare and display JSON objects based on created structures.
//The function collects an HTTP request, outputs a JSON object and saves a list of binary packages of a certain branch (branch) for further work
list_json readJSONAPI(const string url,string branch, string arch, const string method){
        // Создаём клиент и привязываем к домену. Туда пойдут наши запросы
      string full_url = url+"/api/"+method+"/"+branch;
      string full_url_s;
      if (arch != null || arch != ""){
        full_url_s = full_url + "?arch="+arch;
      }
      else {
        full_url_s = full_url;
      }
    uri url(full_url_s);
    http_client client(url);
    http_request req;

    req.set_method(methods::GET);
    pplx::task<json::value> requestTask = client.request(req).then([](http_response response)
    {
        json::value jsonObject;
        try
        {
            if ( response.status_code() == status_codes::OK )
            {
                response.headers().set_content_type(L"application/json"); // Set headers to receive content type as JSON 
                jsonObject = response.extract_json().get();
            }
        }
        catch (const http_exception& e)
        {
            cout << e.error_code().message() << "\n";
        }     
        return jsonObject; // returned a json value    
    });
    json::array packages = requestTask.get().at(L"packages").as_array(); // We get the returned response here
    list_json js;
    js.branch = branch;
    js.pack = packages;
    return js;    	
}
// The function displays the contents of the read object
void writeListJSON(list_json ljs){
    
   // Достаём значения
  ljs.branch = branch;
  cout << "branch " << ljs.branch << endl;
   
  cout << "{ "<< endl;
  for (package p : ljs.pack){
	  cout << "name " << p.name<< endl;
  	  cout << "epoch " << p.epoch<< endl;
  	  cout << "version " << p.version<< endl;
  	  cout << "release " << p.release<< endl;
  	  cout << "arch " << p.arch<< endl;
  	  cout << "disttag " << p.disttag<< endl;
  	  cout << "buildtime " << p.buildtime<< endl;
  	  cout << "source " << p.source<< endl;
  }
  cout << "}"<< endl;  
}
//Data comparison functions according to the conditions of the task
bool IsVersionReleaseMore(package pack1, package pack2){
	return (pack1.name == pack2.name) && (pack1.version > pack2.version) && (pack1.release > pack2.release);
}
bool IsElemNotInList(list<package> str_list, package pack){
    return find (str_list.begin(), str_list.end(), pack) == str_list.end();
}
// The function collects the result of the utility in the form of a JSION object
list<packbranch> ResListPack (list_json lpack1,list_json lpack2){
    list<packbranch> resl;
    packbranch pb;
    for (package p : lpack1.pack){
        if (IsElemNotInList(lpack2,p)){
            pb.branch = lpack1.branch;
            pb.pack = p;
            resl.push_back(pb);
        }
        for (package p2 : lpack2.pack){
        if (IsVersionReleaseMore(p,p2)){
            pb.branch = lpack1.branch;
            pb.pack = p;
            resl.push_back(pb);
        }
        }
    }
    for (package p : lpack2.pack){
        if (IsElemNotInList(lpack1,p)){
            pb.branch = lpack1.branch;
            pb.pack = p;
            resl.push_back(pb);
        }
    }
    return resl;    
}
void writeResJSON(list<packbranch> ljs){    
   // Достаём значения 
    cout << "Result JSON" << endl;    
  for (packbranch js : ljs){ 
      cout << "{ "<< endl;     
      cout << "branch : " << js.branch << ",\n";
	  cout << "pack :  " << endl;
      cout << "{ \n";
      cout << "name : " << js.pack.name << ",\n"; 
  	  cout << "epoch : " << js.pack.epoch << ",\n";
  	  cout << "version : " << js.pack.version << ",\n";
  	  cout << "release : " << js.pack.release << ",\n";
  	  cout << "arch : " << js.pack.arch << ",\n";
  	  cout << "disttag : " << js.pack.disttag << ",\n";
  	  cout << "buildtime : " << js.pack.buildtime << ",\n";
  	  cout << "source : " << js.pack.source << ",\n";
      cout << "} \n";
      cout << "}"<< endl;
  }
}





