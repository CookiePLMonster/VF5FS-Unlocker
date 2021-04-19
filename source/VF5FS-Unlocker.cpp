#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "Utils/MemoryMgr.h"
#include "Utils/Patterns.h"

static bool is_disable_select_pause(void*)
{
	return true;
}

void OnInitializeHook()
{
	using namespace Memory::VP;
	using namespace hook;

	// Set the VF5 arcade to console mode (mode 0)
	{
		auto module_start_y6 = pattern("C6 44 24 ? 01 B8 ? ? ? ? 66 89 44 24 ? 33 ED");
		if (module_start_y6.count_hint(1).size() == 1)
		{
			Patch<uint8_t>(module_start_y6.get_first(4), 0);
		}
	}

	// Disable pause under the Select button
	{
		auto cscene_vf5fs_vtable_v6 = pattern("90 48 8D 05 ? ? ? ? 48 89 06 45 33 E4 4C 89 A6 ? ? ? ? 4C 89 A6 ? ? ? ? 48 8D BE");
		if (cscene_vf5fs_vtable_v6.count_hint(1).size() == 1)
		{
			// Read the pointer to the vtable and replace the is_disable_select_pause method
			void** vtable;
			ReadOffsetValue(cscene_vf5fs_vtable_v6.get_first(1 + 3), vtable);
			Patch(&vtable[79], &is_disable_select_pause);
		}
	}
}