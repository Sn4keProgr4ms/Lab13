#include "MQTT.h"
#include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

double latitude = 0;
double longitude = 0;

String theTopic = "remylab13/latitude";

bool timer_active = false;

Timer timer(2000, timer_end);

OledWingAdafruit display;

void setup()
{
  display.setup();
  client.subscribe("remylab13/latitude");
  client.subscribe("remylab13/longitude");
}

void loop()
{
  display.loop();
  if (client.isConnected())
  {
    client.loop();
    if (!timer_active)
    {
      timer.start();
      timer_active = true;
      client.publish("remylab13/sendrequest", "0");
    }
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe("remylab13/latitude");
    client.subscribe("remylab13/longitude");
  }

  ();

  display.println("latitude Longitude");

  display.print(latitude);
  display.print(" : ");
  display.print(longitude);
  display.display();
}

void callback(char *topic, byte *payload, unsigned int length)
{

  // formats and extracts data from mqtt
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();

  String callbackTopic = topic; // makes the mqtt topic of the data into an arduino string

  if (theTopic.compareTo(callbackTopic))
  {
    latitude = value;
  }
  else
  {
    longitude = value;
  }
}

void format_display()
{
  // sets up basic formatting for when using the display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void timer_end()
{
  timer_active = false;
}