------------------------------
--- Tag: imgui
------------------------------

---@class imgui
imgui = imgui or {}
---@class ImVec2
local ImVec2 = { x = 0, y = 0 }
---@class ImVec4
local ImVec4 = { x = 0, y = 0, z = 0, w = 0 }

local ImGuiWindowFlags = {
	None = 0,
	NoTitleBar = 1,
	NoResize = 2,
	NoMove = 4,
	NoScrollbar = 8,
	NoScrollWithMouse = 16,
	NoCollapse = 32,
	AlwaysAutoResize = 64,
	NoBackground = 128,
	NoSavedSettings = 256,
	NoMouseInputs = 512,
	MenuBar = 1024,
	HorizontalScrollbar = 2048,
	NoFocusOnAppearing = 4096,
	NoBringToFrontOnFocus = 8192,
	AlwaysVerticalScrollbar = 16384,
	AlwaysHorizontalScrollbar = 32768,
	NoNavInputs = 65536,
	NoNavFocus = 131072,
	UnsavedDocument = 262144,
	NoDocking = 524288,
	NoNav = 196608,
	NoDecoration = 43,
	NoInputs = 197120,
	DockNodeHost = 8388608,
	ChildWindow = 16777216,
	Tooltip = 33554432,
	Popup = 67108864,
	Modal = 134217728,
	ChildMenu = 268435456,
}
---
---@class imgui.ImGuiWindowFlags
imgui.ImGuiWindowFlags = ImGuiWindowFlags

local ImGuiChildFlags = {
	ImGuiChildFlags_None = 0,
	ImGuiChildFlags_Borders = 1,
	ImGuiChildFlags_AlwaysUseWindowPadding = 2,
	ImGuiChildFlags_ResizeX = 4,
	ImGuiChildFlags_ResizeY = 8,
	ImGuiChildFlags_AutoResizeX = 16,
	ImGuiChildFlags_AutoResizeY = 32,
	ImGuiChildFlags_AlwaysAutoResize = 64,
	ImGuiChildFlags_FrameStyle = 128,
	ImGuiChildFlags_NavFlattened = 256,
}
---
---@class imgui.ImGuiChildFlags
imgui.ImGuiChildFlags = ImGuiChildFlags

local ImGuiItemFlags = {
	ImGuiItemFlags_None = 0,
	ImGuiItemFlags_NoTabStop = 1,
	ImGuiItemFlags_NoNav = 2,
	ImGuiItemFlags_NoNavDefaultFocus = 4,
	ImGuiItemFlags_ButtonRepeat = 8,
	ImGuiItemFlags_AutoClosePopups = 16,
	ImGuiItemFlags_AllowDuplicateId = 32,
	ImGuiItemFlags_Disabled = 64,
	ImGuiItemFlags_LiveEditOnInputText = 128,
	ImGuiItemFlags_LiveEditOnInputScalar = 256,
	ImGuiItemFlags_LiveEditOnInput = 384,
	ImGuiItemFlags_MixedValue = 512,
}
---
---@class imgui.ImGuiItemFlags
imgui.ImGuiItemFlags = ImGuiItemFlags

local ImGuiInputTextFlags = {
	None = 0,
	CharsDecimal = 1,
	CharsHexadecimal = 2,
	CharsScientific = 4,
	CharsUppercase = 8,
	CharsNoBlank = 16,
	AllowTabInput = 32,
	EnterReturnsTrue = 64,
	EscapeClearsAll = 128,
	CtrlEnterForNewLine = 256,
	ReadOnly = 512,
	Password = 1024,
	AlwaysOverwrite = 2048,
	AutoSelectAll = 4096,
	ParseEmptyRefVal = 8192,
	DisplayEmptyRefVal = 16384,
	NoHorizontalScroll = 32768,
	NoUndoRedo = 65536,
	ElideLeft = 131072,
	CallbackCompletion = 262144,
	CallbackHistory = 524288,
	CallbackAlways = 1048576,
	CallbackCharFilter = 2097152,
	CallbackResize = 4194304,
	CallbackEdit = 8388608,
	WordWrap = 16777216,
}
---
---@class imgui.ImGuiInputTextFlags
imgui.ImGuiInputTextFlags = ImGuiInputTextFlags

local ImGuiTreeNodeFlags = {
	None = 0,
	Selected = 1,
	Framed = 2,
	AllowOverlap = 4,
	NoTreePushOnOpen = 8,
	NoAutoOpenOnLog = 16,
	DefaultOpen = 32,
	OpenOnDoubleClick = 64,
	OpenOnArrow = 128,
	Leaf = 256,
	Bullet = 512,
	FramePadding = 1024,
	SpanAvailWidth = 2048,
	SpanFullWidth = 4096,
	SpanLabelWidth = 8192,
	SpanAllColumns = 16384,
	LabelSpanAllColumns = 32768,
	NavLeftJumpsToParent = 131072,
	CollapsingHeader = 26,
	DrawLinesNone = 262144,
	DrawLinesFull = 524288,
	DrawLinesToNodes = 1048576,
}
---
---@class imgui.ImGuiTreeNodeFlags
imgui.ImGuiTreeNodeFlags = ImGuiTreeNodeFlags

local ImGuiPopupFlags = {
	None = 0,
	MouseButtonLeft = 4,
	MouseButtonRight = 8,
	MouseButtonMiddle = 12,
	NoReopen = 32,
	NoOpenOverExistingPopup = 128,
	NoOpenOverItems = 256,
	AnyPopupId = 1024,
	AnyPopupLevel = 2048,
	AnyPopup = 3072,
	MouseButtonShift_ = 2,
	MouseButtonMask_ = 12,
	InvalidMask_ = 3,
}
---
---@class imgui.ImGuiPopupFlags
imgui.ImGuiPopupFlags = ImGuiPopupFlags

local ImGuiSelectableFlags = {
	None = 0,
	NoAutoClosePopups = 1,
	SpanAllColumns = 2,
	AllowDoubleClick = 4,
	Disabled = 8,
	AllowOverlap = 16,
	Highlight = 32,
	SelectOnNav = 64,
}
---
---@class imgui.ImGuiSelectableFlags
imgui.ImGuiSelectableFlags = ImGuiSelectableFlags

local ImGuiComboFlags = {
	None = 0,
	PopupAlignLeft = 1,
	HeightSmall = 2,
	HeightRegular = 4,
	HeightLarge = 8,
	HeightLargest = 16,
	NoArrowButton = 32,
	NoPreview = 64,
	WidthFitPreview = 128,
	HeightMask_ = 30,
}
---
---@class imgui.ImGuiComboFlags
imgui.ImGuiComboFlags = ImGuiComboFlags

local ImGuiTabBarFlags = {
	None = 0,
	Reorderable = 1,
	AutoSelectNewTabs = 2,
	TabListPopupButton = 4,
	NoCloseWithMiddleMouseButton = 8,
	NoTabListScrollingButtons = 16,
	NoTooltip = 32,
	DrawSelectedOverline = 64,
	FittingPolicyMixed = 128,
	FittingPolicyShrink = 256,
	FittingPolicyScroll = 512,
	FittingPolicyMask_ = 896,
	FittingPolicyDefault_ = 128,
}
---
---@class imgui.ImGuiTabBarFlags
imgui.ImGuiTabBarFlags = ImGuiTabBarFlags

local ImGuiTabItemFlags = {
	None = 0,
	UnsavedDocument = 1,
	SetSelected = 2,
	NoCloseWithMiddleMouseButton = 4,
	NoPushId = 8,
	NoTooltip = 16,
	NoReorder = 32,
	Leading = 64,
	Trailing = 128,
	NoAssumedClosure = 256,
}
---
---@class imgui.ImGuiTabItemFlags
imgui.ImGuiTabItemFlags = ImGuiTabItemFlags

local ImGuiFocusedFlags = {
	None = 0,
	ChildWindows = 1,
	RootWindow = 2,
	AnyWindow = 4,
	NoPopupHierarchy = 8,
	DockHierarchy = 16,
	RootAndChildWindows = 3,
}
---
---@class imgui.ImGuiFocusedFlags
imgui.ImGuiFocusedFlags = ImGuiFocusedFlags

