// Include Libraries
#include "RFID.h"
#include "Borehole.h"
#include "Functions.h"
#include "Network.h"
//#include "Storage.h"


// Pin Definitions
#define RFID_PIN_SDA	6
#define RFID_PIN_RST	5
#define WATERFLOW5V_PIN_SIG	A0

// object initialization
RFID rfid(RFID_PIN_SDA,RFID_PIN_RST);
//Storage storage;
Borehole borehole;
Functions lights;
Network network;
//Storage storage(10);

// define vars for gsm/gprs
boolean checkBalance = true;
boolean wasCard = false;
boolean fetch = false;
String currentCard = ""; 
float fetched = 0;

// to be static for borehole district
String district = "S01";

String dummycards[] = {"901018516249","1063116016197", "901611516228", "10661991636"};
float dummybalances[] = {1000,850, 800, 800};

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    /*pinMode(A1,OUTPUT);
    analogWrite(A1,255);*/
    pinMode(10, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(6, HIGH);
    digitalWrite(10, HIGH);
    SPI.begin();
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial); // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    //Serial.println(dummycards[1]);
    /*pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);*/
    //analogWrite(A1,255);
    ////////////////////////////////////////////////////////////////////////////
    pinMode(10, OUTPUT);
    digitalWrite(10,  HIGH);
    
    // Set SPI SS pin to output otherwise the SD library functions will not work.
    // The SD is set to use SPI SS Arduino pin 10 as chip select(CS) by default.
    // To change the pin use SD.begin(SD_CS_PIN)
    //pinMode(10, OUTPUT);
    //digitalWrite(10, HIGH);
    
    //storage.initialise();
    //pinMode(SS,OUTPUT);
    
    //initialize RFID module
    rfid.init();
    
    // initialise network
    network.init();

    // light blue to show household check
    lights.turnOn(1);
    //network.connect("households/all/S01");
    digitalWrite(A4, HIGH);
    delay(1000);
    digitalWrite(A4, LOW);
    //light red if success, otherwise red
    lights.turnOffAll();

    //Serial.println(network.getResponse());
    
   
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
  if(rfid.isCard() && currentCard == ""){
      Serial.println(F("No can do: card broadcasted no id. Trying to get actual ID."));
      while(currentCard == ""){
        if((currentCard = rfid.readTag()) == "None"){
          currentCard = "";
        }
      }
    
  }else if (rfid.isCard() && currentCard != ""){
    
    if(checkBalance){
      Serial.print("checking balance for ");
      Serial.println(currentCard);
      //return;
      Serial.print("\n");
      // dummy balance check
      // to later turn into SD check
      boolean found = false;
      for(int i = 0; i < ((sizeof(dummycards)/sizeof(*dummycards)) - 1); i++){
        Serial.println(dummybalances[i]);
        if(currentCard == dummycards[i]){
          if(dummybalances[i] <= 0){
            network.connect("balance/"+currentCard);
            
            DynamicJsonBuffer responseBuffer;
            JsonObject& responseRoot = responseBuffer.parseObject(network.getResponse());
            dummybalances[i] = (int)responseRoot["max_fetch"];
          }
          borehole.setMaxWaterToDraw(dummybalances[i]);
          Serial.println("Max water to take is : ");
          Serial.println(borehole.getMaxWaterVolumeToTake());
          found = true;
          break;
        }
      }  

      // check network for max fetch volume if card is not on SD card
      if(!found){
        int n = 0;
          while(n < 3){
            delay(500);
            digitalWrite(A3, HIGH);
            delay(500);
            digitalWrite(A3, LOW);
            n++;
          }
        network.connect("balance/"+currentCard);
        Serial.println(network.getResponse());
      }   

      // if no card  on server blink slower

      // switch lights on based on volume left
      if(borehole.getMaxWaterVolumeToTake() > 200){
        // turn on blue light
        Serial.println("blue light turning on");
        lights.turnOn(1);
      }else if((borehole.getMaxWaterVolumeToTake() <= 200) && (borehole.getMaxWaterVolumeToTake() >20)){
        // turn on orange light
        Serial.println("orange light turning on");
        lights.turnOn(2);
      }else{
        // turn on red light
        Serial.println("red light turning on");
        lights.turnOn(0);
      }

      // open borehole and let water flow
      borehole.open();
      
      checkBalance = false;
      fetch = true;
    }else{
      if(borehole.isOpen()){
          if(fetched < borehole.getMaxWaterVolumeToTake()){
            Serial.println(F("giving water"));
            Serial.println(analogRead(WATERFLOW5V_PIN_SIG));
            // code to fetch water
            //Serial.println(analogRead(WATERFLOW5V_PIN_SIG));
            // switch through lights as volume goes down
            if((borehole.getMaxWaterVolumeToTake() - fetched) > 200){
              // turn on blue light
              lights.turnOn(1);
            }else if(((borehole.getMaxWaterVolumeToTake() - fetched) <= 200) && ((borehole.getMaxWaterVolumeToTake() - fetched) >20)){
              // turn on orange light
              lights.turnOn(2);
            }else{
              // turn on red light
              lights.turnOn(0);
            }
            fetched++;
          }else{
            // user out of money
            Serial.println("Your card is out of money: Please make payment before you can use the card again");
              borehole.close();
              network.connect("stop/"+currentCard+"/"+fetched);
              updateDummy();
              currentCard = "";
              wasCard = true;
              lights.turnOffAll();
              fetched = 0;
          }
      }
    }
  }else if(!rfid.isCard() && currentCard != ""){
    Serial.println("Card has been removed.");
    lights.turnOffAll();
    wasCard = true;
    borehole.close();
    network.connect("stop/"+currentCard+"/"+fetched);
    updateDummy();
    currentCard = "";
    checkBalance = true;
    fetched = 0;
  }else {
    //currentCard = "";
    return;
  }
}

void updateDummy()
{
  for(int i = 0; i < ((sizeof(dummycards)/sizeof(*dummycards)) - 1); i++){
    //Serial.println(dummybalances[i]);
    if(currentCard == dummycards[i]){
      Serial.print("updating dummy : ");
      Serial.print(dummybalances[i]);
      dummybalances[i] = dummybalances[i] - fetched;
      Serial.print(" to : ");
      Serial.print(dummybalances[i]);
      break;
    }
  }
}
