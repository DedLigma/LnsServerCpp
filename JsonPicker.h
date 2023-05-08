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
  std::cout << "sadfhjklgfdshgjkldsghdsfgjdlksfhgkdjsflghdfljgkdshfkgljdsfghj" << std::endl;
  for(int i = 0; i < splittedLine.size(); i++){
    std::cout << splittedLine[i] << std::endl;
  }
  std::cout << "sadfhjklgfdshgjkldsghdsfgjdlksfhgkdjsflghdfljgkdshfkgljdsfghj" << std::endl;
}

inline Json::Value JsonPicker(std::string &srnsFilePath) {
  std::ifstream jsonState;
  // jsonState.open("../state.json", std::ios::ate);
  jsonState.open("../state.json");
  Json::Reader reader;
  Json::Value jsonData;

  reader.parse(jsonState, jsonData);
  // completeJsonData["state"]["Lct"]["coordinates"]["longtude"] = 43121234;
  // std::cout << completeJsonData["Solv"]["coordinates"]["longtude"] <<
  // std::endl;

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
