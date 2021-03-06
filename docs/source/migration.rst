.. _migration:

Migrating from tpmac, pmacCoord and pmacUtil
============================================

The pmac module has combined the existing tpmac, pmacUtil and pmacCoord modules into a single module.  This document describes the steps required to convert an existing IOC to make use of the new pmac module.
This guide does not describe any of the new features of the pmac module.  These features are described in the :ref:`user_guide`.

Setup
-----

This guide assumes an IOC setup with the following:

* Delta Tau geobrick with 8 motors.
* Single coordinate system with kinematics.

configure/RELEASE
-----------------

Using tpmac

::

  ASYN         = $(SUPPORT)/asyn
  BUSY         = $(SUPPORT)/busy
  GENSUB       = $(SUPPORT)/genSub
  MOTOR        = $(SUPPORT)/motor
  PMACUTIL     = $(SUPPORT)/pmacUtil
  SEQ          = $(SUPPORT)/seq
  STREAMDEVICE = $(SUPPORT)/streamDevice
  TPMAC        = $(SUPPORT)/tpmac

Using pmac

::

  ASYN         = $(SUPPORT)/asyn
  BUSY         = $(SUPPORT)/busy
  CALC         = $(SUPPORT)/calc
  MOTOR        = $(SUPPORT)/motor
  PMAC         = $(SUPPORT)/pmac

Note that tpmac, pmacUtil and pmacCoord are no longer required.  Calc is required by the pmac module.


src/Makefile
------------

Using tpmac

::

  tpmac_DBD += base.dbd
  tpmac_DBD += asyn.dbd
  tpmac_DBD += pmacAsynIPPort.dbd
  tpmac_DBD += motorSupport.dbd
  tpmac_DBD += devSoftMotor.dbd
  tpmac_DBD += pmacAsynMotor.dbd
  tpmac_DBD += stream.dbd
  tpmac_DBD += genSubRecord.dbd
  tpmac_DBD += pmacUtilSupport.dbd
  tpmac_DBD += pmacAsynCoord.dbd
  tpmac_DBD += busySupport.dbd
  tpmac_SRCS += tpmac_registerRecordDeviceDriver.cpp
  tpmac_LIBS += busy
  tpmac_LIBS += pmacAsynCoord
  tpmac_LIBS += pmacUtil
  tpmac_LIBS += seq
  tpmac_LIBS += pv
  tpmac_LIBS += genSub
  tpmac_LIBS += stream
  tpmac_LIBS += pcre
  tpmac_LIBS += pmacAsynMotor
  tpmac_LIBS += softMotor
  tpmac_LIBS += motor
  tpmac_LIBS += pmacAsynIPPort
  tpmac_LIBS += asyn
  tpmac_LIBS += $(EPICS_BASE_IOC_LIBS)
  
Using pmac

::

  pmac_DBD += base.dbd
  pmac_DBD += asyn.dbd
  pmac_DBD += pmacAsynIPPort.dbd
  pmac_DBD += motorSupport.dbd
  pmac_DBD += devSoftMotor.dbd
  pmac_DBD += pmacAsynMotorPort.dbd
  pmac_DBD += busySupport.dbd
  pmac_DBD += calcSupport.dbd
  pmac_SRCS += pmac_registerRecordDeviceDriver.cpp
  pmac_LIBS += calc
  pmac_LIBS += busy
  pmac_LIBS += pmacAsynMotorPort
  pmac_LIBS += softMotor
  pmac_LIBS += motor
  pmac_LIBS += pmacAsynIPPort
  pmac_LIBS += asyn
  pmac_LIBS += $(EPICS_BASE_IOC_LIBS)

The pmac module no longer requires (dbd or lib):

* stream
* gensub
* pmacUtil
* pmacCoord

Note pmacAsynMotor has been replaced with pmacAsynMotorPort.

Db/substitutions
----------------

Using tpmac for status records

