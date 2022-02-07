#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Wifi";
const char* password = "Qwerty345";
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
double data,celsius; 
void setup(void){

  pinMode(A0, INPUT);
  
  delay(100);
  Serial.begin(9600);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleHomepage);
  server.on("/api/temperature",handleGetTemp);  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){
  data = analogRead(A0);
  celsius = (data/1024.0)*3300/10;
  delay(10);
  server.handleClient();
}


void handleHomepage()
{
  page = "<!DOCTYPE html>\n";
  page += "<html lang=\"en\">\n";
  page += "    <head>\n";
  page += "        <meta charset=\"UTF-8\" />\n";
  page += "        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\" />\n";
  page += "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
  page += "        <title>ESP8266 - Temperature Monitor with LM35</title>\n";
  page += "        <style>\n";
  page += "            * {\n";
  page += "                font-family: -apple-system, BlinkMacSystemFont, \"Segoe UI\",\n";
  page += "                    Roboto, Oxygen, Ubuntu, Cantarell, \"Open Sans\",\n";
  page += "                    \"Helvetica Neue\", sans-serif;\n";
  page += "            }\n";
  page += "            body {\n";
  page += "                margin: 0;\n";
  page += "                display: flex;\n";
  page += "                flex-direction: column;\n";
  page += "                justify-content: center;\n";
  page += "                align-items: center;\n";
  page += "            }\n";
  page += "            header {\n";
  page += "                padding: 20px;\n";
  page += "                text-align: center;\n";
  page += "                font-weight: 400;\n";
  page += "                margin-bottom: 30px;\n";
  page += "                font-size: 20px;\n";
  page += "                background-color: rgb(113, 119, 238);\n";
  page += "                color: white;\n";
  page += "                width: 100vw;\n";
  page += "            }\n";
  page += "            .temp-field {\n";
  page += "                font-size: 18px;\n";
  page += "                text-align: center;\n";
  page += "            }\n";
  page += "        </style>\n";
  page += "        <script src=\"https://cdn.jsdelivr.net/npm/chart.js@3.7.0/dist/chart.min.js\"></script>\n";
  page += "    </head>\n";
  page += "    <body>\n";
  page += "        <header>\n";
  page += "            <strong>ESP8266</strong> - Temperature Monitor with LM35\n";
  page += "            <br />\n";
  page += "            <em>Sudhay.S</em>\n";
  page += "        </header>\n";
  page += "        <main class=\"container\">\n";
  page += "            <div class=\"temp-field\">\n";
  page += "                The Current Temperature is\n";
  page += "                <strong><span id=\"tempHolder\">"+String(celsius)+"</span> &deg;C</strong>\n";
  page += "            </div>\n";
  page += "            <br />\n";
  page += "            <canvas id=\"temperature-chart\" width=\"500\" height=\"400\"></canvas>\n";
  page += "        </main>\n";
  page += "        <script>\n";
  page += "            const tempCanvasContext = document\n";
  page += "                .getElementById(\"temperature-chart\")\n";
  page += "                .getContext(\"2d\");\n";
  page += "\n";
  page += "            const temperatureChart = new Chart(tempCanvasContext, {\n";
  page += "                type: \"line\",\n";
  page += "                data: {\n";
  page += "                    labels: [],\n";
  page += "                    datasets: [\n";
  page += "                        {\n";
  page += "                            label: \"Observed Temperature\",\n";
  page += "                            data: [],\n";
  page += "                            backgroundColor: [\"rgba(255, 99, 132, 0.4)\"],\n";
  page += "                            borderColor: [\"red\"],\n";
  page += "                            borderWidth: 2,\n";
  page += "                        },\n";
  page += "                    ],\n";
  page += "                },\n";
  page += "                options: {\n";
  page += "                    plugins: {\n";
  page += "                        title: {\n";
  page += "                            display: true,\n";
  page += "                            text: [\n";
  page += "                                \"Live updating graph\",\n";
  page += "                                \"(Refreshes every 5 seconds)\",\n";
  page += "                            ],\n";
  page += "                            font: {\n";
  page += "                                size: \"17px\",\n";
  page += "                            },\n";
  page += "                        },\n";
  page += "                    },\n";
  page += "                    scales: {\n";
  page += "                        x: {\n";
  page += "                            ticks: {\n";
  page += "                                minRotation: 20,\n";
  page += "                            },\n";
  page += "                        },\n";
  page += "                    },\n";
  page += "                },\n";
  page += "            });\n";
  page += "\n";
  page += "            // Obtain temperature from ESP8266 Web Server's hosted API every 5 seconds\n";
  page += "            setInterval(async () => {\n";
  page += "                tempResponse = await fetch(\n";
  page += "                    \"http://192.168.89.192/api/temperature\"\n";
  page += "                );\n";
  page += "                obtainedTemp = await tempResponse.text();\n";
  page += "                // Update chart\n";
  page += "                temperatureChart.data.datasets[0].data.push(obtainedTemp);\n";
  page += "                temperatureChart.data.labels.push(\n";
  page += "                    new Date().toLocaleTimeString()\n";
  page += "                );\n";
  page += "                tempHolder.innerText = obtainedTemp;\n";
  page += "                temperatureChart.update();\n";
  page += "            }, 5000);\n";
  page += "        </script>\n";
  page += "    </body>\n";
  page += "</html>\n";
  page += "";
  server.send(200, "text/html", page);
}

//To send AJAX response asked every 5 seconds
void handleGetTemp()
{
  server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));  //Setting the header to enable CORS
  server.send(200,"text/plain",String(celsius));
}
