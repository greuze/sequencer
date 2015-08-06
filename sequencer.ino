const int DIFFERENT_VALUES = 4;
const int MAX_LENGTH = 40;
const int SHOW_DELAY = 1000;
const int BLINK_DELAY = 300;
const int DEBOUNCE_DELAY = 200;
const int READ_WAITING_DELAY = 200;
const int FIRST_INPUT = 2;
const int FIRST_OUTPUT = 6;

int sequence[MAX_LENGTH];
int currentLength = 0;

// Increase the random sequence in one element
void newElement() {
  long e = random(DIFFERENT_VALUES);
  sequence[currentLength] = e;
  currentLength++;
}

void cleanOutput() {
  // Switch off all outputs
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    digitalWrite(i + FIRST_OUTPUT, LOW);
  }
}

// Show the current sequence (up to current length)
void showSequence() {
  digitalWrite(13, LOW);
  for (int i = 0; i < currentLength; i++) {
    cleanOutput();
    delay(SHOW_DELAY);
    // Switch on current output
    digitalWrite(sequence[i] + FIRST_OUTPUT, HIGH);
    delay(SHOW_DELAY);
  }
  cleanOutput();
  delay(SHOW_DELAY * 3);
}

void waitForUnpush() {
  int allInputsAreUnpushed = 0;
  while (allInputsAreUnpushed == 0) {
    allInputsAreUnpushed = 1;
    for (int i = 0; i < DIFFERENT_VALUES; i++) {
      // Value to check must be LOW, because input is pull up
      if (digitalRead(i + FIRST_INPUT) ==  LOW) {
        // At least one input is not unpushed
        allInputsAreUnpushed = 0;
        // Doesn't read other inputs
        break;
      }
    }
    delay(READ_WAITING_DELAY);
  }
}

// Returns the smallest input that was pushed (after waiting for it to unpush)
int readFirstPushed() {
  int pushedElement = -1;
  while (pushedElement == -1) {
    for (int i = 0; i < DIFFERENT_VALUES; i++) {
      // Value to check must be LOW, because input is pull up
      if (digitalRead(i + FIRST_INPUT) ==  LOW) {
        // Waits some milliseconds to prevent bouncing
        delay(DEBOUNCE_DELAY);
        // Pusher is still pushed
        if (digitalRead(i + FIRST_INPUT) ==  LOW) {
          pushedElement = i;
          waitForUnpush();
          // Doesn't read other inputs
          break;
        }
      }
    }
    delay(READ_WAITING_DELAY);
  }
  return pushedElement;
}

void blinkCorrectElement(int element) {
  while (true) {
    digitalWrite(element + FIRST_OUTPUT, HIGH);
    delay(BLINK_DELAY);
    digitalWrite(element + FIRST_OUTPUT, LOW);
    delay(BLINK_DELAY);
  }
}

void readSequence() {
  digitalWrite(13, HIGH);
  for (int i = 0; i < currentLength; i++) {
    int pushed = readFirstPushed();
    if (sequence[i] != pushed) {
      // Wrong element, game over
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
  // Set mode in all inputs as pullup
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    pinMode(i + FIRST_INPUT, INPUT_PULLUP);
  }
  // Set mode in all outputs
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    pinMode(i + FIRST_OUTPUT, OUTPUT);
  }
  // Will show input/output state
  pinMode(13, OUTPUT);
  // Initialize serial
  Serial.begin(9600);
}

void loop() {
  newElement();
  showSequence();
  readSequence();
}