::

  # Macros:
  #  DEVICE   Pmac/Geobrick name
  #  VERSION  0 for Pmac, 1 for Geobrick
  #  PLC      PLC for CPU load monitoring, e.g. 5
  #  PORT     Delta tau motor controller comms port
  #  NAXES    Number of axes
  #  name     Object and gui association name
  #  DESC     Description of pmac use
  #  MOIOC    The motion IOC number controlling this brick
  #  CTLIP    The IP address to use for PMAC control
  #  CTLPORT  The port number to use for PMAC control
  #  CTLMODE  The mode to use for PMAC control, 'ts' for terminal server, 'tcpip' for ethernet
  file $(PMACUTIL)/db/pmacStatus.template
  {
  pattern { DEVICE, VERSION, PLC, PORT, NAXES, name, DESC, MOIOC, CTLIP, CTLPORT, CTLMODE }
    { "PMAC_TEST", "1", "5", "BRICK1port", "8", "BRICK1.STAT", "", "", "", "", "" }
  }
  
  # Macros:
  #  DEVICE  Pmac/Geobrick name
  #  AXIS    Axis number
  #  PORT    Asyn port
  file $(PMACUTIL)/db/pmacStatusAxis.template
  {
  pattern { DEVICE, AXIS, PORT }
    { "PMAC_TEST", "1", "BRICK1port" }
    { "PMAC_TEST", "2", "BRICK1port" }
    { "PMAC_TEST", "3", "BRICK1port" }
    { "PMAC_TEST", "4", "BRICK1port" }
    { "PMAC_TEST", "5", "BRICK1port" }
    { "PMAC_TEST", "6", "BRICK1port" }
    { "PMAC_TEST", "7", "BRICK1port" }
    { "PMAC_TEST", "8", "BRICK1port" }
  }

Using pmac for status records

::

  # Macros:
  #  DEVICE    Pmac/Geobrick name
  #  PLC       PLC for CPU load monitoring, e.g. 5
  #  PORT      Delta tau motor controller
  #  NAXES     Number of axes
  #  name      Object and gui association name
  #  TIMEOUT   Template argument
  #  FEEDRATE  Template argument
  file $(PMAC)/db/pmacStatus.template
  {
  pattern { DEVICE, PLC, PORT, NAXES, name, TIMEOUT, FEEDRATE }
    { "PMAC_TEST", "5", "BRICK1", "8", "BRICK1.STAT", "4", "100" }
  }

  # Macros:
  #  DEVICE  Pmac/Geobrick name
  #  AXIS    Axis number
  #  PORT    Asyn port
  file $(PMAC)/db/pmacStatusAxis.template
  {
  pattern { DEVICE, AXIS, PORT }
    { "PMAC_TEST", "1", "BRICK1" }
    { "PMAC_TEST", "2", "BRICK1" }
    { "PMAC_TEST", "3", "BRICK1" }
    { "PMAC_TEST", "4", "BRICK1" }
    { "PMAC_TEST", "5", "BRICK1" }
    { "PMAC_TEST", "6", "BRICK1" }
    { "PMAC_TEST", "7", "BRICK1" }
    { "PMAC_TEST", "8", "BRICK1" }
  }

Note that for tpmac the PORT macro is set to the low level asyn driver port name, but for pmac it is now set to the controller port name.  All communications in the pmac driver are handled through the controller classes.

Using tpmac for motor records

