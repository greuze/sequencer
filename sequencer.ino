const int DIFFERENT_VALUES = 4;
const int MAX_LENGTH = 20;
const int SHOW_DELAY = 1000;
const int BLINK_DELAY = 300;
const int DEBOUNCE_DELAY = 50;

int sequence[MAX_LENGTH];
int currentLength = 0;

// Set every pin to specified mode
void setMode(int mode) {
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    pinMode(i, mode);
  }
  // To show current mode
  if (mode == OUTPUT) {
    digitalWrite(13, LOW);
  } else {
    digitalWrite(13, HIGH);
  }
}

// Increase the random sequence in one element
void newElement() {
  long e = random(DIFFERENT_VALUES);
  sequence[currentLength] = e;
  currentLength++;
}

void cleanOutput() {
  // Switch off all outputs
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    digitalWrite(i, LOW);
  }
}

// Show the current sequence (up to current length)
void showSequence() {
  setMode(OUTPUT);
  for (int i = 0; i < currentLength; i++) {
    cleanOutput();
    delay(SHOW_DELAY);
    // Switch on current output
    digitalWrite(sequence[i], HIGH);
    delay(SHOW_DELAY);
  }
  cleanOutput();
}

void waitForUnpush() {
  int allInputsAreUnpushed = 0;
  while (allInputsAreUnpushed == 0) {
    allInputsAreUnpushed = 1;
    for (int i = 0; i < DIFFERENT_VALUES; i++) {
      if (digitalRead(i) ==  HIGH) {
        // At least one input is not unpushed
        allInputsAreUnpushed = 0;
        // Doesn't read other inputs
        break;
      }
    }
  }
}

// Returns the smallest input that was pushed (after waiting for it to unpush)
int readFirstPushed() {
  int pushedElement = -1;
  while (pushedElement == -1) {
    for (int i = 0; i < DIFFERENT_VALUES; i++) {
      if (digitalRead(i) ==  HIGH) {
        // Waits some milliseconds to prevent bouncing
        delay(DEBOUNCE_DELAY);

        waitForUnpush();

        pushedElement = i;
        // Doesn't read other inputs
        break;
      }
    }
  }
  return pushedElement;
}

void blinkCorrectElement(int element) {
  setMode(OUTPUT);
  while (true) {
    digitalWrite(element, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(element, LOW);
    delay(BLINK_DELAY);
  }
}

void readSequence() {
  setMode(INPUT);
  for (int i = 0; i < currentLength; i++) {
    int pushed = readFirstPushed();
    if (sequence[i] != pushed) {
      // Wrong element, game over
      Serial.begin(9600);
      Serial.println("Game over");
      Serial.print("Sequence was ");
      Serial.print(sequence[i]);
      Serial.print(" but pushed ");
      Serial.println(pushed);
      Serial.println("Done\n");

      blinkCorrectElement(sequence[i]);
    }
  }
}

void setup() {
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  // Will show input/output state
  pinMode(13, OUTPUT);
}

void loop() {
  newElement();
  showSequence();
  readSequence();
}

