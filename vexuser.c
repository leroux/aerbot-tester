#include <stdlib.h>

#include "ch.h"     // needs for all ChibiOS programs
#include "hal.h"    // hardware abstraction layer header
#include "vex.h"    // vex library header

static vexDigiCfg dConfig[kVexDigital_Num] = {
  { kVexDigital_1,    kVexSensorDigitalOutput, kVexConfigOutput,      0 },
  { kVexDigital_2,    kVexSensorDigitalOutput, kVexConfigOutput,      0 },
  { kVexDigital_3,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_4,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_5,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_6,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_7,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_8,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_9,    kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_10,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_11,   kVexSensorDigitalInput,  kVexConfigInput,       0 },
  { kVexDigital_12,   kVexSensorDigitalInput,  kVexConfigInput,       0 }
};

#define shooterArm kVexDigital_1
#define intakeArm kVexDigital_2

static vexMotorCfg mConfig[kVexMotorNum] = {
  { kVexMotor_1,      kVexMotorUndefined,      kVexMotorReversed,     kVexSensorNone,        0 },
  { kVexMotor_2,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_3,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_4,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_5,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_6,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_7,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_8,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_9,      kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 },
  { kVexMotor_10,     kVexMotorUndefined,      kVexMotorNormal,       kVexSensorNone,        0 }
};

#define shooterLeftMotor kVexMotor_3
#define shooterRightMotor kVexMotor_2
#define intakeMotor kVexMotor_4

void vexUserSetup() {
  vexDigitalConfigure(dConfig, DIG_CONFIG_SIZE(dConfig));
  vexMotorConfigure(mConfig, MOT_CONFIG_SIZE(mConfig));
}

void vexUserInit() {
  // ...
}

msg_t vexAutonomous(void *arg) {
  (void)arg;
  vexTaskRegister("auton");

  while (1) {
    vexSleep(25);
  }

  return (msg_t)0;
}

msg_t vexOperator(void *arg) {
  (void)arg;
  vexTaskRegister("operator");

  while (!chThdShouldTerminate())
    vexSleep(25);

  return (msg_t)0;
}

msg_t shooterTask(void *arg) {
  (void)arg;
  vexTaskRegister("shooter");

  int shooterArmToggled = 0;

  while (!chThdShouldTerminate()) {
    // shooter motor
    if (abs(vexControllerGet(Ch3)) > 15) {
      vexMotorSet(shooterLeftMotor, vexControllerGet(Ch3));
      vexMotorSet(shooterRightMotor, vexControllerGet(Ch3));
    } else {
      vexMotorSet(shooterLeftMotor, 0);
      vexMotorSet(shooterRightMotor, 0);
    }

    // toggle shooter pneumatic arm
    if(!shooterArmToggled && vexControllerGet(Btn6D)){
      vexDigitalPinSet(shooterArm, 1 - vexDigitalPinGet(shooterArm));
    }
    shooterArmToggled = vexControllerGet(Btn6D);

    vexSleep(25);
  }

  return (msg_t)0;
}

msg_t intakeTask(void *arg) {
  (void)arg;
  vexTaskRegister("intake");

  int intakeArmToggled = 0;

  while (!chThdShouldTerminate()) {
    // intake motor
    if (abs(vexControllerGet(Ch2)) > 15)
      vexMotorSet(intakeMotor, vexControllerGet(Ch2));
    else
      vexMotorSet(intakeMotor, 0);

    // toggle intake pneumatic
    if(!intakeArmToggled && vexControllerGet(Btn6U)){
      vexDigitalPinSet(intakeArm, 1 - vexDigitalPinGet(intakeArm));
    }
    intakeArmToggled = vexControllerGet(Btn6U);

    vexSleep(25);
  }

  return (msg_t)0;
}
