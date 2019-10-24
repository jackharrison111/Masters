#include "converter.h"

void convert::makeMap(){
	string jsonString;
	std::ifstream test_file("infofile.json");
	getline(test_file, jsonString); 
	
	Json::Value help;
	Json::CharReaderBuilder rbuilder;
	Json::StreamWriterBuilder wbuilder;
	Json::CharReader *reader = rbuilder.newCharReader();
	string errors;
	Bool_t parseSuccess = reader->parse(jsonString.c_str(),jsonString.c_str() + jsonString.size(), &help, &errors);

	delete reader;

	if (!parseSuccess){
		std::cout<<"Error in parsing json file" << std::endl;
	}

	for(Json::Value::const_iterator outer = help.begin(); outer != help.end(); outer++){
		for(Json::Value::const_iterator inner = (*outer).begin() ; inner != (*outer).end(); inner++){
			string parName = Json::writeString(wbuilder,inner.key());
			parName = parName.erase(0,1);
			parName = parName.erase(parName.length()-1,1);
			double parValue = stod(Json::writeString(wbuilder,*inner));
			this->data.insert(pair<string, double>(parName, parValue));
		}

		string outerName = Json::writeString(wbuilder, outer.key());
		outerName = outerName.erase(0,1);
		outerName = outerName.erase(outerName.length()-1,1);
		infos.insert(pair<string, map<string, double>>(outerName, this->data));
		this->data.clear();
	}

	std::cout<<this->infos["ttbar_lep"]["sumw"] << std::endl;
}
