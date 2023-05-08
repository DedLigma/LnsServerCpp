// #include "Headers.h"
#include "FileWatcher.h"
#include "JsonPicker.h"
#include <cstdlib>
#include <drogon/drogon.h>
#include <string>

typedef std::function<void(const drogon::HttpResponsePtr &)> Callback;

class Hand {
public:
  int changer = 1;
  std::string filePath = "../srns_0x0003.txt";
  std::time_t lastWriteTime;

  Hand(std::string filePath) {

    this->lastWriteTime = boost::filesystem::last_write_time(filePath);
  }

  Hand() {}

  void indexHandler(const drogon::HttpRequestPtr &request,
                    Callback &&callback) {
    Json::Value jsonBody;

    // jsonBody["message"] = std::time(nullptr);
    jsonBody["state"] = JsonPicker(filePath);

    auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
    callback(response);
  }

  void changeHandler(const drogon::HttpRequestPtr &request,
                     Callback &&callback) {

    this->changer =
        watchFile(this->filePath, this->changer, this->lastWriteTime);
    auto response = drogon::HttpResponse::newHttpJsonResponse(this->changer);
    callback(response);
  }

  void DragonApp() {

    drogon::app()
        .registerHandler("/state", &Hand::indexHandler, {drogon::Get})
        .registerHandler("/change", &Hand::changeHandler, {drogon::Get})
        .addListener("127.0.0.1", 3001)
        .setThreadNum(8)
        .registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req,
                                       const drogon::HttpResponsePtr &resp) {
          resp->addHeader("Access-Control-Allow-Origin", "*");
        })
        .run();
  }
};

int main() {
  std::string filePath = "../srns_0x0003.txt";
  std::cout << "Enter path of file srns_0x0003.txt" << std::endl;
  std::cin >> filePath;

  if (filePath.empty() || filePath == "std") {
    std::cout << "Standart path choose: "
                 "../srns_0x0003.txt"
              << std::endl;
    filePath = "../srns_0x0003.txt";
  } else {
    std::cout << "The path will be: " + filePath << std::endl;
  }
  Hand handObj{filePath};
  handObj.DragonApp();

  return EXIT_SUCCESS;
}
