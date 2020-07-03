#DEFINE changeTickModePin 19
#DEFINE timingPin 5
#DEFINE statePin0 6
#DEFINE statePin1 7

// 0 - clock
// 1 - while
// 2 - manual

uint_8 timingMode = 0x01b;

void pre () {
    pinMode(timingPin, INPUT_PULLUP);
    pinMode(statePin0, INPUT_PULLUP);
    pinMode(statePin1, INPUT_PULLUP);
    pinMode(changeTickModePin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(changeTickModePin), blink, CHANGE);
}

void change_timing () {

    int newTimingMode = digitalRead(statePin0) + 2*digitalRead(statePin1);
    if (newTimingMode != 3) timingMode = newTimingMode;

}

void loop () {
    

    if (timingMode == 2) {
        while (!digitalRead(timingPin)) {
        }
    }
    
    
}