local ImGuiHoveredFlags = {
	None = 0,
	ChildWindows = 1,
	RootWindow = 2,
	AnyWindow = 4,
	NoPopupHierarchy = 8,
	DockHierarchy = 16,
	AllowWhenBlockedByPopup = 32,
	AllowWhenBlockedByActiveItem = 128,
	AllowWhenOverlappedByItem = 256,
	AllowWhenOverlappedByWindow = 512,
	AllowWhenDisabled = 1024,
	NoNavOverride = 2048,
	AllowWhenOverlapped = 768,
	RectOnly = 928,
	RootAndChildWindows = 3,
	ForTooltip = 4096,
	Stationary = 8192,
	DelayNone = 16384,
	DelayShort = 32768,
	DelayNormal = 65536,
	NoSharedDelay = 131072,
}
---
---@class imgui.ImGuiHoveredFlags
imgui.ImGuiHoveredFlags = ImGuiHoveredFlags

local ImGuiDockNodeFlags = {
	None = 0,
	KeepAliveOnly = 1,
	NoDockingOverCentralNode = 4,
	PassthruCentralNode = 8,
	NoDockingSplit = 16,
	NoResize = 32,
	AutoHideTabBar = 64,
	NoUndocking = 128,
}
---
---@class imgui.ImGuiDockNodeFlags
imgui.ImGuiDockNodeFlags = ImGuiDockNodeFlags

local ImGuiDragDropFlags = {
	None = 0,
	SourceNoPreviewTooltip = 1,
	SourceNoDisableHover = 2,
	SourceNoHoldToOpenOthers = 4,
	SourceAllowNullID = 8,
	SourceExtern = 16,
	PayloadAutoExpire = 32,
	PayloadNoCrossContext = 64,
	PayloadNoCrossProcess = 128,
	AcceptBeforeDelivery = 1024,
	AcceptNoDrawDefaultRect = 2048,
	AcceptNoPreviewTooltip = 4096,
	AcceptDrawAsHovered = 8192,
	AcceptPeekOnly = 3072,
}
---
---@class imgui.ImGuiDragDropFlags
imgui.ImGuiDragDropFlags = ImGuiDragDropFlags

local ImGuiDataType = {
	S8 = 0,
	U8 = 1,
	S16 = 2,
	U16 = 3,
	S32 = 4,
	U32 = 5,
	S64 = 6,
	U64 = 7,
	Float = 8,
	Double = 9,
	Bool = 10,
	String = 11,
	COUNT = 12,
}
---
---@class imgui.ImGuiDataType
imgui.ImGuiDataType = ImGuiDataType

local ImGuiDir = {
	None = -1,
	Left = 0,
	Right = 1,
	Up = 2,
	Down = 3,
	COUNT = 4,
}
---
---@class imgui.ImGuiDir
imgui.ImGuiDir = ImGuiDir

local ImGuiSortDirection = {
	None = 0,
	Ascending = 1,
	Descending = 2,
}
---
---@class imgui.ImGuiSortDirection
imgui.ImGuiSortDirection = ImGuiSortDirection

local ImGuiKey = {
	None = 0,
	NamedKey_BEGIN = 512,
	Tab = 512,
	LeftArrow = 513,
	RightArrow = 514,
	UpArrow = 515,
	DownArrow = 516,
	PageUp = 517,
	PageDown = 518,
	Home = 519,
	End = 520,
	Insert = 521,
	Delete = 522,
	Backspace = 523,
	Space = 524,
	Enter = 525,
	Escape = 526,
	LeftCtrl = 527,
	LeftShift = 528,
	LeftAlt = 529,
	LeftSuper = 530,
	RightCtrl = 531,
	RightShift = 532,
	RightAlt = 533,
	RightSuper = 534,
	Menu = 535,
	["0"] = 536,
	["1"] = 537,
	["2"] = 538,
	["3"] = 539,
	["4"] = 540,
	["5"] = 541,
	["6"] = 542,
	["7"] = 543,
	["8"] = 544,
	["9"] = 545,
	A = 546,
	B = 547,
	C = 548,
	D = 549,
	E = 550,
	F = 551,
	G = 552,
	H = 553,
	I = 554,
	J = 555,
	K = 556,
	L = 557,
	M = 558,
	N = 559,
	O = 560,
	P = 561,
	Q = 562,
	R = 563,
	S = 564,
	T = 565,
	U = 566,
	V = 567,
	W = 568,
	X = 569,
	Y = 570,
	Z = 571,
	F1 = 572,
	F2 = 573,
	F3 = 574,
	F4 = 575,
	F5 = 576,
	F6 = 577,
	F7 = 578,
	F8 = 579,
	F9 = 580,
	F10 = 581,
	F11 = 582,
	F12 = 583,
	F13 = 584,
	F14 = 585,
	F15 = 586,
	F16 = 587,
	F17 = 588,
	F18 = 589,
	F19 = 590,
	F20 = 591,
	F21 = 592,
	F22 = 593,
	F23 = 594,
	F24 = 595,
	Apostrophe = 596,
	Comma = 597,
	Minus = 598,
	Period = 599,
	Slash = 600,
	Semicolon = 601,
	Equal = 602,
	LeftBracket = 603,
	Backslash = 604,
	RightBracket = 605,
	GraveAccent = 606,
	CapsLock = 607,
	ScrollLock = 608,
	NumLock = 609,
	PrintScreen = 610,
	Pause = 611,
	Keypad0 = 612,
	Keypad1 = 613,
	Keypad2 = 614,
	Keypad3 = 615,
	Keypad4 = 616,
	Keypad5 = 617,
	Keypad6 = 618,
	Keypad7 = 619,
	Keypad8 = 620,
	Keypad9 = 621,
	KeypadDecimal = 622,
	KeypadDivide = 623,
	KeypadMultiply = 624,
	KeypadSubtract = 625,
	KeypadAdd = 626,
	KeypadEnter = 627,
	KeypadEqual = 628,
	AppBack = 629,
	AppForward = 630,
	Oem102 = 631,
	GamepadStart = 632,
	GamepadBack = 633,
	GamepadFaceLeft = 634,
	GamepadFaceRight = 635,
	GamepadFaceUp = 636,
	GamepadFaceDown = 637,
	GamepadDpadLeft = 638,
	GamepadDpadRight = 639,
	GamepadDpadUp = 640,
	GamepadDpadDown = 641,
	GamepadL1 = 642,
	GamepadR1 = 643,
	GamepadL2 = 644,
	GamepadR2 = 645,
	GamepadL3 = 646,
	GamepadR3 = 647,
	GamepadLStickLeft = 648,
	GamepadLStickRight = 649,
	GamepadLStickUp = 650,
	GamepadLStickDown = 651,
	GamepadRStickLeft = 652,
	GamepadRStickRight = 653,
	GamepadRStickUp = 654,
	GamepadRStickDown = 655,
	MouseLeft = 656,
	MouseRight = 657,
	MouseMiddle = 658,
	MouseX1 = 659,
	MouseX2 = 660,
	MouseWheelX = 661,
	MouseWheelY = 662,
	ReservedForModCtrl = 663,
	ReservedForModShift = 664,
	ReservedForModAlt = 665,
	ReservedForModSuper = 666,
	NamedKey_END = 667,
	NamedKey_COUNT = 155,
	ImGuiMod_None = 0,
	ImGuiMod_Ctrl = 4096,
	ImGuiMod_Shift = 8192,
	ImGuiMod_Alt = 16384,
	ImGuiMod_Super = 32768,
	ImGuiMod_Mask_ = 61440,
}
---
---@class imgui.ImGuiKey
imgui.ImGuiKey = ImGuiKey

local ImGuiInputFlags = {
	ImGuiInputFlags_None = 0,
	ImGuiInputFlags_Repeat = 1,
	ImGuiInputFlags_RouteActive = 1024,
	ImGuiInputFlags_RouteFocused = 2048,
	ImGuiInputFlags_RouteGlobal = 4096,
	ImGuiInputFlags_RouteAlways = 8192,
	ImGuiInputFlags_RouteOverFocused = 16384,
	ImGuiInputFlags_RouteOverActive = 32768,
	ImGuiInputFlags_RouteUnlessBgFocused = 65536,
	ImGuiInputFlags_RouteFromRootWindow = 131072,
	ImGuiInputFlags_Tooltip = 262144,
}
---
---@class imgui.ImGuiInputFlags
imgui.ImGuiInputFlags = ImGuiInputFlags

