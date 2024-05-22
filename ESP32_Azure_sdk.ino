/**
* ©2018 by Daniel Eichhorn - ThingPulse (https://thingpulse.com)
*/
 
#include <WiFi.h>
#include <HTTPClient.h>
 
#define IOT_HUB_NAME "TempHubb"
#define DEVICE_NAME "Esp32"
#define SAS_TOKEN "SharedAccessSignature sr=TempHubb.azure-devices.net%2Fdevices%2FEsp32&sig=oDZIoJdTT8Rw1NcMnqX5FgZRX101hT2xyCq9tHzTFYs%3D&se=1716295254"
 
const char* ssid = "xxx";  // your network SSID (name of wifi network)
const char* password = "xxx";    // your network password
 
const int SensorPin = 36;

  
// Created with openssl s_client -showcerts -connect thingpulse.azure-devices.net:443
// and by picking the root certificate
const char* root_ca =
  "-----BEGIN CERTIFICATE-----\n"
  "MIIEtjCCA56gAwIBAgIQCv1eRG9c89YADp5Gwibf9jANBgkqhkiG9w0BAQsFADBh"
  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3"
  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH"
  "MjAeFw0yMjA0MjgwMDAwMDBaFw0zMjA0MjcyMzU5NTlaMEcxCzAJBgNVBAYTAlVT"
  "MR4wHAYDVQQKExVNaWNyb3NvZnQgQ29ycG9yYXRpb24xGDAWBgNVBAMTD01TRlQg"
  "UlMyNTYgQ0EtMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMiJV34o"
  "eVNHI0mZGh1Rj9mdde3zSY7IhQNqAmRaTzOeRye8QsfhYFXSiMW25JddlcqaqGJ9"
  "GEMcJPWBIBIEdNVYl1bB5KQOl+3m68p59Pu7npC74lJRY8F+p8PLKZAJjSkDD9Ex"
  "mjHBlPcRrasgflPom3D0XB++nB1y+WLn+cB7DWLoj6qZSUDyWwnEDkkjfKee6ybx"
  "SAXq7oORPe9o2BKfgi7dTKlOd7eKhotw96yIgMx7yigE3Q3ARS8m+BOFZ/mx150g"
  "dKFfMcDNvSkCpxjVWnk//icrrmmEsn2xJbEuDCvtoSNvGIuCXxqhTM352HGfO2JK"
  "AF/Kjf5OrPn2QpECAwEAAaOCAYIwggF+MBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYD"
  "VR0OBBYEFAyBfpQ5X8d3on8XFnk46DWWjn+UMB8GA1UdIwQYMBaAFE4iVCAYlebj"
  "buYP+vq5Eu0GF485MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcD"
  "AQYIKwYBBQUHAwIwdgYIKwYBBQUHAQEEajBoMCQGCCsGAQUFBzABhhhodHRwOi8v"
  "b2NzcC5kaWdpY2VydC5jb20wQAYIKwYBBQUHMAKGNGh0dHA6Ly9jYWNlcnRzLmRp"
  "Z2ljZXJ0LmNvbS9EaWdpQ2VydEdsb2JhbFJvb3RHMi5jcnQwQgYDVR0fBDswOTA3"
  "oDWgM4YxaHR0cDovL2NybDMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsUm9v"
  "dEcyLmNybDA9BgNVHSAENjA0MAsGCWCGSAGG/WwCATAHBgVngQwBATAIBgZngQwB"
  "AgEwCAYGZ4EMAQICMAgGBmeBDAECAzANBgkqhkiG9w0BAQsFAAOCAQEAdYWmf+AB"
  "klEQShTbhGPQmH1c9BfnEgUFMJsNpzo9dvRj1Uek+L9WfI3kBQn97oUtf25BQsfc"
  "kIIvTlE3WhA2Cg2yWLTVjH0Ny03dGsqoFYIypnuAwhOWUPHAu++vaUMcPUTUpQCb"
  "eC1h4YW4CCSTYN37D2Q555wxnni0elPj9O0pymWS8gZnsfoKjvoYi/qDPZw1/TSR"
  "penOgI6XjmlmPLBrk4LIw7P7PPg4uXUpCzzeybvARG/NIIkFv1eRYIbDF+bIkZbJ"
  "QFdB9BjjlA4ukAg2YkOyCiB8eXTBi2APaceh3+uBLIgLk8ysy52g2U3gP7Q26Jlg"
  "q/xKzj3O9hFh/g=="
  "-----END CERTIFICATE-----\n";
 
WiFiClientSecure client;
 
void sendRequest(String iothubName, String deviceName, String sasToken, String message) {
  if ((WiFi.status() == WL_CONNECTED)) {  //Check the current connection status
 
    HTTPClient http;
    String url = "https://" + iothubName + ".azure-devices.net/devices/" + deviceName + "/messages/events?api-version=2020-03-13";
    http.begin(url, root_ca);  //Specify the URL and certificate
    http.addHeader("Authorization", sasToken);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Content-Encoding", "utf-8");
    int httpCode = http.POST(message);
 
    if (httpCode > 0) {  //Check for the returning code
 
      String payload = http.getString();
      Serial.print("http Code: ");
      Serial.println(httpCode);
      Serial.println(payload);
    }
 
    else {
      Serial.print("Error on HTTP request. Code: ");
      Serial.println(http.errorToString(httpCode));
    }
 
    http.end();  //Free the resources
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(100);
 
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
}
 
void loop() {

int Sensor = analogRead(SensorPin);
Serial.print("SensorVal: ");
Serial.println(Sensor);
  
  sendRequest(IOT_HUB_NAME, DEVICE_NAME, SAS_TOKEN, "{ \"Light\": " + String(Sensor) + " }");
  delay(10000);
}
