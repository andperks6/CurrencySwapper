# VR Address Verification Needed

This document lists the NEW hooks added in CurrencySwapper 2.0 that need VR address verification. These hooks did not exist in the previous VR 1.x port, so their offsets need to be verified for SkyrimVR.

## New Training Hooks Requiring Verification

### 1. CalculateTrainingCostHook::CalculateTrainingCost
**File:** `src/Hooks/Training/TrainingHooks.cpp:132`
**Current:** `REL::ID(51793), 0x87`
**Status:** ⚠️ **NEEDS VR VERIFICATION**

This hook intercepts the training cost calculation. In AE, this was at `REL::ID(52667), 0x87`.

**What to verify:**
- Confirm that the offset 0x87 is correct for VR in REL::ID 51793
- Test training menu to ensure costs are calculated correctly with custom currency

---

### 2. CalculateTrainingCostHook::CalculateTrainingCostTextUpdate
**File:** `src/Hooks/Training/TrainingHooks.cpp:143`
**Current:** `REL::ID(51794), 0x291`
**Status:** ⚠️ **NEEDS VR VERIFICATION**

This hook updates the displayed training cost text in the training menu. In AE, this was at `REL::ID(52668), 0x291`.

**What to verify:**
- Confirm that the offset 0x291 is correct for VR in REL::ID 51794
- Test training menu UI to ensure the cost text updates properly with custom currency

---

### 3. SendNotEnoughGoldMessageHook::SendNotEnoughGoldMessage
**File:** `src/Hooks/Training/TrainingHooks.cpp:167`
**Current:** `REL::ID(51793), 0x1BB`
**Status:** ⚠️ **NEEDS VR VERIFICATION**

This hook intercepts the "not enough gold" message during training. In AE, this was at `REL::ID(52667), 0x1BB`.

**What to verify:**
- Confirm that the offset 0x1BB is correct for VR in REL::ID 51793
- Test training without sufficient custom currency to verify the message displays correctly

---

## How to Verify

1. **Build and test the plugin** - The pattern validation (`REL::make_pattern<"E8">().match()`) will fail if the offset is completely wrong
2. **Check logs** - If pattern validation fails, the log will show which hook failed
3. **Use a disassembler/debugger** - Compare the VR binary to find the correct offsets if needed
4. **Functional testing:**
   - Set a custom currency using the mod
   - Open training menu with a trainer
   - Verify:
     - Training cost displays correctly
     - Training cost text updates when switching skills
     - "Not enough [currency name]" message appears when insufficient funds
     - Training completes successfully when you have enough custom currency

## REL::ID Mappings (AE → VR)

For reference, here are the verified REL::ID mappings:

| Function | AE ID | VR ID | Notes |
|----------|-------|-------|-------|
| **Barter Hooks** | | | |
| GetPlayerGold | 50957 | 50013 | ✅ Verified |
| GetVendorGold | 50957 | 50013 | ✅ Verified |
| GetGoldFromSale | 50951 | 50007 | ✅ Verified |
| GetGoldFromPurchase | 50951 | 50007 | ✅ Verified |
| RawDeal | 50952 | 50008 | ✅ Verified |
| RejectedDeal | 50951 | 50007 | ✅ Verified |
| RecalcVendorGold | 50957 | 50013 | ✅ Verified |
| ShowBarterMenu | 50955 | 50011 | ✅ Verified |
| **Training Hooks** | | | |
| SetupTrainingMenu | 52666 | 51792 | ✅ Verified |
| GetPlayerGold (Training) | 52667 | 51793 | ✅ Verified |
| CalculateTrainingCost | 52667 | 51793 | ⚠️ **Offset 0x87 needs verification** |
| CalculateTrainingCostTextUpdate | 52668 | 51794 | ⚠️ **Offset 0x291 needs verification** |
| RemovePlayerGold | 52667 | 51793 | ✅ Verified |
| SendNotEnoughGoldMessage | 52667 | 51793 | ⚠️ **Offset 0x1BB needs verification** |
| UpdateTrainingCurrency | 52668 | 51794 | ✅ Verified |
| **Offsets** | | | |
| UpdateBottomBar | 50957 | 50013 | ✅ Verified |
| MoveItemBetweenContainers | 16059 | 15821 | ✅ Verified |
| RemoveItem | 51636 | 50741 | ✅ Verified |

---

**Last Updated:** 2025-12-31
**Branch:** vr-2.0
