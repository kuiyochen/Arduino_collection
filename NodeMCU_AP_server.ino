// 參考資料： http://nhs-tw.blogspot.com/2015/11/step-by-step-esp8266-12-with-arduino_27.html
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// 打開手機 wifi 搜尋名稱為 MyHome 的網路，密碼為 12345678。
const char* ssid     = "MyHome";
const char* password = "12345678";

ESP8266WebServer server(80);
String D4State = "off";

void setup() {
    Serial.begin(115200);

    pinMode(D4, OUTPUT);
    digitalWrite(D4, LOW);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    Serial.println("");
    Serial.println("IP address: ");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP); // 沒意外應該是 192.168.4.1 ，打開瀏覽器輸入後送出就會到網頁裡

    createWebServer();
    server.begin();
}
void loop(){
    server.handleClient();
}

void createWebServer(){
    server.on("/", []() {
        IPAddress ip = WiFi.softAPIP();
        String content = "<html>\n<head><meta charset=\"utf-8\" />\n    <style>\n    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n    .buttonon {\n       background: #3D94F6;\n       background-image: -webkit-linear-gradient(top, #3D94F6, #1E62D0);\n       background-image: -moz-linear-gradient(top, #3D94F6, #1E62D0);\n       background-image: -ms-linear-gradient(top, #3D94F6, #1E62D0);\n       background-image: -o-linear-gradient(top, #3D94F6, #1E62D0);\n       background-image: -webkit-gradient(to bottom, #3D94F6, #1E62D0);\n       -webkit-border-radius: 20px;\n       -moz-border-radius: 20px;\n       border-radius: 20px;\n       color: #FFFFFF;\n       font-family: Brush Script MT;\n       font-size: 40px;\n       font-weight: 100;\n       padding: 40px;\n       -webkit-box-shadow: 1px 1px 20px 0 #000000;\n       -moz-box-shadow: 1px 1px 20px 0 #000000;\n       box-shadow: 1px 1px 20px 0 #000000;\n       text-shadow: 1px 1px 20px #000000;\n       border: solid #337FED 1px;\n       text-decoration: none;\n       display: inline-block;\n       cursor: pointer;\n       text-align: center;\n    }\n\n    .buttonon:hover {\n       border: solid #337FED 1px;\n       background: #1E62D0;\n       background-image: -webkit-linear-gradient(top, #1E62D0, #3D94F6);\n       background-image: -moz-linear-gradient(top, #1E62D0, #3D94F6);\n       background-image: -ms-linear-gradient(top, #1E62D0, #3D94F6);\n       background-image: -o-linear-gradient(top, #1E62D0, #3D94F6);\n       background-image: -webkit-gradient(to bottom, #1E62D0, #3D94F6);\n       -webkit-border-radius: 20px;\n       -moz-border-radius: 20px;\n       border-radius: 20px;\n       text-decoration: none;\n    }\n    .buttonoff {\n       background: #D43DF6;\n       background-image: -webkit-linear-gradient(top, #D43DF6, #D01E5F);\n       background-image: -moz-linear-gradient(top, #D43DF6, #D01E5F);\n       background-image: -ms-linear-gradient(top, #D43DF6, #D01E5F);\n       background-image: -o-linear-gradient(top, #D43DF6, #D01E5F);\n       background-image: -webkit-gradient(to bottom, #D43DF6, #D01E5F);\n       -webkit-border-radius: 20px;\n       -moz-border-radius: 20px;\n       border-radius: 20px;\n       color: #FFFFFF;\n       font-family: Brush Script MT;\n       font-size: 40px;\n       font-weight: 100;\n       padding: 40px;\n       -webkit-box-shadow: 1px 1px 20px 0 #000000;\n       -moz-box-shadow: 1px 1px 20px 0 #000000;\n       box-shadow: 1px 1px 20px 0 #000000;\n       text-shadow: 1px 1px 20px #000000;\n       border: solid #FF0000 1px;\n       text-decoration: none;\n       display: inline-block;\n       cursor: pointer;\n       text-align: center;\n    }\n\n    .buttonoff:hover {\n       border: solid #ED3352 1px;\n       background: #EF9CF4;\n       background-image: -webkit-linear-gradient(top, #EF9CF4, #EB76FD);\n       background-image: -moz-linear-gradient(top, #EF9CF4, #EB76FD);\n       background-image: -ms-linear-gradient(top, #EF9CF4, #EB76FD);\n       background-image: -o-linear-gradient(top, #EF9CF4, #EB76FD);\n       background-image: -webkit-gradient(to bottom, #EF9CF4, #EB76FD);\n       -webkit-border-radius: 20px;\n       -moz-border-radius: 20px;\n       border-radius: 20px;\n       text-decoration: none;\n    }\n    </style>\n    <script type=\"text/javascript\">\n        function Submiton( ) {\n            var xhttp = new XMLHttpRequest();\n            xhttp.onreadystatechange = function() {\n                if (this.readyState == 4 && this.status == 200) {\n                    if (this.responseText == \"OK\"){\n                        document.getElementById(\"Submit\").outerHTML = '<a href=\"#\" class=\"buttonoff\" id=\"Submit\" onclick=\"Submitoff()\">OFF</a>';}\n                    }\n            };\n            xhttp.open(\"GET\", \"/on\", true);\n            xhttp.send();\n        }\n        function Submitoff( ) {\n            var xhttp = new XMLHttpRequest();\n            xhttp.onreadystatechange = function() {\n                if (this.readyState == 4 && this.status == 200) {\n                    if (this.responseText == \"OK\"){\n                        document.getElementById(\"Submit\").outerHTML = '<a href=\"#\" class=\"buttonon\" id=\"Submit\" onclick=\"Submiton()\">ON</a>';\n                    }\n                }\n            };\n            xhttp.open(\"GET\", \"/off\", true);\n            xhttp.send();\n        }\n        setInterval(function ( ) {\n            var xhttp = new XMLHttpRequest();\n            xhttp.onreadystatechange = function() {\n                if (this.readyState == 4 && this.status == 200) {\n                        document.getElementById(\"count\").innerHTML = this.responseText;\n                }\n            };\n            // var params = JSON.stringify({\"data\": String(Number(document.getElementById(\"count\").innerHTML) + 1)});\n            xhttp.open(\"GET\", \"/count?data=\" + String(Number(document.getElementById(\"count\").innerHTML) + 1), true);\n            // xhttp.setRequestHeader(\"Content-type\", \"application/json; charset=utf-8\");\n            // xhttp.setRequestHeader(\"Content-length\", params.length());\n            xhttp.send();\n        }, 200);\n    </script>\n</head>\n\n<body><h1>FabLab 遙控船控制系統</h1><p>請按下測試按鈕</p>\n\n<a href=\"#\" class=\"buttonon\" id=\"Submit\" onclick=\"Submiton()\">ON</a>\n<div><pre id=\"count\">0</pre></div>\n\n\n";
        digitalWrite(D4, HIGH);
        server.send(200, "text/html", content);
    });

    server.on("/on", [](){
        digitalWrite(D4, HIGH);
        server.send(200, "text/plain", "OK");
    });
    server.on("/off", [](){
        digitalWrite(D4, LOW);
        server.send(200, "text/plain", "OK");
    });
    server.on("/count", [](){
        Serial.print(".");
        String str = server.arg("data");
        Serial.println(str);
        server.send(200, "text/plain", str);
    });
}

