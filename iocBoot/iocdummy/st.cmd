#!../../bin/linux-x86_64/dummy

## You may have to change dummy to something else
## everywhere it appears in this file

< envPaths
epicsEnvSet("DEVICE", "DUMMY")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/dummy.dbd"
dummy_registerRecordDeviceDriver pdbbase

## load drivers, param: portName
DummyDriverConfigure("$(DEVICE)")

## Load record instances
#dbLoadRecords("db/xxx.db","user=jerryjiaHost")
dbLoadRecords("db/dummy.db","P=$(DEVICE),PORT=$(DEVICE),ADDR=0,TIMEOUT=0")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=jerryjiaHost"
