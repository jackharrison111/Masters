//TODO: make variables private and a function to access them

#include "jsoncpp/src/lib_json/json_reader.cpp"
#include "jsoncpp/src/lib_json/json_writer.cpp"
#include "jsoncpp/src/lib_json/json_value.cpp"

class convert{
	public:
		std::map<string, std::map<string, double>> infos;
		std::map<string, double> data;
		std::map<string, string> dataSets;

		convert(){}
		void makeMap();
		~convert(){}
};

void convert::makeMap(){
	//// read the .json files into strings ////
	Json::CharReaderBuilder rbuilder;
	Json::StreamWriterBuilder wbuilder;
	Json::CharReader *reader = rbuilder.newCharReader();


	string dataSetsString;
	std::ifstream dataJson("dataSets.json");
	getline(dataJson, dataSetsString); 
	Json::Value dataSetsParsed;
	string dataSetsErrors;
	
	Bool_t dParseSuccess = reader->parse(dataSetsString.c_str(),dataSetsString.c_str() + dataSetsString.size(), &dataSetsParsed, &dataSetsErrors);


	string infofileString;
	std::ifstream infoJson("infofile.json");
	getline(infoJson, infofileString); 
	Json::Value infofileParsed;
	string infofileErrors;
	
	Bool_t iParseSuccess = reader->parse(infofileString.c_str(),infofileString.c_str() + infofileString.size(), &infofileParsed, &infofileErrors);
	///////////////////////////////////////////
	
	delete reader;


	if(!dParseSuccess){
		std::cout<<"Error in parsing dataSets.json" << std::endl;
	}else{
		for(Json::Value::const_iterator it = dataSetsParsed.begin(); it != dataSetsParsed.end(); it++){
			string process = Json::writeString(wbuilder, it.key());
			process = process.erase(0,1);
			process = process.erase(process.length()-1,1);
			string mcFileName = Json::writeString(wbuilder,*it);
			mcFileName = mcFileName.erase(0,1);
			mcFileName = mcFileName.erase(mcFileName.length()-1,1);
			this->dataSets.insert(pair<string, string>(process,mcFileName));
		}
	}

	if (!iParseSuccess){
		std::cout<<"Error in parsing infofile.json" << std::endl;
	}else{
		for(Json::Value::const_iterator outer = infofileParsed.begin(); outer != infofileParsed.end(); outer++){
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

			//infofile.py contains more than dataSets.py
			//if the which iterator 
			std::map<string, string>::iterator which;
			if(dataSets.count(outerName)>0){
				which = dataSets.find(outerName);
				infos.insert(pair<string, std::map<string, double>>(which->second, this->data));
			}
			this->data.clear();
		}
	}
}

