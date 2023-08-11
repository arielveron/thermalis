/// Interface definition ///

class INASABrain {
  public:
    virtual bool isMotorOn() = 0;
    virtual bool update(float currentTemp) = 0;
    virtual String getMotorLastStartTime() = 0;
    virtual String getMotorLastEndTime() = 0;
    virtual String getBackendVersion() = 0;
    virtual String getmonitoringMode() = 0;
};


/// Derived class ///

class NASABrain : public INASABrain {
  private:
    ITimeSpanEngine* tsEngine;
    bool motorOn;
    bool actuate = false;
    monitoringMode backendMode;
    String motorLastStartTime;
    String motorLastEndTime;

  public:

    NASABrain(ITimeSpanEngine *ts, monitoringMode actuateMode) : backendMode(actuateMode), tsEngine(ts) {
      motorOn = false;
      if (backendMode == TERMONASA) {
        actuate = true;
      }
      if (backendMode == HELANASA) {
        actuate = false;
      }
    }

    bool isMotorOn() {
      return motorOn;
    }

    bool update(float currentTemp) {
      // a temperature less than -20 is a sensor failure
      if (currentTemp < -20) {
        motorOn = false;
        return motorOn;
      }

      bool newMotorStatus;
      bool mustSetMotor = false;

      if (currentTemp < tsEngine->getTempMin()) {
        newMotorStatus = actuate;
        mustSetMotor = true;
      }
      if (currentTemp > tsEngine->getTempMax()) {
        newMotorStatus = !actuate;
        mustSetMotor = true;
      } // if temp is low & if ts is currently forced & the motor is off, then start the motor
      else if (tsEngine->forcedTs() == tsEngine->getCurrentTimeSpanId() && motorOn != actuate) {
        newMotorStatus = actuate;
        mustSetMotor = true;
      }


      if (mustSetMotor) {
        // stores motor start & stop times
        if (motorOn == false and newMotorStatus == true) {
          motorLastStartTime = tsEngine->getCurrentTime();
          motorLastEndTime = "NULL";
        }
        if (motorOn == true and newMotorStatus == false) {
          motorLastEndTime = tsEngine->getCurrentTime();
          tsEngine->resetForcedTimeSpan();
        }

        motorOn = newMotorStatus;
      }

      return motorOn;
    }

    String getMotorLastStartTime() {
      return motorLastStartTime;
    }
    String getMotorLastEndTime() {
      return motorLastEndTime;
    }

    String getBackendVersion() {
      return backendVersion;
    }

    String getmonitoringMode() {
      if (backendMode == HELANASA) {
        return "HelaNASA";
      }
      if (backendMode == TERMONASA) {
        return "TermoNASA";
      }
    }
};
