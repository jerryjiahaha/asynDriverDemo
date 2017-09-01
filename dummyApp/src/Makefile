TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = dummy
# dummy.dbd will be created and installed
DBD += dummy.dbd

# dummy.dbd will be made up from these files:
dummy_DBD += base.dbd
dummy_DBD += asyn.dbd

# Include dbd files from all support applications:
#dummy_DBD += xxx.dbd
dummy_DBD += drvDummy.dbd

# Add all the support libraries needed by this IOC
#dummy_LIBS += xxx
dummy_LIBS += asyn

# dummy_registerRecordDeviceDriver.cpp derives from dummy.dbd
dummy_SRCS += dummy_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
dummy_SRCS_DEFAULT += dummyMain.cpp
dummy_SRCS_vxWorks += -nil-

dummy_SRCS_DEFAULT += drvDummy.cpp

# Add support from base/src/vxWorks if needed
#dummy_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
dummy_LIBS += $(EPICS_BASE_IOC_LIBS)

USR_CPPFLAGS += -std=c++14 -Wall

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

