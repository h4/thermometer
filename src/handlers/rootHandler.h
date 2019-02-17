#include <ESP8266WebServer.h>
#include <config.h>

class RootHandler : public RequestHandler
{
public:
  RootHandler(system_info_t& info);

  bool canHandle(HTTPMethod requestMethod, String requestUri) override;
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) override;
  bool canUpload(String requestUri) override;
  void upload(ESP8266WebServer& server, String requestUri, HTTPUpload& upload) override;

private:
  system_info_t _info;
  void _fn(ESP8266WebServer& server);
};
