#include "UsbKeyboard.h"
#include "IoExtender.h"
#include "EventFunctions.h"

#define LAZY_INIT_DELAY 1000
#define SEND_KEYS 1

#define STATUS_LED_PIN 15
#define GREEN_LED_PIN 16
#define YELLOW_LED_PIN 17

#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3

#define KEY_SCROLL_LOCK 71


char hello_pressed[] = { KEY_H, KEY_E, KEY_L, KEY_L, KEY_O, KEY_SPACE, 0x00 };
char world_released[] = { KEY_W, KEY_O, KEY_R, KEY_L, KEY_D, 0x00 };

IoExtender *dyn_all_extenders[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};

bool lazy_setup_initialized = 0;
uint16_t last_btn_update_time = 0;

void setup() {     
  Serial.begin(9600);     

  UsbKeyboard.update();
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  digitalWrite(STATUS_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
}

uint8_t delayed_setup() {
  uint8_t setup_status = 0;
  uint8_t io_extender_status = -1;
  uint8_t io_extender_status_global = -1;
  IoExtender *extender;
  
  Serial.println("--------------------");
  Serial.println("     Lazy init      ");
  Serial.println("--------------------");


  // initializing multiplexers 
  for (uint8_t ext_number = 0; ext_number < IO_EXTENDER_NUMBER; ext_number++) {
    extender = new IoExtender();
    io_extender_status = extender->init(IO_EXTENDER_START_ADDRESS + ext_number);      
    
    if (io_extender_status == 0) {      
      // multiplexer found - so we need to initialize it
      Serial.println("FOUND");
      
      extender->invertPortsPolarity();
      extender->getPortsStatus();  

      dyn_all_extenders[ext_number] = extender;
      
      io_extender_status_global = 0;      
    } else {
      // multiplexer not found - we null it so it wont bother as later            
      Serial.println("NOT FOUND");
      
      delete extender;
    }    
  }
  
  if (io_extender_status_global == 0) {
    // at least one multiplexer found    
    digitalWrite(YELLOW_LED_PIN, LOW);
  } else {
    setup_status++;
  }

  // checking keyboard connection
  UsbKeyboard.sendKeyStroke(KEY_SCROLL_LOCK);
  UsbKeyboard.sendKeyStroke(KEY_SCROLL_LOCK);
  
  digitalWrite(GREEN_LED_PIN, LOW);

  return setup_status;
}

void handle_buttons() {
  uint8_t btn_num;
  int8_t changes[IO_EXTENDER_PORT_NUMBER];

  for (uint8_t io_number=0;io_number<IO_EXTENDER_NUMBER;io_number++) {
    if (dyn_all_extenders[io_number] != NULL) {
      
      dyn_all_extenders[io_number]->getChanges(changes);
      for (btn_num=0;btn_num<IO_EXTENDER_PORT_NUMBER;btn_num++) {
        if (changes[btn_num] != 0) {

          Serial.print("IO");
          Serial.print(io_number);          
          Serial.print("BTN");
          Serial.print(btn_num);
          
          if (changes[btn_num] > 0) {
            Serial.println("_PRESSED");  
            if (SEND_KEYS) {  
              (*pressed_event[(IO_EXTENDER_PORT_NUMBER*io_number)+btn_num])();
            }
          } else {
            Serial.println("_RELEASED");
            if (SEND_KEYS) {
              (*released_event[(IO_EXTENDER_PORT_NUMBER*io_number)+btn_num])();
            }
          }      
        } 
      }       
    }
  }      
}

void loop() {  
  UsbKeyboard.update();
  
  if (lazy_setup_initialized == 0) {
    pre_init_loop();
  } else {
    after_init_loop();
  }
  
  delay(10);
}

void pre_init_loop() {
  uint16_t current_time = millis(); 

  if ((current_time / 100) % 2 == 0) {
    digitalWrite(STATUS_LED_PIN, HIGH);
  } else {
    digitalWrite(STATUS_LED_PIN, LOW);
  }
  
  if (current_time > LAZY_INIT_DELAY) {
    uint8_t delayed_status = delayed_setup();    
    if (delayed_status == 0) {
      lazy_setup_initialized = 1;       
    }
  }  
}

void after_init_loop() {
  uint16_t current_time = millis(); 

  if ((current_time / 100) % 50 == 0) {
    digitalWrite(STATUS_LED_PIN, LOW);
  } else {
    digitalWrite(STATUS_LED_PIN, HIGH);
  }

  if (current_time - last_btn_update_time > 100) {
    handle_buttons();  
    last_btn_update_time = current_time;
  }  
}


void sendKeyboardInput(char input[]) {
  int c;
  int i=0;
  int limit = 100; // dla bezpieczeństwa :s

  Serial.println("Sending USB: ");
  do {
    limit-=1;
    c = input[i++];
    if (c != 0x00) {
      Serial.print(c);
      Serial.print(":");
      UsbKeyboard.sendKeyStroke((byte)c);
    }
    
  } while (c != 0x00 && limit > 0);

  Serial.println("\nEND USB");
}

