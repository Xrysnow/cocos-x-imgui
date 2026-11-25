#pragma once
#include "imgui/imgui.h"

IMGUI_IMPL_API bool     ImGui_ImplCC_Init(bool install_callbacks);
IMGUI_IMPL_API void     ImGui_ImplCC_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplCC_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplCC_SetGamepadPlayer(unsigned player);
IMGUI_IMPL_API void     ImGui_ImplCC_RenderDrawData(ImDrawData* draw_data);
IMGUI_IMPL_API void     ImGui_ImplCC_RenderPlatform();

// Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool     ImGui_ImplCC_CreateDeviceObjects();
IMGUI_IMPL_API bool     ImGui_ImplCC_EnsureSdfProgram();
IMGUI_IMPL_API void     ImGui_ImplCC_DestroyDeviceObjects();
