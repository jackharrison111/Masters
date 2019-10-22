#include "jsoncpp/src/lib_json/json_reader.cpp"
#include "jsoncpp/src/lib_json/json_writer.cpp"
#include "jsoncpp/src/lib_json/json_value.cpp"
#include<map>
#include<string>

Json::StreamWriterBuilder wbuilder;
map<string, map<string, double>> infos;

int converter(){
string jsonString;
std::ifstream test_file("infofile.json");
getline(test_file, jsonString); 


Json::Value help;
Json::CharReaderBuilder rbuilder;
Json::CharReader * reader = rbuilder.newCharReader();
string errors;
bool parseSuccess = reader->parse(jsonString.c_str(),jsonString.c_str() + jsonString.size(), &help, &errors);

delete reader;

if (!parseSuccess){
	std::cout<<"Error in parsing json file" << std::endl;
}

for(Json::Value::const_iterator outer = help.begin(); outer != help.end(); outer++){
	map<string, double> data;
	for(Json::Value::const_iterator inner = (*outer).begin() ; inner != (*outer).end(); inner++){
		string parName = Json::writeString(wbuilder,inner.key());
		parName = parName.erase(0,1);
		parName = parName.erase(parName.length()-1,1);
		double parValue = stod(Json::writeString(wbuilder,*inner));
		data.insert(pair<string, double>(parName, parValue));
	}

	string outerName = Json::writeString(wbuilder, outer.key());
	outerName = outerName.erase(0,1);
	outerName = outerName.erase(outerName.length()-1,1);
	infos.insert(pair<string, map<string, double>>(outerName, data));
}

std::cout<<infos["ttbar_lep"]["sumw"] << std::endl;
return 0;
}
