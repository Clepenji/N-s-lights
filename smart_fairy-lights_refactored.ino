const int light_sensor_pin = A2;
const int light_pin = 10;
const int button_pin = 4;


enum LightState {
  ON_TO_OFF,
  IDLE_OFF,
  OFF_TO_ON,
  ON,
  DISABLE
};

enum AmbientState {
  BRIGHT,
  DARK,
  BRIGHT_TO_DARK,
  DARK_TO_BRIGHT
};

struct Task {
  LightState state;
  AmbientState activated_when;
};

struct linked_list {
  int data;
  struct linked_list* next;
};


struct Task lights;
AmbientState AmbientStateIs;

struct linked_list readings_array[6];
unsigned long last_ambience_sensing = 0;
const unsigned long ambience_sensing_delay = 15; //millis

const unsigned long disable_timeout = 1200000; //20 min
const unsigned long auto_on_timeout = 1800000;
unsigned long disabled_since, auto_on_since;


void setup() {
  //Initialising pins
  pinMode(light_pin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);

  //linking linked list
  for (int i = 0; i < 5; i++) {
    readings_array[i].next = &readings_array[i + 1];
  }
  readings_array[5].next = &readings_array[0];

  Serial.begin(9600);

}

void loop() {
  unsigned long currentMillis = millis();
  boolean clicked_once = false;
  boolean timeout = false;


  int clicks = read_button(currentMillis);
  if (clicks == 1) clicked_once = true;

  if (currentMillis - last_ambience_sensing >= ambience_sensing_delay) {
    sense_ambience(AmbientStateIs);
    last_ambience_sensing = currentMillis;
  }

  //timeouts
  if (lights.state == DISABLE) {
    if (currentMillis - disabled_since >= disable_timeout) {
      timeout = true;
    }
  } else if (lights.state == ON && lights.activated_when == DARK && AmbientStateIs == DARK_TO_BRIGHT) disabled_since = currentMillis; //start counting when next state is DISABLED

  if (lights.state == ON && lights.activated_when == BRIGHT_TO_DARK) {
    if (currentMillis - auto_on_since >= auto_on_timeout) {
      timeout = true;
    }
  } else if (lights.state == OFF_TO_ON && AmbientStateIs == BRIGHT_TO_DARK) auto_on_since = currentMillis; //start counting when auto ON is triggered





  if (lights.state == OFF_TO_ON) digitalWrite(light_pin, HIGH);
  if (lights.state == ON_TO_OFF || lights.state == DISABLE) digitalWrite(light_pin, LOW);



  // state changes
  if (lights.state == ON_TO_OFF) lights.state = IDLE_OFF;

  else if (lights.state == IDLE_OFF) {
    if (clicked_once || (AmbientStateIs == BRIGHT_TO_DARK)) lights.state = OFF_TO_ON;
  }

  else if (lights.state == OFF_TO_ON) {
    lights.state = ON;
    lights.activated_when = AmbientStateIs;
  }

  else if (lights.activated_when == BRIGHT) {
    if (lights.state == ON) {
      if (clicked_once) lights.state = ON_TO_OFF;
    }
  }
  else if (lights.activated_when == BRIGHT_TO_DARK) {
    if (lights.state == ON) {
      if (clicked_once | (AmbientStateIs == BRIGHT) | timeout) lights.state = ON_TO_OFF;
    }
  }
  else if (lights.activated_when == DARK) {
    if (lights.state == ON) {
      if (clicked_once) lights.state = ON_TO_OFF;
      else if (AmbientStateIs == DARK_TO_BRIGHT) lights.state = DISABLE;
    }
    else if (lights.state == DISABLE) {
      if (timeout) lights.state = ON_TO_OFF;
      else if (AmbientStateIs == DARK) lights.state = OFF_TO_ON;
      else if (clicked_once) {
        lights.state = OFF_TO_ON;
      }
    }

  }
}



void sense_ambience(AmbientState AmbientStateIs) {
  int it_is_dark = 300;
  int sharp_ambience_change = 200;
  int max_reading, min_reading = 1024;
  static struct linked_list *ptr = readings_array;

  ptr->data = analogRead(light_sensor_pin);
  if (ptr->data > it_is_dark) AmbientStateIs = BRIGHT;
  else AmbientStateIs = DARK;

  ptr = ptr->next;


  for (int i = 0; i < 6; i++) {
    if (readings_array[i].data > max_reading) max_reading = readings_array[i].data;
    if (readings_array[i].data < min_reading) min_reading = readings_array[i].data;
  }

  if (max_reading - min_reading > sharp_ambience_change) {
    if (AmbientStateIs == BRIGHT) AmbientStateIs = DARK_TO_BRIGHT;
    else AmbientStateIs = BRIGHT_TO_DARK;
  }
}




int read_button(unsigned long currentMillis) {
  int reading = !digitalRead(button_pin);
  static int buttonState, lastButtonReading;
  static int clicks;
  const int clickDebounceTime = 20;
  const int buttonResponseTime = 650;
  //const int longPressTime;

  static unsigned long last_reading_change = 0;
  static unsigned long last_state_change = 0;

  if (reading != lastButtonReading) last_reading_change = currentMillis;

  if (currentMillis - last_reading_change >= clickDebounceTime) {
    if (reading != buttonState) {
      clicks++;
      last_state_change = currentMillis;
      buttonState = reading;
    }
  }

  lastButtonReading = reading;

  if ((currentMillis - last_state_change >= buttonResponseTime) && clicks) {
    int temp = clicks;
    clicks = 0;
    return temp;
  }

  return 0;
}
