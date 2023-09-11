#include <iostream>
#include "string.h"
#include <list>
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
using namespace web::http;
using namespace web::http::client;
using namespace web;
using namespace pplx;
using namespace std;
const string method = "export/branch_binary_packages";
const string REST_API = "https://rdb.altlinux.org";
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
// В этой функции считываем данныет из REST_API в виде JSON объекта
list_json readJSONAPI(const string url,string branch, string arch, const string method){
        // Создаём клиент и привязываем к домену. Туда пойдут наши запросы
      string full_url = url+"/api/"+method+"/"+branch;
      string full_url_s;
      if (arch != NULL || arch != ""){
        full_url_s = full_url + "?arch="+arch;
      }
      else {
        full_url_s = full_url;
      }
    uri url(full_url_s);
    http_client client(url);
    http_request req;
    req.set_method(methods::GET);
    task<json::value> request = client.request(req).then([](http_response response)
    {
        json::value jsonObject;
        try
        {
            if ( response.status_code() == status_codes::OK )
            {
		//Установим заголовки для получения типа контента в формате JSON.
                response.headers().set_content_type(L"application/json"); 
                // получаем json_объекты
		jsonObject = response.extract_json().get();
            }
        }
        catch (const http_exception& e)
        {
            cout << e.error_code().message() << "\n";
        }        
        return jsonObject; // возвращает значения json
    });
    json::array packages = request.get().at(L"packages").as_array(); // Мы получаем возвращенный ответ здесь
    list_json js;
    js.branch = branch;
    list<package> packs_list(std::begin(packages), std::end(packages));
    js.pack = packs_list;
    return js;    	
}
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
bool IsVersionReleaseMore(package pack1, package pack2){
	bool cond1 = pack1.source == pack2.source;
	bool cond2 = (pack1.name == pack2.name) || (pack1.name.find(pack2.name) != std::string::npos) || (pack2.name.find(pack1.name) != std::string::npos);
	bool cond3 = (pack1.epoch != NULL) && (pack2.epoch != NULL);
	bool cond4 = (pack1.epoch !=0) && (pack1.epoch > pack2.epoch); 
	bool cond5 = (pack1.version > pack2.version) && (pack1.release > pack2.release);
	bool cond_general = cond1 && cond2 && cond3 && cond4 && cond5;
	return cond_general;
}
bool IsElemNotInList(list<package> str_list, package pack){
    return find (str_list.begin(), str_list.end(), pack) == str_list.end();
}
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
