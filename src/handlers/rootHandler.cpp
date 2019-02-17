#include <ESP8266WebServer.h>

#include <handlers/rootHandler.h>
#include <config.h>
#include <settings.h>
#include <utils/time.h>

struct eeprom_data_t eeprom_settings;

RootHandler::RootHandler(system_info_t &info)
{
    _info = info;
}

bool RootHandler::canHandle(HTTPMethod requestMethod, String requestUri)
{
    if (requestMethod != HTTP_GET)
    {
        return false;
    }

    if (requestUri != "/")
    {
        return false;
    }

    return true;
}

bool RootHandler::handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
{
    (void)server;
    if (!canHandle(requestMethod, requestUri))
        return false;

    _fn(server);
    return true;
}

bool RootHandler::canUpload(String requestUri)
{
    return false;
}

void RootHandler::upload(ESP8266WebServer &server, String requestUri, HTTPUpload &upload)
{
    (void)server;
    (void)upload;
}

void RootHandler::_fn(ESP8266WebServer &server)
{
    String s = "<!DOCTYPE HTML>\r\n<html><head><meta charset=\"utf-8\"><title>8266_";

    String STAmacID = "";
    if (_info.macId.length() > 0)
        s += _info.macId;
    else
        s += STAmacID;
    s += "</title></head>";
    s += "Status - <a href='/a'>Actions</a> - <a href='/c'>Config</a>";
    s += "<br>";

    s += "Flash real size: ";
    s += _info.flashRealSize;
    s += "<br>";
    s += "Firmware compiled for flash: ";
    s += _info.flashSize;
    s += "<br>";

    s += "<br>STA connect time: ";
    s += humanTimeMillis(millis() - _info.staConnectTime);
    s += " ago";
    s += "<br>";

    wl_status_t status = WiFi.status();
    Serial.print("WiFi Status: ");
    Serial.println(status);

    if (eeprom_settings.STAenabled == true)
    {
        if (status != WL_CONNECTED)
        {
            s += "<br>wifi client enabled and disconnected";
        }
        if (status == WL_CONNECTED)
        {
            s += "<br>wifi client connected";

            String ipSTAstr = "";

            if (ipSTAstr.length() > 0)
            {
                s += ", ip address: <a href=\"http://";
                s += ipSTAstr;
                s += "\">";
                s += ipSTAstr;
                s += "</a>";

                s += ", RSSI: ";
                s += String(WiFi.RSSI());
                s += " dBm";
            }
        }
    }
    else
    {
        s += "<br>wifi client disabled";
    }

    if (eeprom_settings.senderEnabled == true)
        s += "<br>sender enabled";
    else
        s += "<br>sender disabled";

    s += "<br>";
    s += "<br>uptime: ";

    s += humanTimeMillis(millis());

    s += "<br>";
    s += "<br>EEPROM at boot: ";
    s += _info.isEepromSet ? "ok" : "fail";

    s += "</html>\r\n\r\n";
    server.send(200, "text/html", s);
}