local ImGuiConfigFlags = {
	None = 0,
	NavEnableKeyboard = 1,
	NavEnableGamepad = 2,
	NoMouse = 16,
	NoMouseCursorChange = 32,
	NoKeyboard = 64,
	DockingEnable = 128,
	ViewportsEnable = 1024,
	IsSRGB = 1048576,
	IsTouchScreen = 2097152,
}
---
---@class imgui.ImGuiConfigFlags
imgui.ImGuiConfigFlags = ImGuiConfigFlags

local ImGuiBackendFlags = {
	None = 0,
	HasGamepad = 1,
	HasMouseCursors = 2,
	HasSetMousePos = 4,
	RendererHasVtxOffset = 8,
	RendererHasTextures = 16,
	RendererHasViewports = 1024,
	PlatformHasViewports = 2048,
	HasMouseHoveredViewport = 4096,
	HasParentViewport = 8192,
}
---
---@class imgui.ImGuiBackendFlags
imgui.ImGuiBackendFlags = ImGuiBackendFlags

local ImGuiCol = {
	Text = 0,
	TextDisabled = 1,
	WindowBg = 2,
	ChildBg = 3,
	PopupBg = 4,
	Border = 5,
	BorderShadow = 6,
	FrameBg = 7,
	FrameBgHovered = 8,
	FrameBgActive = 9,
	TitleBg = 10,
	TitleBgActive = 11,
	TitleBgCollapsed = 12,
	MenuBarBg = 13,
	ScrollbarBg = 14,
	ScrollbarGrab = 15,
	ScrollbarGrabHovered = 16,
	ScrollbarGrabActive = 17,
	CheckMark = 18,
	CheckboxSelectedBg = 19,
	SliderGrab = 20,
	SliderGrabActive = 21,
	Button = 22,
	ButtonHovered = 23,
	ButtonActive = 24,
	Header = 25,
	HeaderHovered = 26,
	HeaderActive = 27,
	Separator = 28,
	SeparatorHovered = 29,
	SeparatorActive = 30,
	ResizeGrip = 31,
	ResizeGripHovered = 32,
	ResizeGripActive = 33,
	InputTextCursor = 34,
	TabHovered = 35,
	Tab = 36,
	TabSelected = 37,
	TabSelectedOverline = 38,
	TabDimmed = 39,
	TabDimmedSelected = 40,
	TabDimmedSelectedOverline = 41,
	DockingPreview = 42,
	DockingEmptyBg = 43,
	PlotLines = 44,
	PlotLinesHovered = 45,
	PlotHistogram = 46,
	PlotHistogramHovered = 47,
	TableHeaderBg = 48,
	TableBorderStrong = 49,
	TableBorderLight = 50,
	TableRowBg = 51,
	TableRowBgAlt = 52,
	TextLink = 53,
	TextSelectedBg = 54,
	TreeLines = 55,
	DragDropTarget = 56,
	DragDropTargetBg = 57,
	UnsavedMarker = 58,
	NavCursor = 59,
	NavWindowingHighlight = 60,
	NavWindowingDimBg = 61,
	ModalWindowDimBg = 62,
	COUNT = 63,
}
---
---@class imgui.ImGuiCol
imgui.ImGuiCol = ImGuiCol

local ImGuiStyleVar = {
	Alpha = 0,
	DisabledAlpha = 1,
	WindowPadding = 2,
	WindowRounding = 3,
	WindowBorderSize = 4,
	WindowMinSize = 5,
	WindowTitleAlign = 6,
	ChildRounding = 7,
	ChildBorderSize = 8,
	PopupRounding = 9,
	PopupBorderSize = 10,
	FramePadding = 11,
	FrameRounding = 12,
	FrameBorderSize = 13,
	ItemSpacing = 14,
	ItemInnerSpacing = 15,
	IndentSpacing = 16,
	CellPadding = 17,
	ScrollbarSize = 18,
	ScrollbarRounding = 19,
	ScrollbarPadding = 20,
	GrabMinSize = 21,
	GrabRounding = 22,
	ImageRounding = 23,
	ImageBorderSize = 24,
	TabRounding = 25,
	TabBorderSize = 26,
	TabMinWidthBase = 27,
	TabMinWidthShrink = 28,
	TabBarBorderSize = 29,
	TabBarOverlineSize = 30,
	TableAngledHeadersAngle = 31,
	TableAngledHeadersTextAlign = 32,
	TreeLinesSize = 33,
	TreeLinesRounding = 34,
	MenuItemRounding = 35,
	SelectableRounding = 36,
	DragDropTargetRounding = 37,
	ButtonTextAlign = 38,
	SelectableTextAlign = 39,
	SeparatorSize = 40,
	SeparatorTextBorderSize = 41,
	SeparatorTextAlign = 42,
	SeparatorTextPadding = 43,
	DockingSeparatorSize = 44,
	COUNT = 45,
}
---
---@class imgui.ImGuiStyleVar
imgui.ImGuiStyleVar = ImGuiStyleVar

local ImGuiButtonFlags = {
	None = 0,
	MouseButtonLeft = 1,
	MouseButtonRight = 2,
	MouseButtonMiddle = 4,
	MouseButtonMask_ = 7,
	EnableNav = 8,
	AllowOverlap = 4096,
}
---
---@class imgui.ImGuiButtonFlags
imgui.ImGuiButtonFlags = ImGuiButtonFlags

local ImGuiColorEditFlags = {
	None = 0,
	NoAlpha = 2,
	NoPicker = 4,
	NoOptions = 8,
	NoSmallPreview = 16,
	NoInputs = 32,
	NoTooltip = 64,
	NoLabel = 128,
	NoSidePreview = 256,
	NoDragDrop = 512,
	NoBorder = 1024,
	NoColorMarkers = 2048,
	AlphaOpaque = 4096,
	AlphaNoBg = 8192,
	AlphaPreviewHalf = 16384,
	AlphaBar = 262144,
	HDR = 524288,
	DisplayRGB = 1048576,
	DisplayHSV = 2097152,
	DisplayHex = 4194304,
	Uint8 = 8388608,
	Float = 16777216,
	PickerHueBar = 33554432,
	PickerHueWheel = 67108864,
	PickerNoRotate = 134217728,
	InputRGB = 268435456,
	InputHSV = 536870912,
	DefaultOptions_ = 311427072,
	AlphaMask_ = 28674,
	DisplayMask_ = 7340032,
	DataTypeMask_ = 25165824,
	PickerMask_ = 100663296,
	InputMask_ = 805306368,
}
---
---@class imgui.ImGuiColorEditFlags
imgui.ImGuiColorEditFlags = ImGuiColorEditFlags

local ImGuiSliderFlags = {
	None = 0,
	Logarithmic = 32,
	NoRoundToFormat = 64,
	NoInput = 128,
	WrapAround = 256,
	ClampOnInput = 512,
	ClampZeroRange = 1024,
	NoSpeedTweaks = 2048,
	ColorMarkers = 4096,
	AlwaysClamp = 1536,
	InvalidMask_ = 1879048207,
}
---
---@class imgui.ImGuiSliderFlags
imgui.ImGuiSliderFlags = ImGuiSliderFlags

local ImGuiMouseButton = {
	Left = 0,
	Right = 1,
	Middle = 2,
	COUNT = 5,
}
---
---@class imgui.ImGuiMouseButton
imgui.ImGuiMouseButton = ImGuiMouseButton

local ImGuiMouseCursor = {
	None = -1,
	Arrow = 0,
	TextInput = 1,
	ResizeAll = 2,
	ResizeNS = 3,
	ResizeEW = 4,
	ResizeNESW = 5,
	ResizeNWSE = 6,
	Hand = 7,
	Wait = 8,
	Progress = 9,
	NotAllowed = 10,
	COUNT = 11,
}
---
---@class imgui.ImGuiMouseCursor
imgui.ImGuiMouseCursor = ImGuiMouseCursor

local ImGuiMouseSource = {
	ImGuiMouseSource_Mouse = 0,
	ImGuiMouseSource_TouchScreen = 1,
	ImGuiMouseSource_Pen = 2,
	ImGuiMouseSource_COUNT = 3,
}
---
---@class imgui.ImGuiMouseSource
imgui.ImGuiMouseSource = ImGuiMouseSource

local ImGuiCond = {
	None = 0,
	Always = 1,
	Once = 2,
	FirstUseEver = 4,
	Appearing = 8,
}
---
---@class imgui.ImGuiCond
imgui.ImGuiCond = ImGuiCond

