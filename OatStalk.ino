
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <ESP8266WiFi.h>
#include <Phant.h>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW

const char WiFiSSID[] = "galileo";
const char WiFiPSK[] = "yesyesyes";

////////////////
// Phant Keys //
////////////////
const char PhantHost[] = "data.sparkfun.com";
const char PublicKey[] = "jwDXnoXRGXU2ZY0YNw4j";
const char PrivateKey[] = "zpAMRKMXqMh8Bxpxv05R";

void setup() 
{
  Serial.begin(115200);
  connectWiFi();
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  imu.begin();
}

void loop()
{
  // Update the sensor values whenever new data is available
  if ( imu.gyroAvailable() )
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    imu.readMag();
  } 
  Serial.print(millis());
  Serial.print(",");
  Serial.print(imu.calcGyro(imu.gx), 2);
  Serial.print(",");
  Serial.print(imu.calcGyro(imu.gy), 2);
  Serial.print(",");
  Serial.print(imu.calcGyro(imu.gz), 2);
  Serial.print(",");
  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(",");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(",");
  Serial.print(imu.calcAccel(imu.az), 2);
  Serial.print(",");
  Serial.print(imu.calcMag(imu.mx), 2);
  Serial.print(",");
  Serial.print(imu.calcMag(imu.my), 2);
  Serial.print(",");
  Serial.print(imu.calcMag(imu.mz), 2);
  Serial.println();
  postToPhant();
  delay(10000);
}

void connectWiFi()
{
  byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
  Serial.println(WiFi.status());
}

int postToPhant()
{
  // Declare an object from the Phant library - phant
  Phant phant(PhantHost, PublicKey, PrivateKey);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String postedID = "Thing-" + macID;

  // Add the four field/value pairs defined by our stream:
 // phant.add("id", postedID);
  phant.add("gx", imu.calcGyro(imu.gx));
  phant.add("gy", imu.calcGyro(imu.gy));
  phant.add("gz", imu.calcGyro(imu.gz));
  phant.add("ax", imu.calcAccel(imu.ax));
  phant.add("ay", imu.calcAccel(imu.ay));
  phant.add("az", imu.calcAccel(imu.az));
  phant.add("mx", imu.calcMag(imu.mx));
  phant.add("my", imu.calcMag(imu.my));
  phant.add("mz", imu.calcMag(imu.mz));
  phant.add("time", millis());

  // Now connect to data.sparkfun.com, and post our data:
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(PhantHost, httpPort)) 
  {
    // If we fail to connect, return 0.
    return 0;
  }
  // If we successfully connected, print our Phant post:
  client.print(phant.post());

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line); // Trying to avoid using serial
  }
  return 1; // Return success
}
