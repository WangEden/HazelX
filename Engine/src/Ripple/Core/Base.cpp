#include "hzpch.h"
#include "Base.h"

#include "Log.h"

#define RIPPLE_BUILD_ID "v0.1a"

namespace Ripple {

	void InitializeCore()
	{
		Ripple::Log::Init();

		RP_CORE_TRACE("Ripple Engine {}", RIPPLE_BUILD_ID);
		RP_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		RP_CORE_TRACE("Shutting down...");
	}
}
