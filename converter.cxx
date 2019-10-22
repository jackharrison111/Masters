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
	//std::cout<<outer.key()<<std::endl;
	map<string, double> data;
	for(Json::Value::const_iterator inner = (*outer).begin() ; inner != (*outer).end(); inner++){
		//std::cout << "  " << inner.key() << " : " << *inner << std::endl;
		string parName = Json::writeString(wbuilder,inner.key());
		double parValue = stod(Json::writeString(wbuilder,*inner));

		data.insert(pair<string, double>(parName, parValue));
	}

		




	string outerName = Json::writeString(wbuilder, outer.key());
	infos.insert(pair<string, map<string, double>>(outerName, data));
}

/*for(map<string, map<string, double>>::iterator it = infos.begin(); it!= infos.end(); it++){
	std::cout << it->first << std::endl;
	for(map<string, double>::iterator at = (it->second).begin(); at!=(it->second).end(); at++){
		std::cout<<"   "<<at->first<<" : "<<at->second<<std::endl;
	}
}
*/
//std::cout<<(infos.find("ttbar_lep"))["sumw"]<<std::endl;
map<string, map<string,double>>::iterator it = infos.find("ttbar_lep");
//cout << (it->second).find("sumw") << endl;

//std::cout << ((infos["ttbar_lep"])["sumw"]) << std::endl;
/*for(map<string, double>::iterator at = (infos["ttbar_lep"]).begin(); at!=(infos["ttbar_lep"]).end(); at++){
		std::cout<<"   "<<at->first<<" : "<<at->second<<std::endl;
	}*/
return 0;
}

