#include <ESP8266WebServer.h>

#include <handlers/configHandler.h>
#include <config.h>
#include <settings.h>

ConfigHandler::ConfigHandler(system_info_t &info, eeprom_data_t& eeprom_data)
{
    _info = info;
    _eeprom_data = eeprom_data;
}

bool ConfigHandler::canHandle(HTTPMethod requestMethod, String requestUri)
{
    Serial.println(requestMethod);
    Serial.println(requestUri);
    if (requestMethod != HTTP_GET && requestMethod != HTTP_POST )
    {
        return false;
    }

    if (requestUri != "/c")
    {
        return false;
    }

    return true;
}

bool ConfigHandler::handle(ESP8266WebServer &server, HTTPMethod requestMethod, String requestUri)
{
    (void)server;
    if (!canHandle(requestMethod, requestUri))
        return false;

    if (requestMethod == HTTP_GET) {
        displayForm(server);
    }

    if (requestMethod == HTTP_POST) {
        processForm(server);
    }
    
    return true;
}

bool ConfigHandler::canUpload(String requestUri)
{
    return false;
}

void ConfigHandler::upload(ESP8266WebServer &server, String requestUri, HTTPUpload &upload)
{
    (void)server;
    (void)upload;
}

void ConfigHandler::displayForm(ESP8266WebServer &server)
{
  String s = "<!DOCTYPE HTML>\r\n<html><head><meta charset=\"utf-8\"><title>" + String(NAME_TAG) + "8266_";

  String STAmacID = "";
    if (_info.macId.length() > 0)
        s += _info.macId;
    else
        s += STAmacID;
  s += "</title></head>";

  s += "<a href='/'>Status</a> - <a href='/a'>Actions</a> - Config";


  s += "<br><form method=\"post\">";

  s += "<br>senderEnabled: <select name=\"senderEnabled\">";
  if (_eeprom_data.senderEnabled == true)
  {
    s += "<option value =\"true\" selected=\"selected\">Enabled</option>";
    s += "<option value =\"false\">Disabled</option>";
  }
  else
  {
    s += "<option value =\"true\">Enabled</option>";
    s += "<option value =\"false\" selected=\"selected\">Disabled</option>";
  }
  s += "</select>";

  s += "<br>STAenabled: <select name=\"STAenabled\">";
  if (_eeprom_data.STAenabled == true)
  {
    s += "<option value =\"true\" selected=\"selected\">Enabled</option>";
    s += "<option value =\"false\">Disabled</option>";
  }
  else
  {
    s += "<option value =\"true\">Enabled</option>";
    s += "<option value =\"false\" selected=\"selected\">Disabled</option>";
  }
  s += "</select>";


  s += "<br>disable AP when STA connected: <select name=\"disableAP\">";
  if (_eeprom_data.disableAP == true)
  {
    s += "<option value =\"true\" selected=\"selected\">Enabled</option>";
    s += "<option value =\"false\">Disabled</option>";
  }
  else
  {
    s += "<option value =\"true\">Enabled</option>";
    s += "<option value =\"false\" selected=\"selected\">Disabled</option>";
  }
  s += "</select>";

  s += "<br>";


  s += "<br>ssid: <input type=text name=ssid size=30 maxlength=16 value='";
  s += _eeprom_data.STAssid;

  s += "' />";
  //if (clientConfigAllowed == true)
  //{
  s += "<br>pass: <input type=text name=pass size=30 maxlength=16 value='";
  s += _eeprom_data.STApass;
  s += "' /><br>";
  /*}
    else
    {
    s += "<br>pass: <input disabled=\"disabled\" type=text name=pass size=30 maxlength=16 value='******' /><br>";
    }*/

  s += "<br>http auth for STA clients: <select name=\"CONFIGauthEnabled\">";
  if (_eeprom_data.CONFIGauthEnabled == true)
  {
    s += "<option value =\"true\" selected=\"selected\">Enabled</option>";
    s += "<option value =\"false\">Disabled</option>";
  }
  else
  {
    s += "<option value =\"true\">Enabled</option>";
    s += "<option value =\"false\" selected=\"selected\">Disabled</option>";
  }
  s += "</select>";

  s += "<br>http user: <input type=text name=CONFIGuser size=30 maxlength=8 value='";
  s += _eeprom_data.CONFIGuser;

  s += "' />";
  //if (clientConfigAllowed == true)
  //{
  s += "<br>http pass: <input type=text name=CONFIGpass size=30 maxlength=8 value='";
  s += _eeprom_data.CONFIGpass;
  s += "' /><br>";
  /*}
    else
    {
    s += "<br>http pass: <input disabled=\"disabled\" type=text name=HTTPpass size=30 maxlength=8 value='******' /><br>";
    }*/

  s += "<br>Reboot after storing <input type=\"checkbox\" name=\"rebootRq\"/>";
  s += "<br>";

  //if (clientConfigAllowed == true)
  s += "<input type='submit' value='Save'></form>";
  //else
  //  s += "<input type='submit' value='Save' disabled=\"disabled\"> changes disabled from external wifi</form>";



  s += "</html>\r\n\r\n";
  server.send(200, "text/html", s);
}

void ConfigHandler::processForm(ESP8266WebServer &server)
{}
