/* Smart parking system using IR sensors and IoT

   Hardware used
        ESP8266
        LCD
        IR sensors
        RFID
        Buzzer
        Servo motor

*/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL3LZCCMjI8"
#define BLYNK_TEMPLATE_NAME "Car parking"
#define BLYNK_AUTH_TOKEN "pGBgvVFiN2NM2tSw80k62cDP0lAMvFUX"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
char ssid[] = "realmeXT";
char pass[] = "oc73xush";
//char ssid[] = "GalaxyM12";
//char pass[] = "Manju123";

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#include <Servo.h>
Servo myservo;  // create servo object to control a servo

BlynkTimer timer;

const int BUZZER = D0;
const int SENSOR0 = D6;
const int SENSOR1 = D7;
const int SENSOR2 = D5;

WidgetLED led0(V1);
WidgetLED led1(V3);
WidgetLED led2(V5);

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_RED       "#D3435C"
#define BLYNK_YELLOW    "#ED9D00"


int lotStatus0, lotStatus1, lotStatus2;
String user0 = "1600547CB48A", user1 = "160054172A7F", user2 = "15008DAB2211";

void setup()
{
  // LCD initialisation
  lcd.init();
  lcd.clear();

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(1, 1);
  lcd.print("WiFi .....!");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(1, 3);
  lcd.print("System!");

  // Sensors initialisation
  pinMode(SENSOR0, INPUT);
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);


  // Initialise serial to 115200 baud rate
  Serial.begin(9600);
  delay(2000);
  Serial.flush();
  pinMode(BUZZER, INPUT);

  // Connect to Blynk server
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  myservo.attach(D4);  // attaches the servo on pin 9 to the servo object

  // Turn on widget leds
  led0.on();
  led1.on();
  led2.on();

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Online");
  delay(2000);


}


void loop()
{
  //Run the Blynk library
  Blynk.run();

  checkLotStatus();

  if (Serial.available() > 0)
  {
    int count = 0;                                          // count = 0
    char input[12];
    while (Serial.available() && count < 12)         // Read 12 characters and store them in input array
    {
      input[count] = char(Serial.read());
      count = count + 1;
      delay(5);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RFID Detected");
    lcd.setCursor(1, 1);
    lcd.print(input);

    // Open and close gates
    myservo.write(180);
    delay(5000);
    myservo.write(0);

    Serial.println("RFID DETECTED");
    Serial.println(input);
    delay(5000);
    Serial.flush();

  }
  else
  {
    printLotStatus();
  }

}


void printLotStatus()
{
  if ((lotStatus0)  && (lotStatus1) && (lotStatus2))
  {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parking Full");
      delay(2000);
  }

  else
  {
    if (lotStatus0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 0:");
      lcd.setCursor(1, 1);
      lcd.print("Occupied");
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 0:");
      lcd.setCursor(1, 1);
      lcd.print("Free");
    }
    delay(2000);


    if (lotStatus1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 1:");
      lcd.setCursor(1, 1);
      lcd.print("Occupied");
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 1:");
      lcd.setCursor(1, 1);
      lcd.print("Free");
    }
    delay(2000);


    if (lotStatus2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 2:");
      lcd.setCursor(1, 1);
      lcd.print("Occupied");
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lot 2:");
      lcd.setCursor(1, 1);
      lcd.print("Free");
    }
    delay(2000);
  }
}

void buzz()
{
  for (int i = 0; i < 20; i++)
  {
    pinMode(BUZZER, OUTPUT); // Configuring Input and output will make the buzzer beep
    delay(100);
    pinMode(BUZZER, INPUT);
    delay(100);
  }
}

void checkLotStatus()
{
  lotStatus0 = !(digitalRead(SENSOR0));
  if (lotStatus0)
  {
    led0.setColor(BLYNK_RED);
  }
  else
  {
    led0.setColor(BLYNK_GREEN);
  }

  lotStatus1 = !(digitalRead(SENSOR1));
  if (lotStatus1)
  {
    led1.setColor(BLYNK_RED);
  }
  else
  {
    led1.setColor(BLYNK_GREEN);
  }

  lotStatus2 = !(digitalRead(SENSOR2));
  if (lotStatus2)
  {
    led2.setColor(BLYNK_RED);
  }
  else
  {
    led2.setColor(BLYNK_GREEN);
  }
}
