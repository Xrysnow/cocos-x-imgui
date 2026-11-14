-- Main

---
---@param show boolean
function imgui.showStyleEditor(show)
end

-- Demo, Debug, Information

--- create Demo window (previously called ShowTestWindow). demonstrate most ImGui features.
--- call this to learn about the library! try to make it always available in your application!
function imgui.showDemoWindow(open)
end
--- create About window. display Dear ImGui version, credits and build/system information.
function imgui.showAboutWindow(open)
end
--- create Metrics/Debugger window. display Dear ImGui internals: windows, draw commands, various internal state, etc.
function imgui.showMetricsWindow(open)
end


-- Styles

--- new, recommended style (default)
function imgui.styleColorsDark()
end

--- classic imgui style
function imgui.styleColorsClassic()
end

--- best used with borders and a custom, thicker font
function imgui.styleColorsLight()
end

--- Windows
--- - Begin() = push window to the stack and start appending to it. End() = pop window from the stack.
--- - You may append multiple times to the same window during the same frame.
--- - Passing 'bool* p_open != NULL' shows a window-closing widget in the upper-right corner of the window,
---   which clicking will set the boolean to false when clicked.
--- - Begin() return false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
---   anything to the window. Always call a matching End() for each Begin() call, regardless of its return value!
---   (this is due to legacy reason and is inconsistent with most other functions such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc.
---    where the EndXXX call should only be called if the corresponding BeginXXX function returned true.)
--- - Note that the bottom of window stack always contains a window called "Debug".
---@param name string
---@param open boolean optional, nullable
---@param flags number optional ImGuiWindowFlags
---@return boolean,boolean|nil
function imgui.begin(name, open, flags)
end

function imgui.endToLua()
end

--- Child Windows
--- - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
--- - For each independent axis of 'size': ==0.0f: use remaining host window size / >0.0f: fixed size / <0.0f: use remaining window size minus abs(size) / Each axis can use a different mode, e.g. ImVec2(0,400).
--- - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting anything to the window.
---   Always call a matching EndChild() for each BeginChild() call, regardless of its return value (this is due to legacy reason and is inconsistent with most other functions such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function returned true.)
---   [Important: due to legacy reason, this is inconsistent with most other functions such as BeginMenu/EndMenu,
---   BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function
---   returned true. Begin and BeginChild are the only odd ones out. Will be fixed in a future update.]
---@param id string
---@param size ImVec2 optional
---@param border number optional
---@param flags number optional ImGuiWindowFlags
---@return boolean
function imgui.beginChild(id, size, border, flags)
end

function imgui.endChild()
end

--
-- Parameters stacks (shared)
--

--- A nil font keeps the current font. Explicit size is unscaled; global font scales apply afterward.
--- Size 0 keeps the current size. When size is omitted, use font.LegacySize or keep the current size for nil.
---@param font imgui.ImFont|nil
---@param font_size_base_unscaled number
---@overload fun(font:imgui.ImFont|nil)
---@overload fun()
function imgui.pushFont(font, font_size_base_unscaled)
end

--- get current font
---@return imgui.ImFont
function imgui.getFont()
end

---
---@param idx number ImGuiCol
---@param color ImVec4|number
function imgui.pushStyleColor(idx, color)
end

---
---@param idx number ImGuiStyleVar
---@param value number|ImVec2
function imgui.pushStyleVar(idx, value)
end

--
-- Widgets
--

---
---@param tex cc.Texture2D
---@param size ImVec2
---@param uv0 ImVec2
---@param uv1 ImVec2
---@param tint_color ImVec4
---@param border_color ImVec4
---@overload fun(sp:cc.Sprite,size:ImVec2,tint_color:ImVec4,border_color:ImVec4)
function imgui.image(tex, size, uv0, uv1, tint_color, border_color)
end

---
---@param tex cc.Texture2D
---@param size ImVec2? defaults to texture size
---@param uv0 ImVec2? defaults to (0, 0)
---@param uv1 ImVec2? defaults to (1, 1)
---@param bg_color ImVec4? defaults to (0, 0, 0, 0)
---@param tint_color ImVec4? defaults to (1, 1, 1, 1)
---@overload fun(sp:cc.Sprite,size:ImVec2?,bg_color:ImVec4?,tint_color:ImVec4?)
function imgui.imageWithBg(tex, size, uv0, uv1, bg_color, tint_color)
end

