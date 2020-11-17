/*
 * Copyright (c) 2020 Christophe Rubeck.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

void serverSetup() {

  const auto sendHeader = []() {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    Serial.println(server.uri());
  };

  const auto sendConfig = []() {
    char buffer[256];
    getConfig(buffer, sizeof(buffer));
    Serial.print("GET: ");
    Serial.println(buffer);
    server.send(200, "application/json", buffer);
  };
  
  server.on("/", HTTP_GET, [&sendHeader](){
    sendHeader();
    server.send_P(200, "text/html", dist_index_html, dist_index_html_len);
  });
  
  server.on("/index.html", HTTP_GET, [&sendHeader](){
    sendHeader();
    server.send_P(200, "text/html", dist_index_html, dist_index_html_len);
  });

  server.on("/app.js",HTTP_GET, [&sendHeader](){
    sendHeader();
    server.send_P(200, "text/javascript", dist_app_js, dist_app_js_len);
  });

  server.on("/images/icon_hi_res.png",HTTP_GET, [&sendHeader](){
    sendHeader();
    server.send_P(200, "image/png", dist_images_icon_hi_res_png, dist_images_icon_hi_res_png_len);
  });
  
  server.on("/images/icon_lo_res.png",HTTP_GET, [&sendHeader](){
    sendHeader();
    server.send_P(200, "image/png", dist_images_icon_lo_res_png, dist_images_icon_lo_res_png_len);
  });

  server.on("/api", HTTP_GET, sendConfig);
  
  server.on("/api", HTTP_POST, [&sendConfig]() {
    
    String data = server.arg("plain");
    int result = setConfig(data);

    Serial.print("POST: ");
    Serial.println(data);

    if(result == 0) {
      sendConfig();
    }
    else {
      server.send(500, "text/html", "Error");
    }
  });
  
  server.on("/info", HTTP_GET, []() {
    char buffer[300];
    getInfo(buffer, sizeof(buffer));
    server.send(200, "application/json", buffer);
  });

  server.onNotFound([](){
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    Serial.println(message);
    server.send(404, "text/plain", message);
  });
  
  server.begin();
}
