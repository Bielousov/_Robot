// Environment sensors thread
Thread environmentThread = Thread();

const uint16_t ENVIRONMENT_SCAN_FREQUENCY = 500;          // Environment sensors read frequency, in ms

void initEnvironmet() {
    pinMode(PIN_PIR_SENSOR, INPUT);
    
    environmentThread.onRun(onEnvironmentUpdate);
    environmentThread.setInterval(ENVIRONMENT_SCAN_FREQUENCY);
}

/*
 * Public Methods
 */
bool isHumanDetected() {
  return State.Environment.isHumanDetected;
}

uint16_t getTimeSinceHumanDetected() {
  return getTime() - State.Environment.timeHumanDetected;
}

uint16_t getTimeSinceHumanLost() {
  return getTime() - State.Environment.timeHumanLost;
}


/*
 * Events
 */
void onHumanDetected() {
  if (!State.Environment.isHumanDetected) {
    // TODO: State change event
  }
  
  State.Environment.isHumanDetected = true;
  State.Environment.timeHumanDetected = getTime();
}

void onHumanLost() {
  State.Environment.isHumanDetected = false;
  State.Environment.timeHumanLost = getTime();
}


/*
 * Asynchronous thread process
 * ===========================
 */
void runEnvironmentThread() {
  if(environmentThread.shouldRun()) {
      environmentThread.run();
  }
}


/* 
 * Thread callback 
 * ---------------
 */
void onEnvironmentUpdate() {
  bool pirSensorValue = digitalRead(PIN_PIR_SENSOR);

  if (pirSensorValue) {
    onHumanDetected();
  }
}
