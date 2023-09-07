#include <iostream>
#include <fstream>

#include <library/library_cli.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>


using namespace std;
using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace web::json;



int main() {
		
    
     string branch1,branch2,arch1,arch2;
    cout << "Enter the first branch ";
    cin >> branch1;
    cout << "Enter the arch of first branch ";
    cin >> arch1;
    cout << "Enter the second branch ";
    cin >> branch2;
    cout << "Enter the arch of second branch ";
    cin >> arch2;
	cout << "List of binary_packages of the first branch ";
    list_json js_str1 = readJSONAPI(REST_API,branch1,arch1,method);
	writeListJSON(js_str1);
    cout << "List of binary_packages of the second branch ";
    list_json js_str2 = readJSONAPI(REST_API,branch2,arch2,method);
	writeListJSON(js_str2);
    list<packbranch> reslist = ResListPack(js_str1,js_str2);
    writeResJSON(reslist);
	    return 0;
}


