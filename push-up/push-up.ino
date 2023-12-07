#define echoPin  2
#define trigPin  3
#define resetPin 4
#define whiteLedPin 5
#define redLedPin 6

float duration; // microseconds
float cm;
long numDisplay = 100;
boolean trigUp = false;
boolean trigDown = false;
int counterPushUp = 0;
int buzzer = 7; // Passive buzzer pin
boolean pushUpCompleted = false; // Flag to indicate push-up completion
int setOfPushUps = 0; // Variable to keep track of sets

unsigned long startTime; // To track the start time
const unsigned long pushUpDuration = 180000; // 3 minutes in milliseconds
const int pushUpTarget = 10; // Target number of push-ups per set

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(whiteLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(resetPin, INPUT_PULLUP);
  
  digitalWrite(whiteLedPin, LOW); // Initialize the white LED to off
  digitalWrite(redLedPin, LOW); // Initialize the red LED to off
  
  delay(500);
  startTime = millis(); // Initialize the start time
}

void loop() {
  // Reset by switch
  if (digitalRead(resetPin) == 0) {
    trigUp = false;
    trigDown = false;
    counterPushUp = 0;
    pushUpCompleted = false; // Reset the flag
    startTime = millis(); // Reset the start time
    digitalWrite(whiteLedPin, LOW); // Turn off the white LED
    digitalWrite(redLedPin, LOW); // Turn off the red LED
  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  
  // Receive, convert time (us) to cm
  duration = pulseIn(echoPin, HIGH);
  cm = duration * 0.034 / 2.0;
  
  if (cm > 15 && cm <= 30) {
    trigUp = true;
  } 
  else if (cm < 10) {
    trigDown = true;
  }
  
  if (trigUp && trigDown) {
    counterPushUp++; // Increment the push-up count
    trigUp = false;
    delay(500);
    tone(7, 800, 40); // Buzzer pin 7, frequency 800, duration 500
    trigDown = false;
    
    // Print the set count and push-up count to the Serial Monitor
    Serial.print("Set Count: ");
    Serial.print(setOfPushUps);
    Serial.print(" - Push-up Count: ");
    Serial.println(counterPushUp);
    
    // Check if the target push-up count is reached within the time limit
    if (counterPushUp >= pushUpTarget && millis() - startTime <= pushUpDuration) {
      // Turn on the white LED
      digitalWrite(whiteLedPin, HIGH);
      digitalWrite(redLedPin, LOW); // Turn off the red LED
      // Reset the push-up count to 1 for the next set
      counterPushUp = 0;
      // Increment the set count
      setOfPushUps++;
    } else {
      // Turn on the red LED
      digitalWrite(redLedPin, HIGH);
      digitalWrite(whiteLedPin, LOW); // Turn off the white LED
    }
  }
}