---
---@param tex cc.Texture2D
---@param size ImVec2
---@param uv0 ImVec2
---@param uv1 ImVec2
---@param frame_padding number if <0, uses default frame padding settings. 0 for no padding
---@param bg_color ImVec4
---@param tint_color ImVec4
---@return boolean
---@overload fun(sp:cc.Sprite,size:ImVec2,frame_padding:number,bg_color:ImVec4,tint_color:ImVec4):boolean
function imgui.imageButton(tex, size, uv0, uv1, frame_padding, bg_color, tint_color)
end

---
---@param label string
---@param v boolean
---@return boolean,boolean pressed, v_new
function imgui.checkbox(label, v)
end

---
---@param label string
---@param flags number
---@param flags_value number
---@return boolean,number
function imgui.checkboxFlags(label, flags, flags_value)
end

--- - Two args: use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
--- - Three args: shortcut to handle the above pattern when value is an integer
---@param label string
---@param v number
---@param v_button number
---@return boolean,number
---@overload fun(label:string,active:boolean):boolean
function imgui.radioButton(label, v, v_button)
end

---combo
---@param label string
---@param current_item number
---@param items string[]
---@param popup_max_height_in_items number
---@return boolean,number
function imgui.combo(label, current_item, items, popup_max_height_in_items)
end


--
-- Widgets: Drag Sliders
-- - CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
-- - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
-- - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
-- - Format string may also be set to NULL or use the default format ("%f" or "%d").
-- - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
-- - Use v_min < v_max to clamp edits to given limits. Note that CTRL+Click manual input can override those limits.
-- - Use v_max = FLT_MAX / INT_MAX etc to avoid clamping to a maximum, same with v_min = -FLT_MAX / INT_MIN to avoid clamping to a minimum.
-- - We use the same sets of flags for DragXXX() and SliderXXX() functions as the features are the same and it makes it easier to swap them.
-- - Legacy: Pre-1.78 there are DragXXX() function signatures that takes a final `float power=1.0f' argument instead of the `ImGuiSliderFlags flags=0' argument.
--   If you get a warning converting a float to ImGuiSliderFlags, read https://github.com/ocornut/imgui/issues/3361
--

---
---@param label string
---@param value number
---@param v_speed number @optional
---@param v_min number @optional
---@param v_max number @optional
---@param format string @optional
---@param flags number @optional, ImGuiSliderFlags
---@return boolean,number
function imgui.dragFloat(label, value, v_speed, v_min, v_max, format, flags)
end

---
---@param label string
---@param array number[]
---@param v_speed number
---@param v_min number
---@param v_max number
---@param format string
---@param flags number
---@return boolean,number[]
function imgui.dragFloatN(label, array, v_speed, v_min, v_max, format, flags)
end

---
---@param label string
---@param value number
---@param v_speed number @optional
---@param v_min number @optional
---@param v_max number @optional
---@param format string @optional
---@param flags number @optional, ImGuiSliderFlags
---@return boolean,number
function imgui.dragInt(label, value, v_speed, v_min, v_max, format, flags)
end

---
---@param label string
---@param array number[]
---@param v_speed number @optional
---@param v_min number @optional
---@param v_max number @optional
---@param format string @optional
---@param flags number @optional, ImGuiSliderFlags
---@return boolean,number[]
function imgui.dragIntN(label, array, v_speed, v_min, v_max, format, flags)
end

--
-- Widgets: Regular Sliders
-- - CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
-- - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
-- - Format string may also be set to NULL or use the default format ("%f" or "%d").
-- - Legacy: Pre-1.78 there are SliderXXX() function signatures that takes a final `float power=1.0f' argument instead of the `ImGuiSliderFlags flags=0' argument.
--   If you get a warning converting a float to ImGuiSliderFlags, read https://github.com/ocornut/imgui/issues/3361
--

---
---@param label string
---@param value number
---@param v_min number
---@param v_max number
---@param format string @optional
---@param flags number @optional, ImGuiSliderFlags
---@return boolean,number
function imgui.sliderFloat(label, value, v_min, v_max, format, flags)
end

---
---@param label string
---@param array number[]
---@param v_min number
---@param v_max number
---@param format string @optional
---@param flags number @optional, ImGuiSliderFlags
---@return boolean,number[]
function imgui.sliderFloatN(label, array, v_min, v_max, format, flags)
end

