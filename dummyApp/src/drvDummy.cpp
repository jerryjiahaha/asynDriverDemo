#include <exception>
#include "drvDummy.h"

#define _ASYN_METHOD_ENTRY_ \
	const char *paramName; \
	const char *functionName = __func__; \
	int function = pasynUser->reason; \
	asynStatus status = asynSuccess; \
	status = getParamName(function, &paramName); \
	if ( status != asynSuccess ) { \
		std::cerr << "function not found" << std::endl; \
		return status; \
	}

const size_t DummyDriver::NUM_PARAMS = sizeof(struct DummyDriver::Params)/sizeof(int);
static const char driverName[] = "DummyDriver";

DummyDriver::DummyDriver(const char *portName) : asynPortDriver(
		portName, // asyn port name
		1, // number of sub-address
		NUM_PARAMS, // number of params
		asynInt32Mask | asynFloat64Mask | asynEnumMask | asynDrvUserMask, // interface mask
		asynInt32Mask | asynFloat64Mask | asynEnumMask , // interrupt mask
		0, // asynFlags, non-block and no multi devices
		1, // autoConnect
		0, // priority
		0 // stack size
		) {
	std::cout << "num of params: " << NUM_PARAMS << std::endl;

	// Init asynDriver `param value`, bind `param value` to `param string`
	createParam(DummyEchoStr, asynParamInt32, &(params.echo));

	// Register handlers after params initialized
	registerHandler_Int32(params.echo, &DummyDriver::echoHandler);
}

DummyDriver::~DummyDriver() {}

void DummyDriver::registerHandler_Int32(int func_index, DummyDriverFuncPtr<epicsInt32> func) {
	handlerMap_Int[func_index] = [this, func](asynUser *p, epicsInt32 *v){
		return (this->*func)(p, v);
	};
}

int DummyDriver::echoHandler(asynUser *pasynUser, epicsInt32 *value) {
	std::cout << __func__ << std::endl;
	return 0;
}

asynStatus DummyDriver::writeInt32(asynUser *pasynUser, epicsInt32 value) {
	_ASYN_METHOD_ENTRY_;

	std::cout << "writeInt32 called, value: " << value << ", function: " << function << std::endl;

	int feedback = 0;
	try {
		auto handler = handlerMap_Int.at(function);
		feedback = handler(pasynUser, &value);
	}
	catch (const std::out_of_range &err) {
		// just set value
	}

	if (feedback) {
		// error handling, or will not setIntegerParam
	}

	// set value to
	status = setIntegerParam(function, value);

	if ( status == asynSuccess ) {
		status = (asynStatus) callParamCallbacks();
	}

	if (status) {
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s:%s: status=%d, function=%d, name=%s, value=%d",
				driverName, functionName, status, function, paramName, value);
	}
	else {
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
				"%s:%s: function=%d, name=%s, value=%d\n",
				driverName, functionName, function, paramName, value);
	}

	return status;

}

extern "C" {

// for ioc shell calling
int DummyDriverConfigure(const char* portName) {
	new DummyDriver(portName);
	return asynSuccess;
}

// epics ioc shell
static const iocshArg initArg0 = { "portName", iocshArgString };
static const iocshArg * const initArgs[] = {&initArg0};
static const iocshFuncDef initFuncDef = {"DummyDriverConfigure", 1, initArgs};
static void initCallFunc(const iocshArgBuf *args) {
	DummyDriverConfigure(args[0].sval);
}
void DummyDriverRegister(void) {
	iocshRegister(&initFuncDef, initCallFunc);
}
epicsExportRegistrar(DummyDriverRegister);

}
