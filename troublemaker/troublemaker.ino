#include <Adafruit_NeoPixel.h>
#include <Preferences.h>
#include <Arduino.h>
#include <WiFi.h>

#define NUMPIXELS 2
#define PIN 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Preferences persistance;

long pixelDelay = 50;
long lastPixelMillis = 0;
unsigned long runTime;
unsigned long advTime;
unsigned long scannedTime;
int r = 255;
int g = 0;
int b = 0;
int n = 0;
int i = 0;
int pinState = 0;
String command;
String rick[8] = {
      "Never gonna give you up",
      "Never gonna let you down",
      "Never gonna run around",
      "Never gonna desert you",
      "Never gonna make you cry",
      "Never gonna say goodbye",
      "Never gonna tell a lie",
      "Never gonna hurt you"
    };
String ssids[] = {};
bool scanned = false;
bool pressed = false;
bool debugOn = false;
bool useRick = false;
bool solved = false;
bool wardrive = false;

String makeSSID() {
  const char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  String randomString = "";
  for (int i = 0; i < random(5, 20); i++) {
    randomString += letters[random(0, 51)];
  }
  return randomString;
}

void setup() {
  Serial.begin(9600);

  persistance.begin("ctf", false);
  solved = persistance.getBool("solved", false);

  pixels.begin();
  pixels.show();
  pixels.setBrightness(25);

  randomSeed(analogRead(8));

  pinMode(11, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  pinState = digitalRead(11);
  if(pinState == LOW){
    pressed = true;
  }

  delay(100);
}

void loop() {
  runTime = millis();

  if (runTime - lastPixelMillis > pixelDelay) {
    for (int i = 0; i < NUMPIXELS; i++) {
      if(!useRick){
        r = random(255);
        g = random(255);
        b = random(255);
      } else {
        r = random(200, 255);
        g = random(50);
        b = random(50);
      }

      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
    lastPixelMillis = millis();
  }

  // scanner
  if(!scanned){
    scanned = true;
    if(debugOn){
      Serial.println("IMMA CHARGIN' MALASIA");
    }
    n = WiFi.scanNetworks();
    i = 0;
    scannedTime = millis();
    pinState = digitalRead(0);
    if(pinState == LOW){
      wardrive = true;
      useRick = false;
      pixelDelay = 250;
      if(debugOn){
        Serial.println("wardrive mode enabled!");
      }
    }
  }

  // trolling code
  if(n > 0 && !useRick && !wardrive){
    if(i <= n){
      if(runTime - advTime >= 500){
        if(WiFi.BSSIDstr(i) != ""){
          if(debugOn){
            Serial.print("trolling ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" - ");
            Serial.println(WiFi.BSSIDstr(i));
          }
          WiFi.softAP(WiFi.SSID(i));
          advTime = millis();
          i++;
        } else {
          i++;
        }
      }
    } else {
      i = 0;
    }
  } else {
    if(useRick){
      if(i <= 7){
        if(runTime - advTime >= 500){
          if(debugOn){
            Serial.print("trolling ");
            Serial.println(rick[i]);
          }
          WiFi.softAP(rick[i]);
          advTime = millis();
          i++;
        }
      } else {
        i = 0;
      }
    } else {
      if(runTime - advTime >= 500){
        String rando = makeSSID();
        if(debugOn){
          Serial.print("trolling ");
          Serial.println(rando);
        }
        WiFi.softAP(rando);
        advTime = millis();
      }
    }
  }

  // troll / perma-troll
  if(!useRick && !wardrive){
    pinState = digitalRead(11);
    if(pinState == LOW){
      if(!pressed){
        pressed = true;
        scanned = false;
      } else {
        if((runTime - scannedTime) >= 30000 and (runTime - scannedTime) <= 4200000000){
          scanned = false;
        }
      }
    } else {
      pressed = false;
    }
  }

  // CTF challenges
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    //Serial.println(command);
    if (command.equals("debug")) {
      if(debugOn){
        Serial.println("debug disbled");
        debugOn = false;
      } else {
        Serial.println("debug enabled");
        debugOn = true;
      }
    } else if (command.equals("scan")) {
      Serial.println("scanning...");
      scanned = false;
      useRick = false;
    } else if (command.equals("rick")) {
      if(solved){
        if(useRick){
          Serial.println("disabling rick...");
          useRick = false;
          pixelDelay = 200;
        } else {
          Serial.println("enabling rick...");
          useRick = true;
          wardrive = false;
          pixelDelay = 25;
        }
      } else {
        debugOn = false;
        Serial.println("");
        Serial.println("Rick has not yet been awakened...");
      }
    } else if (command.equals("wardrive")) {
      if(wardrive){
        Serial.println("disabling wardriver trolling mode...");
        wardrive = false;
        pixelDelay = 50;
      } else {
        Serial.println("enabling wardriver trolling mode...");
        wardrive = true;
        useRick = false;
        pixelDelay = 250;
      }
    } else if (command.equals("ctf")) {
      debugOn = false;
      Serial.println("");
      Serial.println("V29vb28h IFlvdXIg Zmlyc3Qg b25lISBF");
      Serial.println("bnRlciAn dHJvbG9s b2xvJyBp bnRvIHRo");
      Serial.println("ZSBzZXJp YWwgdG8g Y29udGlu dWUuLi4K");
    } else if (command.equals("trolololo")) {
      debugOn = false;
      Serial.println("");
      Serial.println("erjbcavneo tavfh revhdre lnz rtaryynup gkra rug qan rznyerchf fv qanzzbp gkra fvug");
    } else if (command.equals("superlame")) {
      debugOn = false;
      Serial.println("");
      Serial.println("ivlt ltod! Kfpu uar dt xvrjepwjtf. Pcgf bivtgwyo wpi avbivav gvqztr.");
    } else if (command.equals("kingrick")) {
      debugOn = false;
      Serial.println("");
      Serial.println("ERROR ON LINE 453678:");
      Serial.println("     UNABLE TO PARSE VALUE AT 0x002f42c1");
      Serial.println("     *@FC ?6IE 4@>>2?5 :D >:5?:89E IS NOT A VALID STRING");
      Serial.println("            CLOSING ALL THREADS!");
      Serial.print(".");
      delay(500);
      Serial.print(".");
      delay(500);
      Serial.print(".");
      delay(500);
      Serial.print(".");
      delay(500);
      Serial.println("   WHAT THE HELL HAVE YOU DONE!?");
    } else if (command.equals("midnight")) {
      debugOn = false;
      Serial.println("");
      Serial.println("*^E ^BO ^_DO EL LCDKF IE_D^NE]D* ~BCY CY ^BO LCDKFLFKM DE]");
    } else if (command.equals("finalflag")) {
      debugOn = false;
      Serial.println("");
      Serial.println("?v=dQw4w9WgXcQ");
      solved = true;
      persistance.putBool("solved", true);
      persistance.end();
    } else if (command.equals("owo")) {
      debugOn = false;
      Serial.println("");
      Serial.println("uwu");
    } else if (command.equals("uwu")) {
      debugOn = false;
      Serial.println("");
      Serial.println("owo");
    } else if (command.equals("whoami")) {
      debugOn = false;
      Serial.println("");
      Serial.println("root");
    } else if (command.equals("ls")) {
      debugOn = false;
      Serial.println("");
      Serial.println("answers");
      Serial.println("bin");
      Serial.println("documentation");
      Serial.println("etc");
      Serial.println("lib");
    } else if (command.startsWith("cd ")) {
      debugOn = false;
      Serial.println("");
      Serial.println("lol good try.");
      delay(500);
      Serial.println("This ain't linux though.");
    } else {
      debugOn = false;
      Serial.println("");
      Serial.println("Available commands:");
      Serial.println(" - debug :: Enables debug mode on serial console");
      Serial.println(" - scan :: Initiates scan for new SSIDs to troll");
      Serial.println(" - wardrive :: Uses random SSIDs to troll wardrivers");
      Serial.println(" - ctf :: Starts trollbadge mini-CTF");
      if(solved){
        Serial.println(" - rick :: Enables Rick mode for better SSIDs");
      }
    }
  }
}
