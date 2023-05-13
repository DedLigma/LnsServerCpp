#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <json/json.h>
#include <json/value.h>
#include <string>

inline void StringSplitter(std::string &line,
                           std::vector<std::string> &result) {
  std::string word;
  word.clear();
  line = line + ' ';
  for (std::string::iterator iter = line.begin(); iter != line.end(); ++iter) {
    if (*iter != ' ') {
      word += *iter;
    } else {
      result.push_back(word);
      word.clear();
    }
  }
}



inline void SolvHandler(std::string &line, std::ifstream &srnsFile,
                        Json::Value &jsonData) {
  std::vector<std::string> splittedLine;
  splittedLine.clear();
  StringSplitter(line, splittedLine);
  jsonData["Solv"]["start_timer"] = splittedLine[2];
  jsonData["Solv"]["time_data"] = splittedLine[5];
  jsonData["Solv"]["timer"] = splittedLine[6];
  splittedLine[7].erase(splittedLine[7].end() - 1, splittedLine[7].end());
  jsonData["Solv"]["coordinates"]["latitude"] = splittedLine[7];
  jsonData["Solv"]["coordinates"]["longtude"] = splittedLine[8];
  splittedLine[10].erase(0,2);
  jsonData["Solv"]["height"] = splittedLine[10];
  jsonData["Solv"]["svs"] = splittedLine[12];
  splittedLine[14].erase(0,4);
  jsonData["Solv"]["res"] = splittedLine[14];
  jsonData["Solv"]["modi_ms"] = splittedLine[16];
  jsonData["Solv"]["modi_ns"] = splittedLine[18];
  splittedLine[20].erase((splittedLine[20].end() - 1), splittedLine[20].end());
  splittedLine[20].erase(0,5);
  jsonData["Solv"]["pdop"] = splittedLine[20];
  splittedLine[21].erase(0,2);
  jsonData["Solv"]["e_distance"] = splittedLine[21];
  jsonData["Solv"]["appar_counter"] = splittedLine[24];

}

inline Json::Value JsonPicker(std::string &srnsFilePath) {
  std::ifstream jsonState;
  jsonState.open("../state.json");
  Json::Reader reader;
  Json::Value jsonData;

  reader.parse(jsonState, jsonData);

  std::ifstream srnsFile;
  srnsFile.open(srnsFilePath);

  for (std::string line; std::getline(srnsFile, line);) {
    if (line.find("Solv[3LCT]") != std::string::npos) {
      // std::cout << line << std::endl;
      SolvHandler(line, srnsFile, jsonData);
    }
  }

  return jsonData;
}
