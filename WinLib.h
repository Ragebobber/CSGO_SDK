#pragma once
//win
#include <Windows.h>
#include <d3d9.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <fstream>
#include <Shlobj.h>
#include <unordered_map>
//boost
#include <boost/thread.hpp>
#include <boost/range/irange.hpp>
#include <boost/math/special_functions/round.hpp>
//d3d9
#include <DirectXMath.h>
//hook
#include <detours.h>
//imgui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"
//xor
#include <cstdint>
#include <cstddef>
#include <utility>
#include <type_traits>