/*
 * main.cpp
 *
 *  Created on: 05.09.2023
 *      Author: tata
 */

#include <iostream>
#include <fstream>

#include <library/library_cli.h>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>
//#include <library/library_cli.hpp>


using namespace std;
//using namespace library_cli;
using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace web::json;



int main() {
		
    
    string branch1,branch2,arch;
    cout << "Enter the first branch ";
    cin >> branch1;
    cout << "Enter the second branch ";
    cin >> branch2;
	
    list_json js_str1 = readJSONAPI(REST_API,branch1,arch,method);
	writeListJSON(js_str1);
    list_json js_str2 = readJSONAPI(REST_API,branch2,arch,method);
	writeListJSON(js_str2);
	    //say_hello();
	    return 0;
}


