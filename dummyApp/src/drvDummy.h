#ifndef ASYN_DRV_DUMMY_H_
#define ASYN_DRV_DUMMY_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include <functional>
#include <map>

#include <iocsh.h>
#include <epicsExport.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsTimer.h>
#include <epicsTypes.h>
#include <asynPortDriver.h>

/* NOTE bind `epics pv` to this `parm string`
 * The `parm string` will bind to `param value` (which is mapped to device property 
 */
#define DummyEchoStr	"DUMMY_ECHO"	///< asynInt32, r/w echo trigger

using namespace std;

class DummyDriver : asynPortDriver {
	public:
		DummyDriver(const char *portName);
		virtual ~DummyDriver();
		/* NOTE define `param value` */
		struct Params {
			int echo;
		};
		struct Params params;
//		std::unique_ptr<struct Params> params = std::unique_ptr<struct Params>(new struct Params);
		static const size_t NUM_PARAMS; // need `static` to be initialized before constructor

		virtual asynStatus writeInt32(asynUser *, epicsInt32) override;

		template <typename T>
		using DummyDriverFuncPtr = int (DummyDriver::*)(asynUser *, T *);

		void registerHandler_Int32(int, DummyDriverFuncPtr<epicsInt32>);

	protected:
		template <typename T>
		using handler_t = std::function<int(asynUser *, T*)>;

		template <typename T>
		using handlerMap_t = std::map<int, handler_t<T>>;

		handlerMap_t<epicsInt32> handlerMap_Int;

		// handlers...
		int echoHandler(asynUser *, epicsInt32 *);

};

// XXX my implementation is different from AreaDetector

#endif