---
---@param label string
---@param v_rad number
---@param v_deg_min number
---@param v_deg_max number
---@param format string
---@return boolean,number
function imgui.sliderAngle(label, v_rad, v_deg_min, v_deg_max, format)
end

---
---@param label string
---@param value number
---@param v_min number
---@param v_max number
---@param format string
---@return boolean,number
function imgui.sliderInt(label, value, v_min, v_max, format)
end

---
---@param label string
---@param array number[]
---@param v_min number
---@param v_max number
---@param format string
---@return boolean,number[]
function imgui.sliderIntN(label, array, v_min, v_max, format, power)
end

---
---@param label string
---@param size ImVec2
---@param value number
---@param v_min number
---@param v_max number
---@param format string
---@param power number
---@return boolean,number
function imgui.vSliderFloat(label, size, value, v_min, v_max, format, power)
end

---
---@param label string
---@param size ImVec2
---@param value number
---@param v_min number
---@param v_max number
---@param format string
---@return boolean,number
function imgui.vSliderInt(label, size, value, v_min, v_max, format)
end

--
-- Widgets: Input with Keyboard
-- - If you want to use InputText() with a dynamic string type such as std::string or your own, see misc/cpp/imgui_stdlib.h
-- - Most of the ImGuiInputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
--

---inputText
---@param label string
---@param str string
---@param flags number
---@return boolean,string
function imgui.inputText(label, str, flags)
end

---inputTextMultiline
---@param label string
---@param str string
---@param size ImVec2
---@param flags number
---@return boolean,string
function imgui.inputTextMultiline(label, str, size, flags)
end

---inputTextWithHint
---@param label string
---@param hint string
---@param str string
---@param flags number
---@return boolean,string
function imgui.inputTextWithHint(label, hint, str, flags)
end

---
---@param label string
---@param v number
---@param step number
---@param step_fase number
---@param format string
---@param flags number
---@return boolean,number
function imgui.inputFloat(label, v, step, step_fase, format, flags)
end

--- Edit a double-precision value; returns the native result flag and the updated value.
---@param label string
---@param v number
---@param step number? Defaults to 0.
---@param step_fast number? Defaults to 0.
---@param format string? Defaults to "%.6f".
---@param flags imgui.ImGuiInputTextFlags? Defaults to 0.
---@return boolean,number
function imgui.inputDouble(label, v, step, step_fast, format, flags)
end

---
---@param label string
---@param v number[]
---@param step number
---@param step_fase number
---@param format string
---@param flags number
---@return boolean,number[]
function imgui.inputFloatN(label, v, step, step_fase, format, flags)
end

---
---@param label string
---@param v number
---@param step number
---@param step_fast number
---@param flags number
---@return boolean,number
function imgui.inputInt(label, v, step, step_fast, flags)
end

---
---@param label string
---@param v number[]
---@param step number
---@param step_fase number
---@param flags number
---@return boolean,number[]
function imgui.inputIntN(label, v, step, step_fase, flags)
end

