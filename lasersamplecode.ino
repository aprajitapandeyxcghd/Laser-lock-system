#include <Keypad.h>
#include <Servo.h>

const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Servo myservo;

// Define multiple correct passwords in an array
const char* correctPasswords[] = {"6789", "8765", "4321","7973","2511"};
int numPasswords = 5; // Number of correct passwords

char enteredPassword[5] = "";
int passwordIndex = 0;
int maxPasswordLength = 4;

void setup() {
  Serial.begin(9600);
  Serial.println("Password Unlock System");
  Serial.println("Enter the password followed by # key.");
  myservo.attach(11); // Attach the servo to pin 11
  myservo.write(0);   // Initialize the servo to 0 degrees
}

void unlock() {
  // Move the servo from 0 to 180 degrees
  for (int pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15); // Adjust the speed of servo movement here
  }
  delay(10000); // Pause for 1 second with the servo at 180 degrees
  // Move the servo back to 0 degrees
  for (int pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15); // Adjust the speed of servo movement here
  }
  delay(1000); // Pause for 1 second with the servo at 0 degrees
  myservo.write(0); // Set the servo back to 0 degrees
  Serial.println("System locked.");
  clearPassword();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    if (key == '#') {
      bool isCorrectPassword = false;
      for (int i = 0; i < numPasswords; i++) {
        if (strcmp(enteredPassword, correctPasswords[i]) == 0) {
          // Password is correct
          Serial.println("Access granted!");
          unlock();
          isCorrectPassword = true;
          break; // Exit the loop once a correct password is found
        }
      }

      if (!isCorrectPassword) {
        // Incorrect password
        Serial.println("Incorrect password. Please try again.");
        clearPassword();
      }
    } else if (key == '*') {
      Serial.println("Password cleared.");
      clearPassword();
    } else {
      if (passwordIndex < maxPasswordLength) {
        enteredPassword[passwordIndex] = key;
        passwordIndex++;
      }
    }
  }
}

void clearPassword() {
  for (int i = 0; i < maxPasswordLength; i++) {
    enteredPassword[i] = '\0'; // Clear the entered password
  }
  passwordIndex = 0;
}
