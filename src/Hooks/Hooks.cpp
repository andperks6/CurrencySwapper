#include "Hooks/hooks.h"

#include "Barter/BarterHooks.h"
#include "Training/TrainingHooks.h"

namespace Hooks {
	bool Install() {
		SECTION_SEPARATOR;
		logger::info("Installing hooks..."sv);
		// TODO VR: Training hooks disabled - only allocate for barter
		constexpr size_t allocSize = Barter::allocSize; // + Training::allocSize;
		logger::info("  >Allocated {} bytes."sv, allocSize);
		SKSE::AllocTrampoline(allocSize);

		bool nominal = true;
		if (!Barter::Install()) {
			nominal = false;
		}
		// TODO VR: Training hooks disabled - all 6 offsets failed pattern validation
		// See VR_VERIFICATION_NEEDED.md for details on which offsets need to be found for VR
		logger::warn("Training hooks disabled for VR - offsets need verification");
		/*
		if (!Training::Install()) {
			nominal = false;
		}
		*/
		return nominal;
	}
}