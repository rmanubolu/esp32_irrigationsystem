#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#define SECRET_WIFI_SSID "XXXXX"  // replace this
#define SECRET_WIFI_PASS "xxxxxx" // replace this

#define BOARD_ID "YYYYYY" // replace this
#define SECRET_DEVICE_KEY "yyyyyyyyy" // replace this

// Motor control pins
#define PUMP_FWD 32
#define PUMP_REV 33


// Callback function when IoT cloud dashboard variable changed
void onirrigationPumpChange();

// Variable to control the pump from Cloud
bool irrigationPump;

// Initial Arduino Cloud interface properties
void initProperties() 
{
  ArduinoCloud.addProperty(irrigationPump, Permission::Write).onUpdate(onirrigationPumpChange);
  ArduinoCloud.setBoardId(BOARD_ID);
  ArduinoCloud.setSecretDeviceKey(SECRET_DEVICE_KEY);
}

// Set preferred connection to WiFi
WiFiConnectionHandler ArduinoIoTPreferredConnection(SECRET_WIFI_SSID, SECRET_WIFI_PASS);


void setup() {
  /* Initialize serial and wait up to 5 seconds for port to open */
  Serial.begin(9600);
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime <= 5000); ) { }

  pinMode(PUMP_FWD, OUTPUT);
  pinMode(PUMP_REV, OUTPUT);

  digitalWrite(PUMP_FWD, LOW);
  digitalWrite(PUMP_REV, LOW);

  /* Set the debug message level:
   * - DBG_ERROR: Only show error messages
   * - DBG_WARNING: Show warning and error messages
   * - DBG_INFO: Show info, warning, and error messages
   * - DBG_DEBUG: Show debug, info, warning, and error messages
   * - DBG_VERBOSE: Show all messages
   */
  setDebugMessageLevel(DBG_INFO);

  /* This function takes care of connecting your sketch variables to the ArduinoIoTCloud object */
  initProperties();

  /* Initialize Arduino IoT Cloud library */
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ArduinoCloud.printDebugInfo();
}


void loop() {
  ArduinoCloud.update();
}

// callback function handling cloud variable change from cloud.
void onirrigationPumpChange() 
{
  Serial.println(irrigationPump);

  if(irrigationPump)
  {
    digitalWrite(PUMP_FWD, HIGH);
  }
  else
  {
    digitalWrite(PUMP_REV, HIGH);
  }
  
  delay(1000);

  digitalWrite(PUMP_FWD, LOW);
  digitalWrite(PUMP_REV, LOW);

}
