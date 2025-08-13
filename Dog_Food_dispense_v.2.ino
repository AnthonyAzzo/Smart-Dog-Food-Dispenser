#include <Wire.h> // for I2C communication for RTC 
#include <RTClib.h> // RTC library
#include <AccelStepper.h> // stepper motor 

// Pins for A4988 motor driver
  const int Step_pin = 9;
  const int Dirc_pin = 8;

  // declaring the stepper motor as an object
  AccelStepper stepper(AccelStepper::DRIVER, Step_pin, Dirc_pin);

  //Pins for push buttons
  const int ManualDispense_pin = 2;
  const int SpeedUp_pin = 3;
  const int SpeedDown_pin = 4;

  // declaring the RTC as an object
  RTC_DS3231 rtc;

  // current speed of motor
  float currentSpeed = 1000.0;

  // Morning & evening Feeding boolean variable
  bool morningFoodDispense = false;
  bool eveningFeedDone = false;

  // "Stopwatch" variables to measure dispense time
  unsigned long dispenseStartTime = 0;
  // *** dispenseDuration is no longer needed for this logic
  // unsigned long dispenseDuration = 0;

  // The desired duration for the motor to run at speed, in milliseconds
  const unsigned long dispenseRunDuration = 3000; // e.g., 1.5 seconds

  // A flag to track if we are in a timed dispense cycle
  bool isDispensing = false;

void setup() {
  Serial.begin(9600);
  
  if(!rtc.begin()){
    Serial.println("Unable to find RTC");
    while(1);
  }

  // Configuring the pushButtons
  pinMode(ManualDispense_pin, INPUT_PULLUP);
  pinMode(SpeedUp_pin, INPUT_PULLUP);
  pinMode(SpeedDown_pin, INPUT_PULLUP);

  // Motor initial speed & acceleration 
  stepper.setMaxSpeed(currentSpeed);
  stepper.setAcceleration(10000);
  
  //if (rtc.lostPower()) {
    // This line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //}

}

void loop() {

  // *** CHECKS TO START A TIMED DISPENSE CYCLE ***
  // Checks if ManualDispense button is pressed & if motor is stopped.
  if(digitalRead(ManualDispense_pin) == LOW && !isDispensing && stepper.distanceToGo() == 0){ 
    Serial.println("Starting timed dispense...");
    stepper.moveTo(1000000);      // *** CHANGE: Move towards a distant target to start continuous running
    isDispensing = true;           // Set the flag
    dispenseStartTime = millis();  // START the stopwatch
  }

  // *** CHECKS TO STOP A TIMED DISPENSE CYCLE ***
  // If we are in a dispense cycle, check if it's time to stop.
  if(isDispensing && (millis() - dispenseStartTime >= dispenseRunDuration)) {
    Serial.println("Time is up! Stopping motor smoothly...");
    stepper.stop(); // *** NEW: Tell the motor to decelerate and stop
    isDispensing = false; // Reset the flag
  }

 // If the stopwatch was started and the motor is now stopped, calculate and print the time.
  if (dispenseStartTime != 0 && stepper.distanceToGo() == 0) {
    unsigned long totalDuration = millis() - dispenseStartTime; // STOP the stopwatch
    
    Serial.print("Total dispense cycle (including deceleration) took: ");
    Serial.print(totalDuration);
    Serial.println(" ms");

    dispenseStartTime = 0; // Reset the stopwatch for the next run
  }

  // Check if Speed Up button is pushed 
  if(digitalRead(SpeedUp_pin) == LOW){
    currentSpeed += 50;
    stepper.setMaxSpeed(currentSpeed);
    Serial.print("SpeedUp current speed: ");
    Serial.println(currentSpeed);
    delay(250);
  }
  
  // Check if Speed Down button is pushed 
  if (digitalRead(SpeedDown_pin) == LOW){
    currentSpeed -= 50;
    stepper.setMaxSpeed(currentSpeed);
    Serial.print("SpeedDown current speed: ");
    Serial.println(currentSpeed);
    delay(250);
  }
  
  // This line is crucial! It keeps the motor moving based on the commands.
  stepper.run();

  // --- RTC ---
  // The scheduled feeding logic remains the same, using fixed steps.
  DateTime now = rtc.now();
  if (now.hour() == 7 && now.minute() == 0 && morningFoodDispense == false){
    stepper.move(1600); // Using fixed steps for scheduled feeds is fine
    morningFoodDispense = true;
  }

  if(now.hour() == 8 && now.minute() == 0 && morningFoodDispense == true){
    morningFoodDispense = false;
    Serial.println("Ready for tomorrow's morning feed.");
  }

  if (now.hour() == 16 && now.minute() == 0 && eveningFeedDone == false){
    stepper.move(1600);
    eveningFeedDone = true;
  }

  if(now.hour() == 17 && now.minute() == 0 && eveningFeedDone == true){
    eveningFeedDone = false;
    Serial.println("Ready for tomorrow's evening feed.");
  }
}
