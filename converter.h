#include "jsoncpp/src/lib_json/json_reader.cpp"
#include "jsoncpp/src/lib_json/json_writer.cpp"
#include "jsoncpp/src/lib_json/json_value.cpp"

class convert{
	public:
		map<string, map<string, double>> infos;
		map<string, double> data;

		convert(){};
		void makeMap();
		~convert(){};
};