--
-- Widgets: Color Editor/Picker
-- (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
-- - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
-- - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
--

---
---@param label string
---@param color number[]
---@param flags number
---@return boolean,number[]
function imgui.colorEdit3(label, color, flags)
end

---
---@param label string
---@param color number[]
---@param flags number
---@return boolean,number[]
function imgui.colorEdit4(label, color, flags)
end

---
---@param label string
---@param color number[]
---@param flags number
---@return boolean,number[]
function imgui.colorPicker3(label, color, flags)
end

---
---@param label string
---@param color number[]
---@param flags number
---@param ref_color number[]
---@return boolean,number[]
function imgui.colorPicker4(label, color, flags, ref_color)
end

--- when 'p_visible != NULL': if '*p_visible==true' display an additional small close button on upper right of the header which will set the bool to false when clicked, if '*p_visible==false' don't display the header.
---@param label string
---@param visible boolean
---@param flags number
---@return boolean,boolean
---@overload fun(label:string,flags:number):boolean
function imgui.collapsingHeader(label, visible, flags)
end

--
-- Widgets: Selectables
-- - A selectable highlights when hovered, and can display another color when selected.
-- - Neighbors selectable extend their highlight bounds in order to leave no gap between them.
--

--- "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
---@param label string
---@param selected boolean
---@param flags number
---@param size ImVec2
---@return boolean
function imgui.selectable(label, selected, flags, size)
end

--
-- Widgets: List Boxes
-- - This is essentially a thin wrapper to using BeginChild/EndChild with some stylistic changes.
-- - The BeginListBox()/EndListBox() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() or any items.
-- - The simplified/old ListBox() api are helpers over BeginListBox()/EndListBox() which are kept available for convenience purpose. This is analoguous to how Combos are created.
-- - Choose frame width:   size.x > 0.0f: custom  /  size.x < 0.0f or -FLT_MIN: right-align   /  size.x = 0.0f (default): use current ItemWidth
-- - Choose frame height:  size.y > 0.0f: custom  /  size.y < 0.0f or -FLT_MIN: bottom-align  /  size.y = 0.0f (default): arbitrary default height which can fit ~7 items
--

---
---@param label string
---@param current_item number
---@param items string[]
---@param height_in_items number
---@return boolean,number
function imgui.listBox(label, current_item, items, height_in_items)
end

--
-- Widgets: Data Plotting
--

---
---@param label string
---@param values number[]
---@param values_count number optional
---@param values_offset number optional
---@param overlay_text string optional, nullable
---@param scale_min number optional
---@param scale_max number optional
---@param graph_size ImVec2 optional
---@param stride number optional
function imgui.plotLines(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride)
end

---
---@param label string
---@param values number[]
---@param values_count number optional
---@param values_offset number optional
---@param overlay_text string optional, nullable
---@param scale_min number optional
---@param scale_max number optional
---@param graph_size ImVec2 optional
---@param stride number optional
function imgui.plotHistogram(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride)
end

--
-- Widgets: Menus
--

--- return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
---@param label string
---@param shortcut string optional, nullable
---@param selected boolean optional
---@param enabled boolean optional
---@return boolean
function imgui.menuItem(label, shortcut, selected, enabled)
end

--
-- Popups, Modals
-- The properties of popups windows are:
-- - They block normal mouse hovering detection outside them. (*)
-- - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
-- - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
--   User can manipulate the visibility state by calling OpenPopup().
-- (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
-- Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.
--

--- return true if the modal is open, and you can start outputting to it.
---@param name string
---@param open boolean optional, nullable
---@param flags number optional
---@return boolean,boolean
function imgui.beginPopupModal(name, open, flags)
end

--
-- Docking
--

--- Native field names; missing or nil fields retain the native constructor defaults.
--- An empty table uses all native defaults.
---@class ImGuiWindowClass
---@field ClassId? number Defaults to 0.
---@field ParentViewportId? number Defaults to 4294967295 (native -1).
---@field FocusRouteParentWindowId? number Defaults to 0.
---@field ViewportFlagsOverrideSet? number Defaults to 0.
---@field ViewportFlagsOverrideClear? number Defaults to 0.
---@field TabItemFlagsOverrideSet? number Defaults to 0.
---@field DockNodeFlagsOverrideSet? number Defaults to 0.
---@field DockingAlwaysTabBar? boolean Defaults to false.
---@field DockingAllowUnclassed? boolean Defaults to true.
local ImGuiWindowClass = {}

--- Trailing arguments may be omitted; size and flags must be supplied when passing window_class.
---@param dockspace_id number
---@param size ImVec2 Defaults to {x = 0, y = 0} when omitted.
---@param flags number ImGuiDockNodeFlags, defaults to 0 when omitted.
---@param window_class ImGuiWindowClass|nil Omitted or nil uses the native default class.
---@return number
---@overload fun(dockspace_id:number):number
---@overload fun(dockspace_id:number, size:ImVec2):number
---@overload fun(dockspace_id:number, size:ImVec2, flags:number):number
function imgui.dockSpace(dockspace_id, size, flags, window_class)
end

--- Trailing arguments may be omitted; only viewport and window_class accept nil.
---@param dockspace_id number Defaults to 0 when omitted.
---@param viewport imgui.ImGuiViewport|nil Omitted or nil uses the main viewport.
---@param flags number ImGuiDockNodeFlags, defaults to 0 when omitted.
---@param window_class ImGuiWindowClass|nil Omitted or nil uses the native default class.
---@return number
---@overload fun():number
---@overload fun(dockspace_id:number):number
---@overload fun(dockspace_id:number, viewport:imgui.ImGuiViewport|nil):number
---@overload fun(dockspace_id:number, viewport:imgui.ImGuiViewport|nil, flags:number):number
function imgui.dockSpaceOverViewport(dockspace_id, viewport, flags, window_class)
end

--- Set the class for the next window; window_class must be a non-nil table.
---@param window_class ImGuiWindowClass
function imgui.setNextWindowClass(window_class)
end

--
-- Tables
--

--- Get the current table's sorting specifications, or nil if sorting is disabled.
--- Call inside BeginTable()/EndTable(); do not retain the result across frames or subsequent BeginTable() calls.
---@return imgui.ImGuiTableSortSpecs|nil
function imgui.tableGetSortSpecs()
end

--
-- Multi-selection
--

--- Signed 64-bit item identifier, not a Lua object or an owning pointer.
--- Numbers must be finite integers in [-9007199254740991, 9007199254740991].
--- Use signed ffi int64_t cdata for larger values; never pass them through tonumber().
--- Zero is valid. -1 is an output sentinel and cannot be submitted as an item identifier.
--- Item identifiers are preserved verbatim; no automatic 0-based/1-based conversion occurs.
--- Normalize large-ID table keys explicitly instead of using freshly boxed cdata as keys.
---@alias ImGuiSelectionUserData integer|ffi.cdata

--- Independent table snapshot of one native selection request.
--- Range endpoints are inclusive. For SetAll, endpoints are -1 and RangeDirection is 0.
---@class imgui.ImGuiSelectionRequest
---@field Type imgui.ImGuiSelectionRequestType
---@field Selected boolean
---@field RangeDirection integer +1 or -1 for SetRange, indicating selection direction.
---@field RangeFirstItem ImGuiSelectionUserData
---@field RangeLastItem ImGuiSelectionUserData
local ImGuiSelectionRequest

--- Borrowed IO returned by beginMultiSelect()/endMultiSelect(); never construct or destroy it in Lua.
--- Valid only until any subsequent beginMultiSelect()/endMultiSelect() call, or the next frame.
--- Read and apply Begin's Requests before submitting any items; item submission may clear or replace them.
--- Also read and apply End's Requests immediately after endMultiSelect(); request snapshots are independent.
---@class imgui.ImGuiMultiSelectIO
---@field Requests imgui.ImGuiSelectionRequest[] Read-only property; each read returns a 1-based deep copy.
---@field RangeSrcItem ImGuiSelectionUserData Read-only source item, or -1; ensure it is submitted when clipping.
---@field NavIdItem ImGuiSelectionUserData Read-only navigation item, or -1.
---@field NavIdSelected boolean Read-only navigation selection state.
---@field ItemsCount integer Read-only item count passed to beginMultiSelect(), or -1 if unknown.
---@field RangeSrcReset boolean Writable; set on Begin's IO before endMultiSelect() when deleting the range source.
local ImGuiMultiSelectIO

--- Set the next item's exact identifier. Rejects nil, strings, fractions, unsafe numbers,
--- non-int64 cdata, and the reserved -1 value, including with binding checks disabled.
---@param selection_user_data ImGuiSelectionUserData
function imgui.setNextItemSelectionUserData(selection_user_data)
end

--- Owning selection storage, collected by Lua and safe to retain across frames.
--- Storage IDs are Lua integer numbers in [0, 4294967295], including zero (not int64 cdata).
--- With this helper, submit zero-based item indices to setNextItemSelectionUserData(), not storage IDs.
--- UserData, AdapterIndexToStorageId, internal storage and native iteration are not exposed.
---@class imgui.ImGuiSelectionBasicStorage
---@field Size integer Read-only selected item count.
---@field PreserveOrder boolean Writable; snapshots use selection order when true, ascending ID otherwise. Defaults to false.
local ImGuiSelectionBasicStorage

--- Create empty storage with identity index-to-ID mapping. Also callable as imgui.ImGuiSelectionBasicStorage().
---@return imgui.ImGuiSelectionBasicStorage
function ImGuiSelectionBasicStorage.new()
end

---@param id integer Unsigned 32-bit storage ID.
---@return boolean
function ImGuiSelectionBasicStorage:contains(id)
end

--- Update selection by storage ID; the ID need not currently appear in the mapping.
--- If the native selection-order counter is exhausted, clear the storage before adding selections.
---@param id integer Unsigned 32-bit storage ID.
---@param selected boolean
function ImGuiSelectionBasicStorage:setItemSelected(id, selected)
end

--- Clear selection and reset its order counter; retain the mapping and PreserveOrder.
function ImGuiSelectionBasicStorage:clear()
end

--- Swap selected IDs and their order counters only. Each object retains its mapping and PreserveOrder.
---@param other imgui.ImGuiSelectionBasicStorage
function ImGuiSelectionBasicStorage:swap(other)
end

--- Copy a dense 1-based array of unique unsigned 32-bit IDs; ids[index + 1] maps a zero-based index.
--- nil restores identity mapping; an empty table installs an empty mapping. Invalid input leaves the old map intact.
--- Replacing the mapping does not change selected IDs. Keep it consistent with the submitted item order.
---@param ids integer[]|nil
function ImGuiSelectionBasicStorage:setIndexToStorageIds(ids)
end

--- Resolve a zero-based non-negative int index; explicit mappings require the index to be in bounds.
---@param index integer
---@return integer
function ImGuiSelectionBasicStorage:getStorageIdFromIndex(index)
end

--- Apply live IO, not its Requests snapshot. Requires known ItemsCount >= 0 and mapping coverage for all items.
--- Range endpoints must be ordered zero-based indices within ItemsCount; invalid requests fail before mutation.
--- Apply Begin's IO before submitting any items, and End's IO immediately after endMultiSelect().
--- Do not retain borrowed IO beyond its documented lifetime; no lifetime validation is performed here.
---@param io imgui.ImGuiMultiSelectIO
function ImGuiSelectionBasicStorage:applyRequests(io)
end

--- Return a fresh 1-based array of selected storage IDs, with no sentinel or native iterator state.
--- Editing the array or subsequently changing storage does not affect the other; reading never reorders storage.
---@return integer[]
function ImGuiSelectionBasicStorage:getSelectedItems()
end

--
-- Tab Bars, Tabs
-- [BETA API] API may evolve!
-- Note: Tabs are automatically created by the docking system. Use this to create tab bars/tabs yourself without docking being involved.
--

--- create a Tab. Returns true if the Tab is selected.
---@param name string
---@param open boolean optional, nullable
---@param flags number optional
---@return boolean,boolean
function imgui.beginTabItem(name, open, flags)
end

--
-- Drag and Drop
-- [BETA API] API may evolve!
--

--- type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types.
---@param type string
---@param value any
---@param flags number
---@return boolean
function imgui.setDragDropPayload(type, value, flags)
end

--- accept contents of a given type. If ImGuiDragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
---@param type string
---@param flags number
---@return boolean,any
function imgui.acceptDragDropPayload(type, flags)
end

--- peek directly into the current payload from anywhere. may return NULL. use ImGuiPayload::IsDataType() to test for the payload type.
---@return boolean,any
function imgui.getDragDropPayload()
end

--
-- Miscellaneous Utilities
--

--- calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.
---@param items_count number
---@param items_height number
---@return number,number out_items_display_start, out_items_display_end
function imgui.calcListClipping(items_count, items_height)
end

--
-- Inputs Utilities
--

--- by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse
---@param mouse_pos ImVec2 optional
function imgui.isMousePosValid(mouse_pos)
end

--
-- ID
--

--- push string into the ID stack (will hash string).
--- push integer into the ID stack (will hash integer).
---@param id number|string
function imgui.pushID(id)
end

--
-- Main
--

--- access the IO structure (mouse/keyboard/gamepad inputs, time, various configuration options/flags)
---@return imgui.ImGuiIO
function imgui.getIO()
end

--- access the Style structure (colors, sizes). Always use PushStyleCol(), PushStyleVar() to modify style mid-frame.
---@return imgui.ImGuiStyle
function imgui.getStyle()
end

---
---@param idx number
---@param col ImVec4
function imgui.setStyleColor(idx, col)
end

--- return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.
---@return string
function imgui.saveIniSettingsToMemory()
end

---
---@param path string
---@param size number
---@param config table
--- Ranges are dense start/end pairs with inclusive endpoints; an optional final 0 is accepted.
--- Invalid endpoints/presets raise an error. Arrays are copied into persistent native storage.
---@param ranges integer|integer[] Preset 0..7 or ranges; omitted defaults to ChineseFull.
---@return imgui.ImFont
function imgui.addFontTTF(path, size, config, ranges)
end

---@return cc.Layer
function imgui.createLayer()
end

function imgui.shutdown()
end

---
---@param node cc.Node
---@param tint_color ImVec4
---@param border_color ImVec4
function imgui.ccNode(node, tint_color, border_color)
end

---
---@param node cc.Node
---@param frame_padding number if <0, uses default frame padding settings. 0 for no padding
---@param bg_color ImVec4
---@param tint_color ImVec4
---@return boolean
function imgui.ccNodeButton(node, frame_padding, bg_color, tint_color)
end

---
---@param node cc.Node
---@param col number|ImVec4
function imgui.setCCNodeColor(node, col)
end

---
---@param label cc.Label
---@param col number|ImVec4|boolean
function imgui.setCCLabelColor(label, col)
end

-- markdown

---
---@param f fun(text:string,link:string,isImage:boolean)
function imgui.setMarkdownLinkCallback(f)
end

---
---@param f fun(text:string,link:string):cc.Sprite,ImVec2,ImVec4,ImVec4
function imgui.setMarkdownImageCallback(f)
end

---
---@param index number
---@param font imgui.ImFont
---@param seperator boolean
---@param scale number optional
function imgui.setMarkdownFont(index, font, seperator, scale)
end

---
---@param icon string
function imgui.setMarkdownLinkIcon(icon)
end

---
---@param content string
function imgui.markdown(content)
end

---@class imgui.ImGuiTableSortSpecs
---@field Specs ImGuiTableColumnSortSpecs[] Read-only property; returns a 1-based copy of the sort specification array.
---@field SpecsCount integer Read-only sort specification count; may be zero with SortTristate.
---@field SpecsDirty boolean Set to false after sorting the data.
local ImGuiTableSortSpecs

---@class ImGuiTableColumnSortSpecs
---@field ColumnUserID integer User data passed to TableSetupColumn().
---@field ColumnIndex integer Zero-based column index.
---@field SortOrder integer Zero-based sort priority.
---@field SortDirection imgui.ImGuiSortDirection
local ImGuiTableColumnSortSpecs

--- The plain configuration table accepted by imgui.addFontTTF() also supports Name.
--- Omitted or nil Name keeps the native default; an empty Name uses the generated file/size name.
---@class imgui.ImFontConfig
---@field Name string Debug name, readable and writable. Writes keep at most 39 bytes plus the terminating NUL; embedded NUL bytes are rejected.
local ImFontConfig

---@type imgui.ImFontAtlas
local ImFontAtlas

--- Returns a fresh 1-based snapshot of inclusive start/end pairs, without the native terminating zero.
--- The default ranges are {0x0020, 0x00FF}; changing the table does not affect native storage.
--- The result can be passed directly as imgui.addFontTTF()'s ranges argument.
---@return integer[]
function ImFontAtlas:getGlyphRangesDefault()
end

--- Independent glyph data copied before returning to Lua.
--- UV values describe the atlas at lookup time; reacquire after atlas/texture changes.
---@class imgui.ImFontGlyphSnapshot
---@field Colored boolean
---@field Visible boolean
---@field SourceIdx integer
---@field Codepoint integer
---@field AdvanceX number
---@field X0 number
---@field Y0 number
---@field X1 number
---@field Y1 number
---@field U0 number
---@field V0 number
---@field U1 number
---@field V1 number
local ImFontGlyphSnapshot

---@type imgui.ImFontBaked
local ImFontBaked

--- Look up (and possibly load) a glyph, returning a copy or the fallback glyph's copy.
--- The borrowed baked font must belong to the current frame and a still-valid font/atlas.
---@param codepoint integer Unicode code point supported by this build's ImWchar.
---@return imgui.ImFontGlyphSnapshot
function ImFontBaked:findGlyph(codepoint)
end

--- Look up (and possibly load) a glyph, returning nil when the font has no such glyph.
--- The returned table does not refer to the native Glyphs array.
---@param codepoint integer Unicode code point supported by this build's ImWchar.
---@return imgui.ImFontGlyphSnapshot|nil
function ImFontBaked:findGlyphNoFallback(codepoint)
end

---@class ImVec2:vec2_table
local ImVec2

---@class ImVec4:vec4_table
local ImVec4