local ImGuiTableFlags = {
	None = 0,
	Resizable = 1,
	Reorderable = 2,
	Hideable = 4,
	Sortable = 8,
	NoSavedSettings = 16,
	ContextMenuInBody = 32,
	RowBg = 64,
	BordersInnerH = 128,
	BordersOuterH = 256,
	BordersInnerV = 512,
	BordersOuterV = 1024,
	BordersH = 384,
	BordersV = 1536,
	BordersInner = 640,
	BordersOuter = 1280,
	Borders = 1920,
	NoBordersInBody = 2048,
	NoBordersInBodyUntilResize = 4096,
	SizingFixedFit = 8192,
	SizingFixedSame = 16384,
	SizingStretchProp = 24576,
	SizingStretchSame = 32768,
	NoHostExtendX = 65536,
	NoHostExtendY = 131072,
	NoKeepColumnsVisible = 262144,
	PreciseWidths = 524288,
	NoClip = 1048576,
	PadOuterX = 2097152,
	NoPadOuterX = 4194304,
	NoPadInnerX = 8388608,
	ScrollX = 16777216,
	ScrollY = 33554432,
	SortMulti = 67108864,
	SortTristate = 134217728,
	HighlightHoveredColumn = 268435456,
	SizingMask_ = 57344,
}
---
---@class imgui.ImGuiTableFlags
imgui.ImGuiTableFlags = ImGuiTableFlags

local ImGuiTableColumnFlags = {
	None = 0,
	Disabled = 1,
	DefaultHide = 2,
	DefaultSort = 4,
	WidthStretch = 8,
	WidthFixed = 16,
	NoResize = 32,
	NoReorder = 64,
	NoHide = 128,
	NoClip = 256,
	NoSort = 512,
	NoSortAscending = 1024,
	NoSortDescending = 2048,
	NoHeaderLabel = 4096,
	NoHeaderWidth = 8192,
	PreferSortAscending = 16384,
	PreferSortDescending = 32768,
	IndentEnable = 65536,
	IndentDisable = 131072,
	AngledHeader = 262144,
	IsEnabled = 16777216,
	IsVisible = 33554432,
	IsSorted = 67108864,
	IsHovered = 134217728,
	WidthMask_ = 24,
	IndentMask_ = 196608,
	StatusMask_ = 251658240,
	NoDirectResize_ = 1073741824,
}
---
---@class imgui.ImGuiTableColumnFlags
imgui.ImGuiTableColumnFlags = ImGuiTableColumnFlags

local ImGuiTableRowFlags = {
	None = 0,
	Headers = 1,
}
---
---@class imgui.ImGuiTableRowFlags
imgui.ImGuiTableRowFlags = ImGuiTableRowFlags

local ImGuiTableBgTarget = {
	None = 0,
	RowBg0 = 1,
	RowBg1 = 2,
	CellBg = 3,
}
---
---@class imgui.ImGuiTableBgTarget
imgui.ImGuiTableBgTarget = ImGuiTableBgTarget

local ImGuiListClipperFlags = {
	ImGuiListClipperFlags_None = 0,
	ImGuiListClipperFlags_NoSetTableRowCounters = 1,
}
---
---@class imgui.ImGuiListClipperFlags
imgui.ImGuiListClipperFlags = ImGuiListClipperFlags

local ImGuiMultiSelectFlags = {
	ImGuiMultiSelectFlags_None = 0,
	ImGuiMultiSelectFlags_SingleSelect = 1,
	ImGuiMultiSelectFlags_NoSelectAll = 2,
	ImGuiMultiSelectFlags_NoRangeSelect = 4,
	ImGuiMultiSelectFlags_NoAutoSelect = 8,
	ImGuiMultiSelectFlags_NoAutoClear = 16,
	ImGuiMultiSelectFlags_NoAutoClearOnReselect = 32,
	ImGuiMultiSelectFlags_BoxSelect1d = 64,
	ImGuiMultiSelectFlags_BoxSelect2d = 128,
	ImGuiMultiSelectFlags_BoxSelectNoScroll = 256,
	ImGuiMultiSelectFlags_ClearOnEscape = 512,
	ImGuiMultiSelectFlags_ClearOnClickVoid = 1024,
	ImGuiMultiSelectFlags_ScopeWindow = 2048,
	ImGuiMultiSelectFlags_ScopeRect = 4096,
	ImGuiMultiSelectFlags_SelectOnAuto = 8192,
	ImGuiMultiSelectFlags_SelectOnClickAlways = 16384,
	ImGuiMultiSelectFlags_SelectOnClickRelease = 32768,
	ImGuiMultiSelectFlags_NavWrapX = 65536,
	ImGuiMultiSelectFlags_NoSelectOnRightClick = 131072,
	ImGuiMultiSelectFlags_SelectOnMask_ = 57344,
	ImGuiMultiSelectFlags_CheckboxMode_ = 1048576,
}
---
---@class imgui.ImGuiMultiSelectFlags
imgui.ImGuiMultiSelectFlags = ImGuiMultiSelectFlags

local ImGuiSelectionRequestType = {
	ImGuiSelectionRequestType_None = 0,
	ImGuiSelectionRequestType_SetAll = 1,
	ImGuiSelectionRequestType_SetRange = 2,
}
---
---@class imgui.ImGuiSelectionRequestType
imgui.ImGuiSelectionRequestType = ImGuiSelectionRequestType

local ImTextureFormat = {
	ImTextureFormat_RGBA32 = 0,
	ImTextureFormat_Alpha8 = 1,
}
---
---@class imgui.ImTextureFormat
imgui.ImTextureFormat = ImTextureFormat

local ImTextureStatus = {
	ImTextureStatus_OK = 0,
	ImTextureStatus_Destroyed = 1,
	ImTextureStatus_WantCreate = 2,
	ImTextureStatus_WantUpdates = 3,
	ImTextureStatus_WantDestroy = 4,
}
---
---@class imgui.ImTextureStatus
imgui.ImTextureStatus = ImTextureStatus

---@return nil @(void)
function imgui.alignTextToFramePadding()
end

---@param str_id string @(const char*)
---@param dir imgui.ImGuiDir @(ImGuiDir)
---@return boolean @(bool)
function imgui.arrowButton(str_id, dir)
end

---@param label string @(const char*)
---@param preview_value string @(const char*)
---@param flags number @(ImGuiComboFlags)
---@return boolean @(bool)
function imgui.beginCombo(label, preview_value, flags)
end

---@param label string @(const char*)
---@param preview_value string @(const char*)
---@return boolean @(bool)
function imgui.beginCombo(label, preview_value)
end

---@param disabled boolean @(bool)
---@return nil @(void)
function imgui.beginDisabled(disabled)
end

---@return nil @(void)
function imgui.beginDisabled()
end

---@param flags number @(ImGuiDragDropFlags)
---@return boolean @(bool)
function imgui.beginDragDropSource(flags)
end

---@return boolean @(bool)
function imgui.beginDragDropSource()
end

---@return boolean @(bool)
function imgui.beginDragDropTarget()
end

---@return nil @(void)
function imgui.beginGroup()
end

---@return boolean @(bool)
function imgui.beginItemTooltip()
end

---@param label string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.beginListBox(label, size)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.beginListBox(label)
end

---@return boolean @(bool)
function imgui.beginMainMenuBar()
end

---@param label string @(const char*)
---@param enabled boolean @(bool)
---@return boolean @(bool)
function imgui.beginMenu(label, enabled)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.beginMenu(label)
end

---@return boolean @(bool)
function imgui.beginMenuBar()
end

---@param flags number @(ImGuiMultiSelectFlags)
---@param selection_size number @(int) Defaults to -1 when omitted (unknown size).
---@param items_count number @(int) Defaults to -1 when omitted (unknown count).
--- Borrowed IO; invalidated by the next beginMultiSelect()/endMultiSelect() or next frame.
--- Read and apply Begin's Requests before submitting any items; item submission may clear or replace them.
--- Also read and apply End's Requests immediately after endMultiSelect(); request snapshots are independent.
---@return imgui.ImGuiMultiSelectIO @(ImGuiMultiSelectIO*)
function imgui.beginMultiSelect(flags, selection_size, items_count)
end

---@param flags number @(ImGuiMultiSelectFlags)
---@param selection_size number @(int) Defaults to -1 when omitted (unknown size).
--- Borrowed IO; invalidated by the next beginMultiSelect()/endMultiSelect() or next frame.
--- Read and apply Begin's Requests before submitting any items; item submission may clear or replace them.
--- Also read and apply End's Requests immediately after endMultiSelect(); request snapshots are independent.
---@return imgui.ImGuiMultiSelectIO @(ImGuiMultiSelectIO*)
function imgui.beginMultiSelect(flags, selection_size)
end