::

  # Macros:
  #  P           Device Prefix
  #  M           Device Suffix
  #  PORT        Delta tau motor CS
  #  ADDR        Address on controller
  #  DESC        Description, displayed on EDM screen
  #  MRES        Motor Step Size (EGU)
  #  VELO        Velocity (EGU/s)
  #  PREC        Display Precision
  #  EGU         Engineering Units
  #  TWV         Tweak Step Size (EGU)
  #  DTYP        DTYP of record
  #  DIR         User Direction
  #  VBAS        Base Velocity (EGU/s)
  #  VMAX        Max Velocity (EGU/s), defaults to VELO
  #  ACCL        Seconds to Velocity
  #  BDST        BL Distance (EGU)
  #  BVEL        BL Velocity (EGU/s)
  #  BACC        BL Seconds to Veloc.
  #  DHLM        Dial High Limit
  #  DLLM        Dial Low Limit
  #  HLM         User High Limit
  #  LLM         User Low Limit
  #  HLSV        HW Lim. Violation Svr
  #  INIT        Startup commands
  #  SREV        Steps per Revolution
  #  RRES        Readback Step Size (EGU)
  #  ERES        Encoder Step Size (EGU)
  #  JAR         Jog Acceleration (EGU/s^2)
  #  UEIP        Use Encoder If Present
  #  URIP        Use RDBL If Present
  #  RDBL        Readback Location, set URIP = 1 if you specify this
  #  RTRY        Max retry count
  #  DLY         Readback settle time (s)
  #  OFF         User Offset (EGU)
  #  RDBD        Retry Deadband (EGU)
  #  FOFF        Freeze Offset, 0=variable, 1=frozen
  #  ADEL        Alarm monitor deadband (EGU)
  #  NTM         New Target Monitor, only set to 0 for soft motors
  #  FEHIGH      HIGH limit for following error
  #  FEHIHI      HIHI limit for following error
  #  FEHHSV      HIHI alarm severity for following error
  #  FEHSV       HIGH alarm severity for following error
  #  SCALE       Scale factor, if pmacSetAxisScale is used this should be set
  #  HOMEVIS     If 1 then home is visible on the gui
  #  HOMEVISSTR  If HOMEVIS=0, then display this text on the gui instead
  #  name        Object name and gui association name
  #  alh         Set this to alh to add the motor to the alarm handler and send emails, 
  #  gda_name    Name to export this as to GDA
  #  gda_desc    Description to export this as to GDA
  #  HOME        Prefix for autohome instance. Defaults to $(P). If unspecified,
  file $(PMACUTIL)/db/dls_pmac_cs_asyn_motor.template
  {
  pattern { P, M, PORT, ADDR, DESC, MRES, VELO, PREC, EGU, TWV, DTYP, DIR, VBAS, VMAX, ACCL, BDST, BVEL, BACC, DHLM, DLLM, HLM, LLM, HLSV, INIT, SREV, RRES, ERES, JAR, UEIP, URIP, RDBL, RTRY, DLY, OFF, RDBD, FOFF, ADEL, NTM, FEHIGH, FEHIHI, FEHHSV, FEHSV, SCALE, HOMEVIS, HOMEVISSTR, name, alh, gda_name, gda_desc, HOME }
    { "PMAC_TEST", ":M1", "BRICK1", "1", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M1", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M2", "BRICK1", "2", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M2", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M3", "BRICK1", "3", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M3", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M4", "BRICK1", "4", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M4", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M5", "BRICK1", "5", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M5", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M6", "BRICK1", "6", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M6", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M7", "BRICK1", "7", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M7", "None", "", "$(DESC)", "$(P)" }
    { "PMAC_TEST", ":M8", "BRICK1", "8", "Test motor", "0.001", "1", "3", "mm", "0.0", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "", "", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M8", "None", "", "$(DESC)", "$(P)" }
  }
  
Using pmac for motor records

::

  # Macros:
  #  P                Device Prefix
  #  M                Device Suffix
  #  PORT             Delta tau motor controller
  #  ADDR             Address on controller
  #  DESC             Description, displayed on EDM screen
  #  MRES             Motor Step Size (EGU)
  #  VELO             Velocity (EGU/s)
  #  PREC             Display Precision
  #  EGU              Engineering Units
  #  TWV              Tweak Step Size (EGU)
  #  DTYP             DTYP of record
  #  DIR              User Direction
  #  VBAS             Base Velocity (EGU/s)
  #  VMAX             Max Velocity (EGU/s), defaults to VELO
  #  ACCL             Seconds to Velocity
  #  BDST             BL Distance (EGU)
  #  BVEL             BL Velocity (EGU/s)
  #  BACC             BL Seconds to Veloc.
  #  DHLM             Dial High Limit
  #  DLLM             Dial Low Limit
  #  HLM              User High Limit
  #  LLM              User Low Limit
  #  HLSV             HW Lim. Violation Svr
  #  INIT             Startup commands
  #  SREV             Steps per Revolution
  #  RRES             Readback Step Size (EGU)
  #  ERES             Encoder Step Size (EGU)
  #  JAR              Jog Acceleration (EGU/s^2)
  #  UEIP             Use Encoder If Present
  #  URIP             Use RDBL If Present
  #  RDBL             Readback Location, set URIP = 1 if you specify this
  #  RTRY             Max retry count
  #  DLY              Readback settle time (s)
  #  OFF              User Offset (EGU)
  #  RDBD             Retry Deadband (EGU)
  #  FOFF             Freeze Offset, 0=variable, 1=frozen
  #  ADEL             Alarm monitor deadband (EGU)
  #  NTM              New Target Monitor, only set to 0 for soft motors
  #  FEHIGH           HIGH limit for following error
  #  FEHIHI           HIHI limit for following error
  #  FEHHSV           HIHI alarm severity for following error
  #  FEHSV            HIGH alarm severity for following error
  #  SCALE            Scale factor, if pmacSetAxisScale is used this should be set
  #  HOMEVIS          If 1 then home is visible on the gui
  #  HOMEVISSTR       If HOMEVIS=0, then display this text on the gui instead
  #  name             Object name and gui association name
  #  alh              Set this to alh to add the motor to the alarm handler and send emails, 
  #  gda_name         Name to export this as to GDA
  #  gda_desc         Description to export this as to GDA
  #  SPORT            Delta tau motor controller comms port
  #  HOME             Prefix for autohome instance. Defaults to $(P). If unspecified,
  #  PMAC             Prefix for pmacStatus instance. Needed to get axis descriptions
  #  ALLOW_HOMED_SET  Set to a blank to allow this axis to have its homed
  file $(PMAC)/db/dls_pmac_asyn_motor.template
  {
  pattern { P, M, PORT, ADDR, DESC, MRES, VELO, PREC, EGU, TWV, DTYP, DIR, VBAS, VMAX, ACCL, BDST, BVEL, BACC, DHLM, DLLM, HLM, LLM, HLSV, INIT, SREV, RRES, ERES, JAR, UEIP, URIP, RDBL, RTRY, DLY, OFF, RDBD, FOFF, ADEL, NTM, FEHIGH, FEHIHI, FEHHSV, FEHSV, SCALE, HOMEVIS, HOMEVISSTR, name, alh, gda_name, gda_desc, SPORT, HOME, PMAC, ALLOW_HOMED_SET }
    { "PMAC_TEST", ":M1", "BRICK1", "1", "Motor 1", "0.001", "20", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M1", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M2", "BRICK1", "2", "Motor 2", "0.001", "20", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M2", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M3", "BRICK1", "3", "Motor 3", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M3", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M4", "BRICK1", "4", "Motor 4", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M4", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M5", "BRICK1", "5", "Motor 5", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M5", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M6", "BRICK1", "6", "Motor 6", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M6", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M7", "BRICK1", "7", "Motor 7", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M7", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
    { "PMAC_TEST", ":M8", "BRICK1", "8", "Motor 8", "0.001", "1", "3", "mm", "1", "asynMotor", "0", "0", "$(VELO)", "0.5", "0", "0", "", "1000", "-1000", "", "", "MAJOR", "", "1000", "", "", "", "0", "0", "", "0", "0", "0", "", "0", "0", "1", "0", "0", "NO_ALARM", "NO_ALARM", "1", "1", "Use motor summary screen", "BRICK1.MOTORS.M8", "None", "", "$(DESC)", "BRICK1port", "$(P)", "$(P)", "#" }
  }

Note for the pmac module the addition of SPORT and PMAC macros.

Startup Script
--------------

Using tpmac

::

  # Create IP Port (PortName, IPAddr)
  pmacAsynIPConfigure("BRICK1port", "172.23.253.11:1025")
  
  # Create asyn motor port (AsynPort, Addr, BrickNum, NAxes)
  pmacAsynMotorCreate("BRICK1port", 0, 0, 8)
  # Configure GeoBrick (MotorPort, DriverName, BrickNum, NAxes+1)
  drvAsynMotorConfigure("BRICK1", "pmacAsynMotor", 0, 9)
  pmacSetIdlePollPeriod(0, 1000)
  pmacSetMovingPollPeriod(0, 100)
  
  # Configure StreamDevice paths
  epicsEnvSet "STREAM_PROTOCOL_PATH", "/dls_sw/prod/R3.14.12.3/support/pmacCoord/1-41/data:/dls_sw/prod/R3.14.12.3/support/pmacUtil/4-36/data"
  
  # Create CS (ControllerPort, Addr, CSNumber, CSRef, Prog)
  pmacAsynCoordCreate("BRICK1port", 0, 2, 0, 10)
  # Configure CS (PortName, DriverName, CSRef, NAxes)
  drvAsynMotorConfigure("BRICK1CS2", "pmacAsynCoord", 0, 9)
  # Set Idle and Moving poll periods (CS_Ref, PeriodMilliSeconds)
  pmacSetCoordIdlePollPeriod(0, 500)
  pmacSetCoordMovingPollPeriod(0, 100)

Using pmac

::

  # Create IP Port (PortName, IPAddr)
  pmacAsynIPConfigure("BRICK1port", "172.23.253.11:1025")
  
  # Configure Model 3 Controller Driver (Controler Port,Asyn Motor Port, ADDR, Axes, MOVE_POLL, IDLE_POLL)
  pmacCreateController("BRICK1", "BRICK1port", 0, 8, 100, 1000)
  # Configure Model 3 Axes Driver (Controler Port, Axis Count)
  pmacCreateAxes("BRICK1", 8)
  
  # Create CS (CS Port, Controller Port, CSNumber, Prog)
  pmacCreateCS("BRICK1CS2", "BRICK1", 2, 10)
  # Configure Model 3 CS Axes Driver (Controller Port, Axis Count)
  pmacCreateCSAxes("BRICK1CS2", 9)

There are some differences in these boot files, they are listed below:

* pmacAsynMotorCreate and drvAsynMotorConfigure have been replaced with pmacCreateController and pmacCreateAxes.
* pmacCreateController accepts the polling rates as inputs, so there is no requirement to call pmacSetIdlePollPeriod or pmacSetMovingPollPeriod.
* There is no need to set STREAM_PROTOCOL_PATH for the pmac module, all communications occur through the controller.
* pmacAsynCoordCreate and drvAsynMotorConfigure have been replace with pmacCreateCS and pmacCreateCSAxes.
* Polling is controlled by the motor controller and so there are no pmacSetCoordIdlePollPeriod or pmacSetCoordMovingPollPeriod calls.
