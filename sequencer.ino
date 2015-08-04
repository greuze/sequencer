const int DIFFERENT_VALUES = 2;
const int MAX_LENGTH = 20;

int sequence[MAX_LENGTH];
int currentLength = 0;

// Set every pin to specified mode
void setMode(int mode) {
  for (int i = 0; i < DIFFERENT_VALUES; i++) {
    pinMode(i, mode);  
  }
}

// Increase the random sequence in one element
void newElement() {
  long e = random(DIFFERENT_VALUES);
  sequence[currentLength] = e;
  currentLength++;
}

void setup() {
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  setMode(INPUT);
}

void loop() {
}
