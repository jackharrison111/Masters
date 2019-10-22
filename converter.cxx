#include "jsoncpp/src/lib_json/json_reader.cpp"
//#include <fstream>
#include "jsoncpp/src/lib_json/json_writer.cpp"
#include "jsoncpp/src/lib_json/json_value.cpp"
//#include "jsoncpp/include/json/reader.h"
#include <string>
int converter(){



string jsonString;
std::ifstream test_file("test.json");
getline(test_file, jsonString); 


Json::Value help;
Json::CharReaderBuilder builder;
Json::CharReader * reader = builder.newCharReader();
string errors;
bool parseSuccess = reader->parse(jsonString.c_str(),jsonString.c_str() + jsonString.size(), &help, &errors);

delete reader;

if (!parseSuccess){
	std::cout<<"FuCk SAkEE ALi" << std::endl;
}
std::cout<<help["Graviton1500"]["xsec"];



return 0;
}

