#include "powerSave.h"
#include "settings.h"

/* Turn off the display */
void turnOffDisplay() { setBrightness(0, false); }

/* If the device screen is off, turn on, else just refresh sleep timer */
bool wakeUpScreen() {
    previousMillis = millis();
    if (isScreenOff) {
        isScreenOff = false;
        dimmer = false;
        getBrightness();
        return true;
    } else if (dimmer) {
        dimmer = false;
        getBrightness();
        return true;
    }
    return false;
}

/* Check if it's time to put the device to sleep */
void checkPowerSaveTime() {
    if (dimmerSet != 0) {
        if ((millis() - previousMillis) >= (dimmerSet * 1000) && dimmer == false && isSleeping == false) {
            dimmer = true;
            setBrightness(5, false);
        } else if ((millis() - previousMillis) >= ((dimmerSet * 1000) + 5000) && isScreenOff == false &&
                   isSleeping == false) {
            isScreenOff = true;
            turnOffDisplay();
        }
    }
}

/* Put device on sleep mode */
void sleepModeOn() {
    isSleeping = true;
    setCpuFrequencyMhz(80);
    turnOffDisplay();
    disableCore0WDT();
    disableCore1WDT();
    disableLoopWDT();
}

/* Wake up device */
void sleepModeOff() {
    isSleeping = false;
    setCpuFrequencyMhz(240);
    enableCore0WDT();
    enableCore1WDT();
    enableLoopWDT();
    feedLoopWDT();
    getBrightness();
}
