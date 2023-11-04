#include <Arduino_HTS221.h>
#include <WiFiNINA.h> // WiFiNINA library for Nano 33 BLE Sense

// Replace with your Wi-Fi credentials
char ssid[] = "yourSSID";
char pass[] = "yourPASSWORD";

char server[] = "your-server.com"; // Replace with your server address
char authToken[] = "your-auth-token"; // Replace with your authorization token

void setup() {
  Serial.begin(9600);
  if (!HTS.begin()) {
    Serial.println("Could not find a valid HTS sensor, check wiring!");
    while (1);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float temperature = HTS.readTemperature();
  float humidity = HTS.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Create the JSON payload
  String data = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

  // Send data to the server with authorization token
  sendPostRequest(data);

  delay(60000); // Send data every 60 seconds
}

void sendPostRequest(String data) {
  WiFiClient client;

  if (client.connect(server, 80)) {
    // HTTP POST request with authorization token
    client.println("POST /your-endpoint HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/json");
    client.println("Content-Length: " + String(data.length()));
    client.println("Authorization: Bearer " + String(authToken)); // Include authorization token
    client.println();
    client.print(data);

    delay(1000); // Allow time for server response

    // Read and print server response
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    Serial.println();

    client.stop();
  } else {
    Serial.println("Connection to server failed");
  }
}
