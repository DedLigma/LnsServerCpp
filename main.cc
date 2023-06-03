#include <drogon/drogon.h>

#include <cstdlib>
#include <string>

#include "FileWatcher.h"
#include "JsonPicker.h"

typedef std::function<void(const drogon::HttpResponsePtr &)> Callback;

class Hand {
 public:
  int changer = 1;
  std::string filePath = "/tmp/srns_0x0003.txt";
  std::time_t lastWriteTime = 0;

  void indexHandler(const drogon::HttpRequestPtr &request,
                    Callback &&callback) {
    Json::Value jsonBody;

    jsonBody["state"] = JsonPicker(this->filePath);

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
  Hand handObj;
  handObj.DragonApp();

  return EXIT_SUCCESS;
}