---@param flags number @(ImGuiMultiSelectFlags)
--- Borrowed IO; invalidated by the next beginMultiSelect()/endMultiSelect() or next frame.
--- Read and apply Begin's Requests before submitting any items; item submission may clear or replace them.
--- Also read and apply End's Requests immediately after endMultiSelect(); request snapshots are independent.
---@return imgui.ImGuiMultiSelectIO @(ImGuiMultiSelectIO*)
function imgui.beginMultiSelect(flags)
end

---@param str_id string @(const char*)
---@param flags number @(ImGuiWindowFlags)
---@return boolean @(bool)
function imgui.beginPopup(str_id, flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.beginPopup(str_id)
end

---@param str_id string @(const char*)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.beginPopupContextItem(str_id, popup_flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.beginPopupContextItem(str_id)
end

---@return boolean @(bool)
function imgui.beginPopupContextItem()
end

---@param str_id string @(const char*)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.beginPopupContextVoid(str_id, popup_flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.beginPopupContextVoid(str_id)
end

---@return boolean @(bool)
function imgui.beginPopupContextVoid()
end

---@param str_id string @(const char*)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.beginPopupContextWindow(str_id, popup_flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.beginPopupContextWindow(str_id)
end

---@return boolean @(bool)
function imgui.beginPopupContextWindow()
end

---@param str_id string @(const char*)
---@param flags number @(ImGuiTabBarFlags)
---@return boolean @(bool)
function imgui.beginTabBar(str_id, flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.beginTabBar(str_id)
end

---@param str_id string @(const char*)
---@param columns number @(int)
---@param flags number @(ImGuiTableFlags)
---@param outer_size ImVec2 @(ImVec2)
---@param inner_width number @(float)
---@return boolean @(bool)
function imgui.beginTable(str_id, columns, flags, outer_size, inner_width)
end

---@param str_id string @(const char*)
---@param columns number @(int)
---@param flags number @(ImGuiTableFlags)
---@param outer_size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.beginTable(str_id, columns, flags, outer_size)
end

---@param str_id string @(const char*)
---@param columns number @(int)
---@param flags number @(ImGuiTableFlags)
---@return boolean @(bool)
function imgui.beginTable(str_id, columns, flags)
end

---@param str_id string @(const char*)
---@param columns number @(int)
---@return boolean @(bool)
function imgui.beginTable(str_id, columns)
end

---@return boolean @(bool)
function imgui.beginTooltip()
end

---@return nil @(void)
function imgui.bullet()
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.bulletText(fmt)
end

---@param label string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.button(label, size)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.button(label)
end

---@return number @(float)
function imgui.calcItemWidth()
end

---@return nil @(void)
function imgui.closeCurrentPopup()
end

---@param desc_id string @(const char*)
---@param col ImVec4 @(ImVec4)
---@param flags number @(ImGuiColorEditFlags)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.colorButton(desc_id, col, flags, size)
end

---@param desc_id string @(const char*)
---@param col ImVec4 @(ImVec4)
---@param flags number @(ImGuiColorEditFlags)
---@return boolean @(bool)
function imgui.colorButton(desc_id, col, flags)
end

---@param desc_id string @(const char*)
---@param col ImVec4 @(ImVec4)
---@return boolean @(bool)
function imgui.colorButton(desc_id, col)
end

---@param in_ ImVec4 @(ImVec4)
---@return number @(ImU32)
function imgui.colorConvertFloat4ToU32(in_)
end

---@param in_ number @(ImU32)
---@return ImVec4 @(ImVec4)
function imgui.colorConvertU32ToFloat4(in_)
end

---@param count number @(int)
---@param id string @(const char*)
---@param borders boolean @(bool)
---@return nil @(void)
function imgui.columns(count, id, borders)
end

---@param count number @(int)
---@param id string @(const char*)
---@return nil @(void)
function imgui.columns(count, id)
end

---@param count number @(int)
---@return nil @(void)
function imgui.columns(count)
end

---@return nil @(void)
function imgui.columns()
end

---@param version_str string @(const char*)
---@param sz_io number @(size_t)
---@param sz_style number @(size_t)
---@param sz_vec2 number @(size_t)
---@param sz_vec4 number @(size_t)
---@param sz_drawvert number @(size_t)
---@param sz_drawidx number @(size_t)
---@return boolean @(bool)
function imgui.debugCheckVersionAndDataLayout(version_str, sz_io, sz_style, sz_vec2, sz_vec4, sz_drawvert, sz_drawidx)
end

---@param idx number @(ImGuiCol)
---@return nil @(void)
function imgui.debugFlashStyleColor(idx)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.debugLog(fmt)
end

---@return nil @(void)
function imgui.debugStartItemPicker()
end

---@param text string @(const char*)
---@return nil @(void)
function imgui.debugTextEncoding(text)
end

---@return nil @(void)
function imgui.destroyPlatformWindows()
end

---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.dummy(size)
end

---@return nil @(void)
function imgui.endCombo()
end

---@return nil @(void)
function imgui.endDisabled()
end

---@return nil @(void)
function imgui.endDragDropSource()
end

---@return nil @(void)
function imgui.endDragDropTarget()
end

---@return nil @(void)
function imgui.endFrame()
end

---@return nil @(void)
function imgui.endGroup()
end

---@return nil @(void)
function imgui.endListBox()
end

---@return nil @(void)
function imgui.endMainMenuBar()
end

---@return nil @(void)
function imgui.endMenu()
end

---@return nil @(void)
function imgui.endMenuBar()
end

--- Borrowed IO; invalidated by the next beginMultiSelect()/endMultiSelect() or next frame.
--- Read and apply Begin's Requests before submitting any items; item submission may clear or replace them.
--- Also read and apply End's Requests immediately after endMultiSelect(); request snapshots are independent.
---@return imgui.ImGuiMultiSelectIO @(ImGuiMultiSelectIO*)
function imgui.endMultiSelect()
end

---@return nil @(void)
function imgui.endPopup()
end

---@return nil @(void)
function imgui.endTabBar()
end

---@return nil @(void)
function imgui.endTabItem()
end

---@return nil @(void)
function imgui.endTable()
end

---@return nil @(void)
function imgui.endTooltip()
end

---@param viewport_id number @(ImGuiID)
---@return ImGuiViewport @(ImGuiViewport*)
function imgui.findViewportByID(viewport_id)
end

---@param viewport ImGuiViewport @(ImGuiViewport*)
---@return ImDrawList @(ImDrawList*)
function imgui.getBackgroundDrawList(viewport)
end

---@return ImDrawList @(ImDrawList*)
function imgui.getBackgroundDrawList()
end

---@return string @(const char*)
function imgui.getClipboardText()
end

---@return number @(int)
function imgui.getColumnIndex()
end

---@param column_index number @(int)
---@return number @(float)
function imgui.getColumnOffset(column_index)
end

---@return number @(float)
function imgui.getColumnOffset()
end

---@param column_index number @(int)
---@return number @(float)
function imgui.getColumnWidth(column_index)
end

---@return number @(float)
function imgui.getColumnWidth()
end

---@return number @(int)
function imgui.getColumnsCount()
end

---@return ImVec2 @(ImVec2)
function imgui.getContentRegionAvail()
end

---@return ImVec2 @(ImVec2)
function imgui.getCursorPos()
end

---@return number @(float)
function imgui.getCursorPosX()
end

---@return number @(float)
function imgui.getCursorPosY()
end

---@return ImVec2 @(ImVec2)
function imgui.getCursorScreenPos()
end

---@return ImVec2 @(ImVec2)
function imgui.getCursorStartPos()
end

---@return imgui.ImFontBaked|nil @(ImFontBaked*)
function imgui.getFontBaked()
end

---@return number @(float)
function imgui.getFontSize()
end

---@return ImVec2 @(ImVec2)
function imgui.getFontTexUvWhitePixel()
end

---@param viewport ImGuiViewport @(ImGuiViewport*)
---@return ImDrawList @(ImDrawList*)
function imgui.getForegroundDrawList(viewport)
end

---@return ImDrawList @(ImDrawList*)
function imgui.getForegroundDrawList()
end

---@return number @(int)
function imgui.getFrameCount()
end

---@return number @(float)
function imgui.getFrameHeight()
end

---@return number @(float)
function imgui.getFrameHeightWithSpacing()
end

---@param str_id_begin string @(const char*)
---@param str_id_end string @(const char*)
---@return number @(ImGuiID)
function imgui.getID(str_id_begin, str_id_end)
end

---@param int_id number @(int)
---@return number @(ImGuiID)
function imgui.getID(int_id)
end

---@param str_id string @(const char*)
---@return number @(ImGuiID)
function imgui.getID(str_id)
end

---@param mouse_button number @(ImGuiMouseButton)
---@param delay number @(float)
---@return number @(int)
function imgui.getItemClickedCountWithSingleClickDelay(mouse_button, delay)
end

---@param mouse_button number @(ImGuiMouseButton)
---@return number @(int)
function imgui.getItemClickedCountWithSingleClickDelay(mouse_button)
end

---@return number @(int)
function imgui.getItemClickedCountWithSingleClickDelay()
end

---@return number @(ImGuiItemFlags)
function imgui.getItemFlags()
end

---@return number @(ImGuiID)
function imgui.getItemID()
end

---@return ImVec2 @(ImVec2)
function imgui.getItemRectMax()
end

---@return ImVec2 @(ImVec2)
function imgui.getItemRectMin()
end

---@return ImVec2 @(ImVec2)
function imgui.getItemRectSize()
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@return string @(const char*)
function imgui.getKeyName(key)
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@param repeat_delay number @(float)
---@param rate number @(float)
---@return number @(int)
function imgui.getKeyPressedAmount(key, repeat_delay, rate)
end

---@return ImGuiViewport @(ImGuiViewport*)
function imgui.getMainViewport()
end

---@param button number @(ImGuiMouseButton)
---@return number @(int)
function imgui.getMouseClickedCount(button)
end

---@return number @(ImGuiMouseCursor)
function imgui.getMouseCursor()
end

---@param button number @(ImGuiMouseButton)
---@param lock_threshold number @(float)
---@return ImVec2 @(ImVec2)
function imgui.getMouseDragDelta(button, lock_threshold)
end

---@param button number @(ImGuiMouseButton)
---@return ImVec2 @(ImVec2)
function imgui.getMouseDragDelta(button)
end

---@return ImVec2 @(ImVec2)
function imgui.getMouseDragDelta()
end

---@return ImVec2 @(ImVec2)
function imgui.getMousePos()
end

---@return ImVec2 @(ImVec2)
function imgui.getMousePosOnOpeningCurrentPopup()
end

---@return number @(float)
function imgui.getScrollMaxX()
end

---@return number @(float)
function imgui.getScrollMaxY()
end

---@return number @(float)
function imgui.getScrollX()
end

---@return number @(float)
function imgui.getScrollY()
end

---@param idx number @(ImGuiCol)
---@return string @(const char*)
function imgui.getStyleColorName(idx)
end

---@param idx number @(ImGuiCol)
---@return ImVec4 @(const ImVec4&)
function imgui.getStyleColorVec4(idx)
end

---@return number @(float)
function imgui.getTextLineHeight()
end

---@return number @(float)
function imgui.getTextLineHeightWithSpacing()
end

---@return number @(double)
function imgui.getTime()
end

---@return number @(float)
function imgui.getTreeNodeToLabelSpacing()
end

---@return string @(const char*)
function imgui.getVersion()
end

---@return number @(ImGuiID)
function imgui.getWindowDockID()
end

---@return number @(float)
function imgui.getWindowDpiScale()
end

---@return ImDrawList @(ImDrawList*)
function imgui.getWindowDrawList()
end

---@return number @(float)
function imgui.getWindowHeight()
end

---@return ImVec2 @(ImVec2)
function imgui.getWindowPos()
end

---@return ImVec2 @(ImVec2)
function imgui.getWindowSize()
end

---@return ImGuiViewport @(ImGuiViewport*)
function imgui.getWindowViewport()
end

---@return number @(float)
function imgui.getWindowWidth()
end

---@param indent_w number @(float)
---@return nil @(void)
function imgui.indent(indent_w)
end

---@return nil @(void)
function imgui.indent()
end

---@param str_id string @(const char*)
---@param size ImVec2 @(ImVec2)
---@param flags number @(ImGuiButtonFlags)
---@return boolean @(bool)
function imgui.invisibleButton(str_id, size, flags)
end

---@param str_id string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.invisibleButton(str_id, size)
end

---@return boolean @(bool)
function imgui.isAnyItemActive()
end

---@return boolean @(bool)
function imgui.isAnyItemFocused()
end

---@return boolean @(bool)
function imgui.isAnyItemHovered()
end

---@return boolean @(bool)
function imgui.isAnyMouseDown()
end

---@return boolean @(bool)
function imgui.isItemActivated()
end

---@return boolean @(bool)
function imgui.isItemActive()
end

---@param mouse_button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isItemClicked(mouse_button)
end

---@return boolean @(bool)
function imgui.isItemClicked()
end

---@return boolean @(bool)
function imgui.isItemDeactivated()
end

---@return boolean @(bool)
function imgui.isItemDeactivatedAfterEdit()
end

---@return boolean @(bool)
function imgui.isItemEdited()
end

---@return boolean @(bool)
function imgui.isItemFocused()
end

---@param flags number @(ImGuiHoveredFlags)
---@return boolean @(bool)
function imgui.isItemHovered(flags)
end

---@return boolean @(bool)
function imgui.isItemHovered()
end

---@return boolean @(bool)
function imgui.isItemToggledOpen()
end

---@return boolean @(bool)
function imgui.isItemToggledSelection()
end

---@return boolean @(bool)
function imgui.isItemVisible()
end

---@param key_chord number @(ImGuiKeyChord)
---@return boolean @(bool)
function imgui.isKeyChordPressed(key_chord)
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@return boolean @(bool)
function imgui.isKeyDown(key)
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@param repeat_ boolean @(bool)
---@return boolean @(bool)
function imgui.isKeyPressed(key, repeat_)
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@return boolean @(bool)
function imgui.isKeyPressed(key)
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@return boolean @(bool)
function imgui.isKeyReleased(key)
end

---@param button number @(ImGuiMouseButton)
---@param repeat_ boolean @(bool)
---@return boolean @(bool)
function imgui.isMouseClicked(button, repeat_)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseClicked(button)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseDoubleClicked(button)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseDown(button)
end

---@param button number @(ImGuiMouseButton)
---@param lock_threshold number @(float)
---@return boolean @(bool)
function imgui.isMouseDragging(button, lock_threshold)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseDragging(button)
end

---@param r_min ImVec2 @(ImVec2)
---@param r_max ImVec2 @(ImVec2)
---@param clip boolean @(bool)
---@return boolean @(bool)
function imgui.isMouseHoveringRect(r_min, r_max, clip)
end

---@param r_min ImVec2 @(ImVec2)
---@param r_max ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.isMouseHoveringRect(r_min, r_max)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseReleased(button)
end

---@param button number @(ImGuiMouseButton)
---@param delay number @(float)
---@return boolean @(bool)
function imgui.isMouseReleasedWithDelay(button, delay)
end

---@param button number @(ImGuiMouseButton)
---@return boolean @(bool)
function imgui.isMouseReleasedWithDelay(button)
end

---@param str_id string @(const char*)
---@param flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.isPopupOpen(str_id, flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.isPopupOpen(str_id)
end

---@param rect_min ImVec2 @(ImVec2)
---@param rect_max ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.isRectVisible(rect_min, rect_max)
end

---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function imgui.isRectVisible(size)
end

---@return boolean @(bool)
function imgui.isWindowAppearing()
end

---@return boolean @(bool)
function imgui.isWindowCollapsed()
end

---@return boolean @(bool)
function imgui.isWindowDocked()
end

---@param flags number @(ImGuiFocusedFlags)
---@return boolean @(bool)
function imgui.isWindowFocused(flags)
end

---@return boolean @(bool)
function imgui.isWindowFocused()
end

---@param flags number @(ImGuiHoveredFlags)
---@return boolean @(bool)
function imgui.isWindowHovered(flags)
end

---@return boolean @(bool)
function imgui.isWindowHovered()
end

---@param label string @(const char*)
---@param fmt string @(const char*)
---@return nil @(void)
function imgui.labelText(label, fmt)
end

---@param ini_filename string @(const char*)
---@return nil @(void)
function imgui.loadIniSettingsFromDisk(ini_filename)
end

---@param ini_data string @(const char*)
---@param ini_size number @(size_t)
---@return nil @(void)
function imgui.loadIniSettingsFromMemory(ini_data, ini_size)
end

---@param ini_data string @(const char*)
---@return nil @(void)
function imgui.loadIniSettingsFromMemory(ini_data)
end

---@return nil @(void)
function imgui.logButtons()
end

---@return nil @(void)
function imgui.logFinish()
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.logText(fmt)
end

---@param auto_open_depth number @(int)
---@return nil @(void)
function imgui.logToClipboard(auto_open_depth)
end

---@return nil @(void)
function imgui.logToClipboard()
end

---@param auto_open_depth number @(int)
---@param filename string @(const char*)
---@return nil @(void)
function imgui.logToFile(auto_open_depth, filename)
end

---@param auto_open_depth number @(int)
---@return nil @(void)
function imgui.logToFile(auto_open_depth)
end

---@return nil @(void)
function imgui.logToFile()
end

---@param auto_open_depth number @(int)
---@return nil @(void)
function imgui.logToTTY(auto_open_depth)
end

---@return nil @(void)
function imgui.logToTTY()
end

---@return nil @(void)
function imgui.newFrame()
end

---@return nil @(void)
function imgui.newLine()
end

---@return nil @(void)
function imgui.nextColumn()
end

---@param id number @(ImGuiID)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.openPopup(id, popup_flags)
end

---@param str_id string @(const char*)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.openPopup(str_id, popup_flags)
end

---@param id number @(ImGuiID)
---@return boolean @(bool)
function imgui.openPopup(id)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.openPopup(str_id)
end

---@param str_id string @(const char*)
---@param popup_flags number @(ImGuiPopupFlags)
---@return boolean @(bool)
function imgui.openPopupOnItemClick(str_id, popup_flags)
end

---@param str_id string @(const char*)
---@return boolean @(bool)
function imgui.openPopupOnItemClick(str_id)
end

---@return boolean @(bool)
function imgui.openPopupOnItemClick()
end

---@return nil @(void)
function imgui.popClipRect()
end

---@return nil @(void)
function imgui.popFont()
end

---@return nil @(void)
function imgui.popID()
end

---@return nil @(void)
function imgui.popItemFlag()
end

---@return nil @(void)
function imgui.popItemWidth()
end

---@param count number @(int)
---@return nil @(void)
function imgui.popStyleColor(count)
end

---@return nil @(void)
function imgui.popStyleColor()
end

---@param count number @(int)
---@return nil @(void)
function imgui.popStyleVar(count)
end

---@return nil @(void)
function imgui.popStyleVar()
end

---@return nil @(void)
function imgui.popTextWrapPos()
end

---@param fraction number @(float)
---@param size_arg ImVec2 @(ImVec2)
---@param overlay string @(const char*)
---@return nil @(void)
function imgui.progressBar(fraction, size_arg, overlay)
end

---@param fraction number @(float)
---@param size_arg ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.progressBar(fraction, size_arg)
end

---@param fraction number @(float)
---@return nil @(void)
function imgui.progressBar(fraction)
end

---@param clip_rect_min ImVec2 @(ImVec2)
---@param clip_rect_max ImVec2 @(ImVec2)
---@param intersect_with_current_clip_rect boolean @(bool)
---@return nil @(void)
function imgui.pushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect)
end

---@param option number @(ImGuiItemFlags)
---@param enabled boolean @(bool)
---@return nil @(void)
function imgui.pushItemFlag(option, enabled)
end

---@param item_width number @(float)
---@return nil @(void)
function imgui.pushItemWidth(item_width)
end

---@param idx number @(ImGuiStyleVar)
---@param val_x number @(float)
---@return nil @(void)
function imgui.pushStyleVarX(idx, val_x)
end

---@param idx number @(ImGuiStyleVar)
---@param val_y number @(float)
---@return nil @(void)
function imgui.pushStyleVarY(idx, val_y)
end

---@param wrap_local_pos_x number @(float)
---@return nil @(void)
function imgui.pushTextWrapPos(wrap_local_pos_x)
end

---@return nil @(void)
function imgui.pushTextWrapPos()
end

---@return nil @(void)
function imgui.render()
end

---@return nil @(void)
function imgui.renderPlatformWindowsDefault()
end

---@param button number @(ImGuiMouseButton)
---@return nil @(void)
function imgui.resetMouseDragDelta(button)
end

---@return nil @(void)
function imgui.resetMouseDragDelta()
end

---@param offset_from_start_x number @(float)
---@param spacing number @(float)
---@return nil @(void)
function imgui.sameLine(offset_from_start_x, spacing)
end

---@param offset_from_start_x number @(float)
---@return nil @(void)
function imgui.sameLine(offset_from_start_x)
end

---@return nil @(void)
function imgui.sameLine()
end

---@param ini_filename string @(const char*)
---@return nil @(void)
function imgui.saveIniSettingsToDisk(ini_filename)
end

---@return nil @(void)
function imgui.separator()
end

---@param label string @(const char*)
---@return nil @(void)
function imgui.separatorText(label)
end

---@param text string @(const char*)
---@return nil @(void)
function imgui.setClipboardText(text)
end

---@param column_index number @(int)
---@param offset_x number @(float)
---@return nil @(void)
function imgui.setColumnOffset(column_index, offset_x)
end

---@param column_index number @(int)
---@param width number @(float)
---@return nil @(void)
function imgui.setColumnWidth(column_index, width)
end

---@param local_pos ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setCursorPos(local_pos)
end

---@param local_x number @(float)
---@return nil @(void)
function imgui.setCursorPosX(local_x)
end

---@param local_y number @(float)
---@return nil @(void)
function imgui.setCursorPosY(local_y)
end

---@param pos ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setCursorScreenPos(pos)
end

---@return nil @(void)
function imgui.setItemDefaultFocus()
end

---@param key imgui.ImGuiKey @(ImGuiKey)
---@return boolean @(bool)
function imgui.setItemKeyOwner(key)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.setItemTooltip(fmt)
end

---@param offset number @(int)
---@return nil @(void)
function imgui.setKeyboardFocusHere(offset)
end

---@return nil @(void)
function imgui.setKeyboardFocusHere()
end

---@param cursor_type number @(ImGuiMouseCursor)
---@return nil @(void)
function imgui.setMouseCursor(cursor_type)
end

---@param visible boolean @(bool)
---@return nil @(void)
function imgui.setNavCursorVisible(visible)
end

---@param want_capture_keyboard boolean @(bool)
---@return nil @(void)
function imgui.setNextFrameWantCaptureKeyboard(want_capture_keyboard)
end

---@param want_capture_mouse boolean @(bool)
---@return nil @(void)
function imgui.setNextFrameWantCaptureMouse(want_capture_mouse)
end

---@return nil @(void)
function imgui.setNextItemAllowOverlap()
end

---@param is_open boolean @(bool)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setNextItemOpen(is_open, cond)
end

---@param is_open boolean @(bool)
---@return nil @(void)
function imgui.setNextItemOpen(is_open)
end

---@param key_chord number @(ImGuiKeyChord)
---@param flags number @(ImGuiInputFlags)
---@return nil @(void)
function imgui.setNextItemShortcut(key_chord, flags)
end

---@param key_chord number @(ImGuiKeyChord)
---@return nil @(void)
function imgui.setNextItemShortcut(key_chord)
end

---@param storage_id number @(ImGuiID)
---@return nil @(void)
function imgui.setNextItemStorageID(storage_id)
end

---@param item_width number @(float)
---@return nil @(void)
function imgui.setNextItemWidth(item_width)
end

---@param alpha number @(float)
---@return nil @(void)
function imgui.setNextWindowBgAlpha(alpha)
end

---@param collapsed boolean @(bool)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setNextWindowCollapsed(collapsed, cond)
end

---@param collapsed boolean @(bool)
---@return nil @(void)
function imgui.setNextWindowCollapsed(collapsed)
end

---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowContentSize(size)
end

---@param dock_id number @(ImGuiID)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setNextWindowDockID(dock_id, cond)
end

---@param dock_id number @(ImGuiID)
---@return nil @(void)
function imgui.setNextWindowDockID(dock_id)
end

---@return nil @(void)
function imgui.setNextWindowFocus()
end

---@param pos ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@param pivot ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowPos(pos, cond, pivot)
end

---@param pos ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setNextWindowPos(pos, cond)
end

---@param pos ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowPos(pos)
end

---@param scroll ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowScroll(scroll)
end

---@param size ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setNextWindowSize(size, cond)
end

---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowSize(size)
end

---@param size_min ImVec2 @(ImVec2)
---@param size_max ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setNextWindowSizeConstraints(size_min, size_max)
end

---@param viewport_id number @(ImGuiID)
---@return nil @(void)
function imgui.setNextWindowViewport(viewport_id)
end

---@param local_x number @(float)
---@param center_x_ratio number @(float)
---@return nil @(void)
function imgui.setScrollFromPosX(local_x, center_x_ratio)
end

---@param local_x number @(float)
---@return nil @(void)
function imgui.setScrollFromPosX(local_x)
end

---@param local_y number @(float)
---@param center_y_ratio number @(float)
---@return nil @(void)
function imgui.setScrollFromPosY(local_y, center_y_ratio)
end

---@param local_y number @(float)
---@return nil @(void)
function imgui.setScrollFromPosY(local_y)
end

---@param center_x_ratio number @(float)
---@return nil @(void)
function imgui.setScrollHereX(center_x_ratio)
end

---@return nil @(void)
function imgui.setScrollHereX()
end

---@param center_y_ratio number @(float)
---@return nil @(void)
function imgui.setScrollHereY(center_y_ratio)
end

---@return nil @(void)
function imgui.setScrollHereY()
end

---@param scroll_x number @(float)
---@return nil @(void)
function imgui.setScrollX(scroll_x)
end

---@param scroll_y number @(float)
---@return nil @(void)
function imgui.setScrollY(scroll_y)
end

---@param tab_or_docked_window_label string @(const char*)
---@return nil @(void)
function imgui.setTabItemClosed(tab_or_docked_window_label)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.setTooltip(fmt)
end

---@param name string @(const char*)
---@param collapsed boolean @(bool)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowCollapsed(name, collapsed, cond)
end

---@param name string @(const char*)
---@param collapsed boolean @(bool)
---@return nil @(void)
function imgui.setWindowCollapsed(name, collapsed)
end

---@param collapsed boolean @(bool)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowCollapsed(collapsed, cond)
end

---@param collapsed boolean @(bool)
---@return nil @(void)
function imgui.setWindowCollapsed(collapsed)
end

---@param name string @(const char*)
---@return nil @(void)
function imgui.setWindowFocus(name)
end

---@return nil @(void)
function imgui.setWindowFocus()
end

---@param name string @(const char*)
---@param pos ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowPos(name, pos, cond)
end

---@param name string @(const char*)
---@param pos ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setWindowPos(name, pos)
end

---@param pos ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowPos(pos, cond)
end

---@param pos ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setWindowPos(pos)
end

---@param name string @(const char*)
---@param size ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowSize(name, size, cond)
end

---@param name string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setWindowSize(name, size)
end

---@param size ImVec2 @(ImVec2)
---@param cond number @(ImGuiCond)
---@return nil @(void)
function imgui.setWindowSize(size, cond)
end

---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function imgui.setWindowSize(size)
end

---@param key_chord number @(ImGuiKeyChord)
---@param flags number @(ImGuiInputFlags)
---@return boolean @(bool)
function imgui.shortcut(key_chord, flags)
end

---@param key_chord number @(ImGuiKeyChord)
---@return boolean @(bool)
function imgui.shortcut(key_chord)
end

---@param label string @(const char*)
---@return nil @(void)
function imgui.showFontSelector(label)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.showStyleSelector(label)
end

---@return nil @(void)
function imgui.showUserGuide()
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.smallButton(label)
end

---@return nil @(void)
function imgui.spacing()
end

---@param label string @(const char*)
---@param flags number @(ImGuiTabItemFlags)
---@return boolean @(bool)
function imgui.tabItemButton(label, flags)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.tabItemButton(label)
end

---@return nil @(void)
function imgui.tableAngledHeadersRow()
end

---@return number @(int)
function imgui.tableGetColumnCount()
end

---@param column_n number @(int)
---@return number @(ImGuiTableColumnFlags)
function imgui.tableGetColumnFlags(column_n)
end

---@return number @(ImGuiTableColumnFlags)
function imgui.tableGetColumnFlags()
end

---@return number @(int)
function imgui.tableGetColumnIndex()
end

---@param column_n number @(int)
---@return string @(const char*)
function imgui.tableGetColumnName(column_n)
end

---@return string @(const char*)
function imgui.tableGetColumnName()
end

---@return number @(int)
function imgui.tableGetHoveredColumn()
end

---@return number @(int)
function imgui.tableGetRowIndex()
end

---@param label string @(const char*)
---@return nil @(void)
function imgui.tableHeader(label)
end

---@return nil @(void)
function imgui.tableHeadersRow()
end

---@return boolean @(bool)
function imgui.tableNextColumn()
end

---@param row_flags number @(ImGuiTableRowFlags)
---@param min_row_height number @(float)
---@return nil @(void)
function imgui.tableNextRow(row_flags, min_row_height)
end

---@param row_flags number @(ImGuiTableRowFlags)
---@return nil @(void)
function imgui.tableNextRow(row_flags)
end

---@return nil @(void)
function imgui.tableNextRow()
end

---@param target number @(ImGuiTableBgTarget)
---@param color number @(ImU32)
---@param column_n number @(int)
---@return nil @(void)
function imgui.tableSetBgColor(target, color, column_n)
end

---@param target number @(ImGuiTableBgTarget)
---@param color number @(ImU32)
---@return nil @(void)
function imgui.tableSetBgColor(target, color)
end

---@param column_n number @(int)
---@param v boolean @(bool)
---@return nil @(void)
function imgui.tableSetColumnEnabled(column_n, v)
end

---@param column_n number @(int)
---@return boolean @(bool)
function imgui.tableSetColumnIndex(column_n)
end

---@param label string @(const char*)
---@param flags number @(ImGuiTableColumnFlags)
---@param init_width_or_weight number @(float)
---@param user_data number @(ImGuiID)
---@return nil @(void)
function imgui.tableSetupColumn(label, flags, init_width_or_weight, user_data)
end

---@param label string @(const char*)
---@param flags number @(ImGuiTableColumnFlags)
---@param init_width_or_weight number @(float)
---@return nil @(void)
function imgui.tableSetupColumn(label, flags, init_width_or_weight)
end

---@param label string @(const char*)
---@param flags number @(ImGuiTableColumnFlags)
---@return nil @(void)
function imgui.tableSetupColumn(label, flags)
end

---@param label string @(const char*)
---@return nil @(void)
function imgui.tableSetupColumn(label)
end

---@param cols number @(int)
---@param rows number @(int)
---@return nil @(void)
function imgui.tableSetupScrollFreeze(cols, rows)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.text(fmt)
end

---@param col ImVec4 @(ImVec4)
---@param fmt string @(const char*)
---@return nil @(void)
function imgui.textColored(col, fmt)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.textDisabled(fmt)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.textLink(label)
end

---@param label string @(const char*)
---@param url string @(const char*)
---@return boolean @(bool)
function imgui.textLinkOpenURL(label, url)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.textLinkOpenURL(label)
end

---@param fmt string @(const char*)
---@return nil @(void)
function imgui.textWrapped(fmt)
end

---@param str_id string @(const char*)
---@param fmt string @(const char*)
---@return boolean @(bool)
function imgui.treeNode(str_id, fmt)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.treeNode(label)
end

---@param str_id string @(const char*)
---@param flags number @(ImGuiTreeNodeFlags)
---@param fmt string @(const char*)
---@return boolean @(bool)
function imgui.treeNodeEx(str_id, flags, fmt)
end

---@param label string @(const char*)
---@param flags number @(ImGuiTreeNodeFlags)
---@return boolean @(bool)
function imgui.treeNodeEx(label, flags)
end

---@param label string @(const char*)
---@return boolean @(bool)
function imgui.treeNodeEx(label)
end

---@param storage_id number @(ImGuiID)
---@return boolean @(bool)
function imgui.treeNodeGetOpen(storage_id)
end

---@return nil @(void)
function imgui.treePop()
end

---@param str_id string @(const char*)
---@return nil @(void)
function imgui.treePush(str_id)
end

---@param indent_w number @(float)
---@return nil @(void)
function imgui.unindent(indent_w)
end

---@return nil @(void)
function imgui.unindent()
end

---@return nil @(void)
function imgui.updatePlatformWindows()
end

