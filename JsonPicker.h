#include <json/json.h>
#include <json/value.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

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

inline void SolvHandler(std::string &line, Json::Value &jsonData) {
  std::vector<std::string> splittedLine;
  splittedLine.clear();
  StringSplitter(line, splittedLine);
  float typeChanger;
  jsonData["Solv"]["start_timer"] = splittedLine[2];
  jsonData["Solv"]["time_data"] = splittedLine[5];
  jsonData["Solv"]["timer"] = splittedLine[6];
  splittedLine[7].erase(splittedLine[7].end() - 1, splittedLine[7].end());
  typeChanger = atof(splittedLine[7].c_str());
  jsonData["Solv"]["coordinates"]["latitude"] = typeChanger;
  typeChanger = atof(splittedLine[8].c_str());
  jsonData["Solv"]["coordinates"]["longtude"] = typeChanger;
  splittedLine[10].erase(0, 2);
  jsonData["Solv"]["height"] = splittedLine[10];
  jsonData["Solv"]["svs"] = splittedLine[12];
  splittedLine[14].erase(0, 4);
  jsonData["Solv"]["res"] = splittedLine[14];
  jsonData["Solv"]["modi_ms"] = splittedLine[16];
  jsonData["Solv"]["modi_ns"] = splittedLine[18];
  splittedLine[20].erase((splittedLine[20].end() - 1), splittedLine[20].end());
  splittedLine[20].erase(0, 5);
  jsonData["Solv"]["pdop"] = splittedLine[20];
  splittedLine[21].erase(0, 2);
  jsonData["Solv"]["e_distance"] = splittedLine[21];
  jsonData["Solv"]["appar_counter"] = splittedLine[24];
}

inline void LctHandler(std::vector<std::string> &lctLines,
                       std::ifstream &srnsFile, Json::Value &jsonData) {
  std::vector<int> residsIntNumbers;
  Json::Value numberArr;
  Json::Value latitudeArr;
  Json::Value longtudeArr;
  Json::Value heightArr;
  Json::Value residsArr;
  Json::Value timeDataArr;
  Json::Value timerArr;
  int startI = 0;
  if (lctLines.size() > 500) startI = 500;
  for (int i = startI; i < lctLines.size(); i++) {
    if (lctLines[i].find("NavMsgLctLxOCoord") != std::string::npos ||
        lctLines[i].find("NavMsgLctLxOResids") != std::string::npos ||
        lctLines[i].find("NavMsgLctLxONewFrame:") != std::string::npos) {
      std::vector<std::string> lctSplittedLine;
      lctSplittedLine.clear();
      StringSplitter(lctLines[i], lctSplittedLine);

      if (!std::count(residsIntNumbers.begin(), residsIntNumbers.end(),
                      atoi(lctSplittedLine[11].c_str())) &&
          strcmp(lctSplittedLine[11].c_str(), "") != 0) {
        residsIntNumbers.push_back(atoi(lctSplittedLine[11].c_str()));
      }
      sort(residsIntNumbers.begin(), residsIntNumbers.end());

      if (lctLines[i].find("NavMsgLctLxOCoord") != std::string::npos) {
        Json::Value::ArrayIndex spaceFix =
            strcmp(lctSplittedLine[11].c_str(), "") != 0 ? 0 : 1;
        Json::Value::ArrayIndex indexLct =
            atoi(lctSplittedLine[11 - spaceFix].c_str());
        float typeChanger;
        lctSplittedLine[14 - spaceFix].erase(
            lctSplittedLine[14 - spaceFix].end() - 1,
            lctSplittedLine[14 - spaceFix].end());

        typeChanger = atof(lctSplittedLine[14 - spaceFix].c_str());
        latitudeArr[indexLct] = typeChanger;

        lctSplittedLine[15 - spaceFix].erase(
            lctSplittedLine[15 - spaceFix].end() - 1,
            lctSplittedLine[15 - spaceFix].end());

        typeChanger = atof(lctSplittedLine[15 - spaceFix].c_str());
        longtudeArr[indexLct] = typeChanger;

        heightArr[indexLct] = lctSplittedLine[16 - spaceFix];
      }

      if (lctLines[i].find("NavMsgLctLxONewFrame:") != std::string::npos) {
        Json::Value::ArrayIndex spaceFix =
            strcmp(lctSplittedLine[11].c_str(), "") != 0 ? 0 : 1;
        Json::Value::ArrayIndex indexLct =
            atoi(lctSplittedLine[11 - spaceFix].c_str());
        timeDataArr[indexLct] = lctSplittedLine[17 - spaceFix];
        timerArr[indexLct] = lctSplittedLine[18 - spaceFix];
      }

      if (lctLines[i].find("NavMsgLctLxOResids") != std::string::npos) {
        lctSplittedLine.erase(lctSplittedLine.begin() + 19);
        for (int j = 14; j < lctSplittedLine.size(); j++) {
          Json::Value::ArrayIndex firstIndex =
              atoi(lctSplittedLine[11].c_str());
          Json::Value::ArrayIndex secondIndex = j - 14;
          residsArr[firstIndex][secondIndex] = atoi(lctSplittedLine[j].c_str());
          residsArr[firstIndex][0] = Json::nullValue;
        }
      }

      for (int i = 0; i < residsIntNumbers.size(); i++) {
        Json::Value::ArrayIndex jsi = i;
        numberArr[jsi] = residsIntNumbers[i];
      }

      jsonData["Lct"]["number"] = numberArr;
      jsonData["Lct"]["coordinates"]["latitude"] = latitudeArr;
      jsonData["Lct"]["coordinates"]["longtude"] = longtudeArr;
      jsonData["Lct"]["height"] = heightArr;
      jsonData["Lct"]["time_data"] = timeDataArr;
      jsonData["Lct"]["timer"] = timerArr;
      jsonData["Lct"]["resids"] = residsArr;
    }
  }
}

inline Json::Value JsonPicker(std::string &srnsFilePath) {
  Json::Value jsonData;

  std::ifstream srnsFile;
  srnsFile.open(srnsFilePath);

  std::vector<std::string> lctLines;
  lctLines.clear();

  for (std::string line; std::getline(srnsFile, line);) {
    if (line.find("Solv[") != std::string::npos) {
      SolvHandler(line, jsonData);
    }
    if (line.find("Lct") != std::string::npos) {
      lctLines.push_back(line);
    }
  }
  LctHandler(lctLines, srnsFile, jsonData);

  return jsonData;
}
