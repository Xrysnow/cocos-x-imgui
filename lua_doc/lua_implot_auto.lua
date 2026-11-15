------------------------------
--- Tag: implot
------------------------------

---@class implot
implot = implot or {}
---@class ImPlotPoint
local ImPlotPoint = { x = 0, y = 0 }
---@class ImPlotRange
local ImPlotRange = { min = 0, max = 0 }
---@class ImPlotRect
local ImPlotRect = { x = 0, y = 0 }

---@type number (const int, readonly)
implot.IMPLOT_AUTO = nil

---@type ImVec4 (const ImVec4, readonly)
implot.IMPLOT_AUTO_COL = nil

local ImAxis = {
	X1 = 0,
	X2 = 1,
	X3 = 2,
	Y1 = 3,
	Y2 = 4,
	Y3 = 5,
	COUNT = 6,
}
---
---@class implot.ImAxis
implot.ImAxis = ImAxis

local ImPlotProp = {
	ImPlotProp_LineColor = 0,
	ImPlotProp_LineColors = 1,
	ImPlotProp_LineWeight = 2,
	ImPlotProp_FillColor = 3,
	ImPlotProp_FillColors = 4,
	ImPlotProp_FillAlpha = 5,
	ImPlotProp_Marker = 6,
	ImPlotProp_MarkerSize = 7,
	ImPlotProp_MarkerSizes = 8,
	ImPlotProp_MarkerLineColor = 9,
	ImPlotProp_MarkerLineColors = 10,
	ImPlotProp_MarkerFillColor = 11,
	ImPlotProp_MarkerFillColors = 12,
	ImPlotProp_Size = 13,
	ImPlotProp_Offset = 14,
	ImPlotProp_Stride = 15,
	ImPlotProp_Flags = 16,
}
---
---@class implot.ImPlotProp
implot.ImPlotProp = ImPlotProp

local ImPlotFlags = {
	None = 0,
	NoTitle = 1,
	NoLegend = 2,
	NoMouseText = 4,
	NoInputs = 8,
	NoMenus = 16,
	NoBoxSelect = 32,
	NoFrame = 64,
	Equal = 128,
	Crosshairs = 256,
	CanvasOnly = 55,
}
---
---@class implot.ImPlotFlags
implot.ImPlotFlags = ImPlotFlags

local ImPlotAxisFlags = {
	None = 0,
	NoLabel = 1,
	NoGridLines = 2,
	NoTickMarks = 4,
	NoTickLabels = 8,
	NoInitialFit = 16,
	NoMenus = 32,
	NoSideSwitch = 64,
	NoHighlight = 128,
	Opposite = 256,
	Foreground = 512,
	Invert = 1024,
	AutoFit = 2048,
	RangeFit = 4096,
	PanStretch = 8192,
	LockMin = 16384,
	LockMax = 32768,
	TickLabelsInside = 65536,
	Lock = 49152,
	NoDecorations = 15,
	AuxDefault = 258,
}
---
---@class implot.ImPlotAxisFlags
implot.ImPlotAxisFlags = ImPlotAxisFlags

local ImPlotSubplotFlags = {
	None = 0,
	NoTitle = 1,
	NoLegend = 2,
	NoMenus = 4,
	NoResize = 8,
	NoAlign = 16,
	ShareItems = 32,
	LinkRows = 64,
	LinkCols = 128,
	LinkAllX = 256,
	LinkAllY = 512,
	ColMajor = 1024,
}
---
---@class implot.ImPlotSubplotFlags
implot.ImPlotSubplotFlags = ImPlotSubplotFlags

local ImPlotLegendFlags = {
	None = 0,
	NoButtons = 1,
	NoHighlightItem = 2,
	NoHighlightAxis = 4,
	NoMenus = 8,
	Outside = 16,
	Horizontal = 32,
	Sort = 64,
	Reverse = 128,
}
---
---@class implot.ImPlotLegendFlags
implot.ImPlotLegendFlags = ImPlotLegendFlags

local ImPlotMouseTextFlags = {
	None = 0,
	NoAuxAxes = 1,
	NoFormat = 2,
	ShowAlways = 4,
}
---
---@class implot.ImPlotMouseTextFlags
implot.ImPlotMouseTextFlags = ImPlotMouseTextFlags

local ImPlotDragToolFlags = {
	None = 0,
	NoCursors = 1,
	NoFit = 2,
	NoInputs = 4,
	Delayed = 8,
	NoMove = 16,
	NoResize = 32,
}
---
---@class implot.ImPlotDragToolFlags
implot.ImPlotDragToolFlags = ImPlotDragToolFlags

local ImPlotColormapScaleFlags = {
	None = 0,
	NoLabel = 1,
	Opposite = 2,
	Invert = 4,
}
---
---@class implot.ImPlotColormapScaleFlags
implot.ImPlotColormapScaleFlags = ImPlotColormapScaleFlags

local ImPlotItemFlags = {
	None = 0,
	NoLegend = 1,
	NoFit = 2,
}
---
---@class implot.ImPlotItemFlags
implot.ImPlotItemFlags = ImPlotItemFlags

local ImPlotLineFlags = {
	None = 0,
	Segments = 1024,
	Loop = 2048,
	SkipNaN = 4096,
	NoClip = 8192,
	Shaded = 16384,
}
---
---@class implot.ImPlotLineFlags
implot.ImPlotLineFlags = ImPlotLineFlags

local ImPlotScatterFlags = {
	None = 0,
	NoClip = 1024,
}
---
---@class implot.ImPlotScatterFlags
implot.ImPlotScatterFlags = ImPlotScatterFlags

local ImPlotBubblesFlags = {
	ImPlotBubblesFlags_None = 0,
}
---
---@class implot.ImPlotBubblesFlags
implot.ImPlotBubblesFlags = ImPlotBubblesFlags

local ImPlotPolygonFlags = {
	ImPlotPolygonFlags_None = 0,
	ImPlotPolygonFlags_Concave = 1024,
}
---
---@class implot.ImPlotPolygonFlags
implot.ImPlotPolygonFlags = ImPlotPolygonFlags

local ImPlotStairsFlags = {
	None = 0,
	PreStep = 1024,
	Shaded = 2048,
}
---
---@class implot.ImPlotStairsFlags
implot.ImPlotStairsFlags = ImPlotStairsFlags

local ImPlotShadedFlags = {
	None = 0,
}
---
---@class implot.ImPlotShadedFlags
implot.ImPlotShadedFlags = ImPlotShadedFlags

local ImPlotBarsFlags = {
	None = 0,
	Horizontal = 1024,
}
---
---@class implot.ImPlotBarsFlags
implot.ImPlotBarsFlags = ImPlotBarsFlags

local ImPlotBarGroupsFlags = {
	None = 0,
	Horizontal = 1024,
	Stacked = 2048,
}
---
---@class implot.ImPlotBarGroupsFlags
implot.ImPlotBarGroupsFlags = ImPlotBarGroupsFlags

local ImPlotErrorBarsFlags = {
	None = 0,
	Horizontal = 1024,
}
---
---@class implot.ImPlotErrorBarsFlags
implot.ImPlotErrorBarsFlags = ImPlotErrorBarsFlags

local ImPlotStemsFlags = {
	None = 0,
	Horizontal = 1024,
}
---
---@class implot.ImPlotStemsFlags
implot.ImPlotStemsFlags = ImPlotStemsFlags

local ImPlotInfLinesFlags = {
	None = 0,
	Horizontal = 1024,
}
---
---@class implot.ImPlotInfLinesFlags
implot.ImPlotInfLinesFlags = ImPlotInfLinesFlags

local ImPlotPieChartFlags = {
	None = 0,
	Normalize = 1024,
	IgnoreHidden = 2048,
	Exploding = 4096,
	NoSliceBorder = 8192,
}
---
---@class implot.ImPlotPieChartFlags
implot.ImPlotPieChartFlags = ImPlotPieChartFlags

local ImPlotHeatmapFlags = {
	None = 0,
	ColMajor = 1024,
}
---
---@class implot.ImPlotHeatmapFlags
implot.ImPlotHeatmapFlags = ImPlotHeatmapFlags

local ImPlotHistogramFlags = {
	None = 0,
	Horizontal = 1024,
	Cumulative = 2048,
	Density = 4096,
	NoOutliers = 8192,
	ColMajor = 16384,
}
---
---@class implot.ImPlotHistogramFlags
implot.ImPlotHistogramFlags = ImPlotHistogramFlags

local ImPlotDigitalFlags = {
	None = 0,
}
---
---@class implot.ImPlotDigitalFlags
implot.ImPlotDigitalFlags = ImPlotDigitalFlags

local ImPlotImageFlags = {
	None = 0,
}
---
---@class implot.ImPlotImageFlags
implot.ImPlotImageFlags = ImPlotImageFlags

local ImPlotTextFlags = {
	None = 0,
	Vertical = 1024,
}
---
---@class implot.ImPlotTextFlags
implot.ImPlotTextFlags = ImPlotTextFlags

local ImPlotDummyFlags = {
	None = 0,
}
---
---@class implot.ImPlotDummyFlags
implot.ImPlotDummyFlags = ImPlotDummyFlags

local ImPlotCond = {
	None = 0,
	Always = 1,
	Once = 2,
}
---
---@class implot.ImPlotCond
implot.ImPlotCond = ImPlotCond

local ImPlotCol = {
	FrameBg = 0,
	PlotBg = 1,
	PlotBorder = 2,
	LegendBg = 3,
	LegendBorder = 4,
	LegendText = 5,
	TitleText = 6,
	InlayText = 7,
	AxisText = 8,
	AxisGrid = 9,
	AxisTick = 10,
	AxisBg = 11,
	AxisBgHovered = 12,
	AxisBgActive = 13,
	Selection = 14,
	Crosshairs = 15,
	COUNT = 16,
}
---
---@class implot.ImPlotCol
implot.ImPlotCol = ImPlotCol

local ImPlotStyleVar = {
	PlotDefaultSize = 0,
	PlotMinSize = 1,
	PlotBorderSize = 2,
	MinorAlpha = 3,
	MajorTickLen = 4,
	MinorTickLen = 5,
	MajorTickSize = 6,
	MinorTickSize = 7,
	MajorGridSize = 8,
	MinorGridSize = 9,
	PlotPadding = 10,
	LabelPadding = 11,
	LegendPadding = 12,
	LegendInnerPadding = 13,
	LegendSpacing = 14,
	MousePosPadding = 15,
	AnnotationPadding = 16,
	FitPadding = 17,
	DigitalPadding = 18,
	DigitalSpacing = 19,
	COUNT = 20,
}
---
---@class implot.ImPlotStyleVar
implot.ImPlotStyleVar = ImPlotStyleVar

local ImPlotScale = {
	Linear = 0,
	Time = 1,
	Log10 = 2,
	SymLog = 3,
}
---
---@class implot.ImPlotScale
implot.ImPlotScale = ImPlotScale

local ImPlotMarker = {
	None = -2,
	Auto = -1,
	Circle = 0,
	Square = 1,
	Diamond = 2,
	Up = 3,
	Down = 4,
	Left = 5,
	Right = 6,
	Cross = 7,
	Plus = 8,
	Asterisk = 9,
	Vertical = 10,
	Horizontal = 11,
	COUNT = 12,
}
---
---@class implot.ImPlotMarker
implot.ImPlotMarker = ImPlotMarker

local ImPlotColormap = {
	Deep = 0,
	Dark = 1,
	Pastel = 2,
	Paired = 3,
	Viridis = 4,
	Plasma = 5,
	Hot = 6,
	Cool = 7,
	Pink = 8,
	Jet = 9,
	Twilight = 10,
	RdBu = 11,
	BrBG = 12,
	PiYG = 13,
	Spectral = 14,
	Greys = 15,
}
---
---@class implot.ImPlotColormap
implot.ImPlotColormap = ImPlotColormap

local ImPlotLocation = {
	Center = 0,
	North = 1,
	South = 2,
	West = 4,
	East = 8,
	NorthWest = 5,
	NorthEast = 9,
	SouthWest = 6,
	SouthEast = 10,
}
---
---@class implot.ImPlotLocation
implot.ImPlotLocation = ImPlotLocation

local ImPlotBin = {
	Sqrt = -1,
	Sturges = -2,
	Rice = -3,
	Scott = -4,
}
---
---@class implot.ImPlotBin
implot.ImPlotBin = ImPlotBin


---@class implot.ImPlotStyle
local ImPlotStyle = {}
implot.ImPlotStyle = ImPlotStyle

---@type ImVec2 (ImVec2)
ImPlotStyle.AnnotationPadding = nil

---@type ImPlotColormap (ImPlotColormap)
ImPlotStyle.Colormap = nil

---@type number (float)
ImPlotStyle.DigitalPadding = nil

---@type number (float)
ImPlotStyle.DigitalSpacing = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.FitPadding = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.LabelPadding = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.LegendInnerPadding = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.LegendPadding = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.LegendSpacing = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MajorGridSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MajorTickLen = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MajorTickSize = nil

---@type number (float)
ImPlotStyle.MinorAlpha = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MinorGridSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MinorTickLen = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MinorTickSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.MousePosPadding = nil

---@type number (float)
ImPlotStyle.PlotBorderSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.PlotDefaultSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.PlotMinSize = nil

---@type ImVec2 (ImVec2)
ImPlotStyle.PlotPadding = nil

---@type boolean (bool)
ImPlotStyle.Use24HourClock = nil

---@type boolean (bool)
ImPlotStyle.UseISO8601 = nil

---@type boolean (bool)
ImPlotStyle.UseLocalTime = nil


---@class implot.ImPlotInputMap
local ImPlotInputMap = {}
implot.ImPlotInputMap = ImPlotInputMap

---@type ImGuiMouseButton (ImGuiMouseButton)
ImPlotInputMap.Fit = nil

---@type ImGuiMouseButton (ImGuiMouseButton)
ImPlotInputMap.Menu = nil

---@type number (int)
ImPlotInputMap.OverrideMod = nil

---@type ImGuiMouseButton (ImGuiMouseButton)
ImPlotInputMap.Pan = nil

---@type number (int)
ImPlotInputMap.PanMod = nil

---@type ImGuiMouseButton (ImGuiMouseButton)
ImPlotInputMap.Select = nil

---@type ImGuiMouseButton (ImGuiMouseButton)
ImPlotInputMap.SelectCancel = nil

---@type number (int)
ImPlotInputMap.SelectHorzMod = nil

---@type number (int)
ImPlotInputMap.SelectMod = nil

---@type number (int)
ImPlotInputMap.SelectVertMod = nil

---@type number (int)
ImPlotInputMap.ZoomMod = nil

---@type number (float)
ImPlotInputMap.ZoomRate = nil

---@param group_id string @(const char*)
---@param vertical boolean @(bool)
---@return boolean @(bool)
function implot.beginAlignedPlots(group_id, vertical)
end

---@param group_id string @(const char*)
---@return boolean @(bool)
function implot.beginAlignedPlots(group_id)
end

---@param axis number @(ImAxis)
---@param flags number @(ImGuiDragDropFlags)
---@return boolean @(bool)
function implot.beginDragDropSourceAxis(axis, flags)
end

---@param axis number @(ImAxis)
---@return boolean @(bool)
function implot.beginDragDropSourceAxis(axis)
end

---@param label_id string @(const char*)
---@param flags number @(ImGuiDragDropFlags)
---@return boolean @(bool)
function implot.beginDragDropSourceItem(label_id, flags)
end

---@param label_id string @(const char*)
---@return boolean @(bool)
function implot.beginDragDropSourceItem(label_id)
end

---@param flags number @(ImGuiDragDropFlags)
---@return boolean @(bool)
function implot.beginDragDropSourcePlot(flags)
end

---@return boolean @(bool)
function implot.beginDragDropSourcePlot()
end

---@param axis number @(ImAxis)
---@return boolean @(bool)
function implot.beginDragDropTargetAxis(axis)
end

---@return boolean @(bool)
function implot.beginDragDropTargetLegend()
end

---@return boolean @(bool)
function implot.beginDragDropTargetPlot()
end

---@param label_id string @(const char*)
---@param mouse_button number @(ImGuiMouseButton)
---@return boolean @(bool)
function implot.beginLegendPopup(label_id, mouse_button)
end

---@param label_id string @(const char*)
---@return boolean @(bool)
function implot.beginLegendPopup(label_id)
end

---@param title_id string @(const char*)
---@param size ImVec2 @(ImVec2)
---@param flags number @(ImPlotFlags)
---@return boolean @(bool)
function implot.beginPlot(title_id, size, flags)
end

---@param title_id string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function implot.beginPlot(title_id, size)
end

---@param title_id string @(const char*)
---@return boolean @(bool)
function implot.beginPlot(title_id)
end

---@return nil @(void)
function implot.cancelPlotSelection()
end

---@param label string @(const char*)
---@param size ImVec2 @(ImVec2)
---@param cmap number @(ImPlotColormap)
---@return boolean @(bool)
function implot.colormapButton(label, size, cmap)
end

---@param label string @(const char*)
---@param size ImVec2 @(ImVec2)
---@return boolean @(bool)
function implot.colormapButton(label, size)
end

---@param label string @(const char*)
---@return boolean @(bool)
function implot.colormapButton(label)
end

---@param cmap number @(ImPlotColormap)
---@return nil @(void)
function implot.colormapIcon(cmap)
end

---@param label string @(const char*)
---@param scale_min number @(double)
---@param scale_max number @(double)
---@param size ImVec2 @(ImVec2)
---@param format string @(const char*)
---@param flags number @(ImPlotColormapScaleFlags)
---@param cmap number @(ImPlotColormap)
---@return nil @(void)
function implot.colormapScale(label, scale_min, scale_max, size, format, flags, cmap)
end

---@param label string @(const char*)
---@param scale_min number @(double)
---@param scale_max number @(double)
---@param size ImVec2 @(ImVec2)
---@param format string @(const char*)
---@param flags number @(ImPlotColormapScaleFlags)
---@return nil @(void)
function implot.colormapScale(label, scale_min, scale_max, size, format, flags)
end

---@param label string @(const char*)
---@param scale_min number @(double)
---@param scale_max number @(double)
---@param size ImVec2 @(ImVec2)
---@param format string @(const char*)
---@return nil @(void)
function implot.colormapScale(label, scale_min, scale_max, size, format)
end

---@param label string @(const char*)
---@param scale_min number @(double)
---@param scale_max number @(double)
---@param size ImVec2 @(ImVec2)
---@return nil @(void)
function implot.colormapScale(label, scale_min, scale_max, size)
end

---@param label string @(const char*)
---@param scale_min number @(double)
---@param scale_max number @(double)
---@return nil @(void)
function implot.colormapScale(label, scale_min, scale_max)
end

---@return nil @(void)
function implot.endAlignedPlots()
end

---@return nil @(void)
function implot.endDragDropSource()
end

---@return nil @(void)
function implot.endDragDropTarget()
end

---@return nil @(void)
function implot.endLegendPopup()
end

---@return nil @(void)
function implot.endPlot()
end

---@return nil @(void)
function implot.endSubplots()
end

---@param idx number @(int)
---@param cmap number @(ImPlotColormap)
---@return ImVec4 @(ImVec4)
function implot.getColormapColor(idx, cmap)
end

---@param idx number @(int)
---@return ImVec4 @(ImVec4)
function implot.getColormapColor(idx)
end

---@return number @(int)
function implot.getColormapCount()
end

---@param name string @(const char*)
---@return number @(ImPlotColormap)
function implot.getColormapIndex(name)
end

---@param cmap number @(ImPlotColormap)
---@return string @(const char*)
function implot.getColormapName(cmap)
end

---@param cmap number @(ImPlotColormap)
---@return number @(int)
function implot.getColormapSize(cmap)
end

---@return number @(int)
function implot.getColormapSize()
end

---@return ImVec4 @(ImVec4)
function implot.getLastItemColor()
end

---@param idx number @(ImPlotMarker)
---@return string @(const char*)
function implot.getMarkerName(idx)
end

---@return ImDrawList @(ImDrawList*)
function implot.getPlotDrawList()
end

---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImPlotRect @(ImPlotRect)
function implot.getPlotLimits(x_axis, y_axis)
end

---@param x_axis number @(ImAxis)
---@return ImPlotRect @(ImPlotRect)
function implot.getPlotLimits(x_axis)
end

---@return ImPlotRect @(ImPlotRect)
function implot.getPlotLimits()
end

---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.getPlotMousePos(x_axis, y_axis)
end

---@param x_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.getPlotMousePos(x_axis)
end

---@return ImPlotPoint @(ImPlotPoint)
function implot.getPlotMousePos()
end

---@return ImVec2 @(ImVec2)
function implot.getPlotPos()
end

---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImPlotRect @(ImPlotRect)
function implot.getPlotSelection(x_axis, y_axis)
end

---@param x_axis number @(ImAxis)
---@return ImPlotRect @(ImPlotRect)
function implot.getPlotSelection(x_axis)
end

---@return ImPlotRect @(ImPlotRect)
function implot.getPlotSelection()
end

---@return ImVec2 @(ImVec2)
function implot.getPlotSize()
end

---@param idx number @(ImPlotCol)
---@return string @(const char*)
function implot.getStyleColorName(idx)
end

---@param hidden boolean @(bool)
---@param cond number @(ImPlotCond)
---@return nil @(void)
function implot.hideNextItem(hidden, cond)
end

---@param hidden boolean @(bool)
---@return nil @(void)
function implot.hideNextItem(hidden)
end

---@return nil @(void)
function implot.hideNextItem()
end

---@param axis number @(ImAxis)
---@return boolean @(bool)
function implot.isAxisHovered(axis)
end

---@param label_id string @(const char*)
---@return boolean @(bool)
function implot.isItemHidden(label_id)
end

---@return boolean @(bool)
function implot.isLastItemHidden()
end

---@param label_id string @(const char*)
---@return boolean @(bool)
function implot.isLegendEntryHovered(label_id)
end

---@return boolean @(bool)
function implot.isPlotHovered()
end

---@return boolean @(bool)
function implot.isPlotSelected()
end

---@return boolean @(bool)
function implot.isSubplotsHovered()
end

---@return ImVec4 @(ImVec4)
function implot.nextColormapColor()
end

---@return number @(ImPlotMarker)
function implot.nextMarker()
end

---@param x number @(float)
---@param y number @(float)
---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(x, y, x_axis, y_axis)
end

---@param x number @(float)
---@param y number @(float)
---@param x_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(x, y, x_axis)
end

---@param pix ImVec2 @(ImVec2)
---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(pix, x_axis, y_axis)
end

---@param x number @(float)
---@param y number @(float)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(x, y)
end

---@param pix ImVec2 @(ImVec2)
---@param x_axis number @(ImAxis)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(pix, x_axis)
end

---@param pix ImVec2 @(ImVec2)
---@return ImPlotPoint @(ImPlotPoint)
function implot.pixelsToPlot(pix)
end

---@param x number @(double)
---@param y number @(double)
---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(x, y, x_axis, y_axis)
end

---@param x number @(double)
---@param y number @(double)
---@param x_axis number @(ImAxis)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(x, y, x_axis)
end

---@param plt ImPlotPoint @(ImPlotPoint)
---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(plt, x_axis, y_axis)
end

---@param x number @(double)
---@param y number @(double)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(x, y)
end

---@param plt ImPlotPoint @(ImPlotPoint)
---@param x_axis number @(ImAxis)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(plt, x_axis)
end

---@param plt ImPlotPoint @(ImPlotPoint)
---@return ImVec2 @(ImVec2)
function implot.plotToPixels(plt)
end

---@param count number @(int)
---@return nil @(void)
function implot.popColormap(count)
end

---@return nil @(void)
function implot.popColormap()
end

---@return nil @(void)
function implot.popPlotClipRect()
end

---@param count number @(int)
---@return nil @(void)
function implot.popStyleColor(count)
end

---@return nil @(void)
function implot.popStyleColor()
end

---@param count number @(int)
---@return nil @(void)
function implot.popStyleVar(count)
end

---@return nil @(void)
function implot.popStyleVar()
end

---@param expand number @(float)
---@return nil @(void)
function implot.pushPlotClipRect(expand)
end

---@return nil @(void)
function implot.pushPlotClipRect()
end

---@param t number @(float)
---@param cmap number @(ImPlotColormap)
---@return ImVec4 @(ImVec4)
function implot.sampleColormap(t, cmap)
end

---@param t number @(float)
---@return ImVec4 @(ImVec4)
function implot.sampleColormap(t)
end

---@param x_axis number @(ImAxis)
---@param y_axis number @(ImAxis)
---@return nil @(void)
function implot.setAxes(x_axis, y_axis)
end

---@param axis number @(ImAxis)
---@return nil @(void)
function implot.setAxis(axis)
end

---@param x_min number @(double)
---@param x_max number @(double)
---@param y_min number @(double)
---@param y_max number @(double)
---@param cond number @(ImPlotCond)
---@return nil @(void)
function implot.setNextAxesLimits(x_min, x_max, y_min, y_max, cond)
end

---@param x_min number @(double)
---@param x_max number @(double)
---@param y_min number @(double)
---@param y_max number @(double)
---@return nil @(void)
function implot.setNextAxesLimits(x_min, x_max, y_min, y_max)
end

---@return nil @(void)
function implot.setNextAxesToFit()
end

---@param axis number @(ImAxis)
---@param v_min number @(double)
---@param v_max number @(double)
---@param cond number @(ImPlotCond)
---@return nil @(void)
function implot.setNextAxisLimits(axis, v_min, v_max, cond)
end

---@param axis number @(ImAxis)
---@param v_min number @(double)
---@param v_max number @(double)
---@return nil @(void)
function implot.setNextAxisLimits(axis, v_min, v_max)
end

---@param axis number @(ImAxis)
---@return nil @(void)
function implot.setNextAxisToFit(axis)
end

---@param x_label string @(const char*)
---@param y_label string @(const char*)
---@param x_flags number @(ImPlotAxisFlags)
---@param y_flags number @(ImPlotAxisFlags)
---@return nil @(void)
function implot.setupAxes(x_label, y_label, x_flags, y_flags)
end

---@param x_label string @(const char*)
---@param y_label string @(const char*)
---@param x_flags number @(ImPlotAxisFlags)
---@return nil @(void)
function implot.setupAxes(x_label, y_label, x_flags)
end

---@param x_label string @(const char*)
---@param y_label string @(const char*)
---@return nil @(void)
function implot.setupAxes(x_label, y_label)
end

---@param x_min number @(double)
---@param x_max number @(double)
---@param y_min number @(double)
---@param y_max number @(double)
---@param cond number @(ImPlotCond)
---@return nil @(void)
function implot.setupAxesLimits(x_min, x_max, y_min, y_max, cond)
end

---@param x_min number @(double)
---@param x_max number @(double)
---@param y_min number @(double)
---@param y_max number @(double)
---@return nil @(void)
function implot.setupAxesLimits(x_min, x_max, y_min, y_max)
end

---@param axis number @(ImAxis)
---@param label string @(const char*)
---@param flags number @(ImPlotAxisFlags)
---@return nil @(void)
function implot.setupAxis(axis, label, flags)
end

---@param axis number @(ImAxis)
---@param label string @(const char*)
---@return nil @(void)
function implot.setupAxis(axis, label)
end

---@param axis number @(ImAxis)
---@return nil @(void)
function implot.setupAxis(axis)
end

---@param axis number @(ImAxis)
---@param fmt string @(const char*)
---@return nil @(void)
function implot.setupAxisFormat(axis, fmt)
end

---@param axis number @(ImAxis)
---@param v_min number @(double)
---@param v_max number @(double)
---@param cond number @(ImPlotCond)
---@return nil @(void)
function implot.setupAxisLimits(axis, v_min, v_max, cond)
end

---@param axis number @(ImAxis)
---@param v_min number @(double)
---@param v_max number @(double)
---@return nil @(void)
function implot.setupAxisLimits(axis, v_min, v_max)
end

---@param axis number @(ImAxis)
---@param v_min number @(double)
---@param v_max number @(double)
---@return nil @(void)
function implot.setupAxisLimitsConstraints(axis, v_min, v_max)
end

---@param axis number @(ImAxis)
---@param scale number @(ImPlotScale)
---@return nil @(void)
function implot.setupAxisScale(axis, scale)
end

---@param axis number @(ImAxis)
---@param z_min number @(double)
---@param z_max number @(double)
---@return nil @(void)
function implot.setupAxisZoomConstraints(axis, z_min, z_max)
end

---@return nil @(void)
function implot.setupFinish()
end

---@param location number @(ImPlotLocation)
---@param flags number @(ImPlotLegendFlags)
---@return nil @(void)
function implot.setupLegend(location, flags)
end

---@param location number @(ImPlotLocation)
---@return nil @(void)
function implot.setupLegend(location)
end

---@param location number @(ImPlotLocation)
---@param flags number @(ImPlotMouseTextFlags)
---@return nil @(void)
function implot.setupMouseText(location, flags)
end

---@param location number @(ImPlotLocation)
---@return nil @(void)
function implot.setupMouseText(location)
end

---@param label string @(const char*)
---@return boolean @(bool)
function implot.showColormapSelector(label)
end

---@param label string @(const char*)
---@return boolean @(bool)
function implot.showInputMapSelector(label)
end

---@param ref ImPlotStyle @(ImPlotStyle*)
---@return nil @(void)
function implot.showStyleEditor(ref)
end

---@return nil @(void)
function implot.showStyleEditor()
end

---@param label string @(const char*)
---@return boolean @(bool)
function implot.showStyleSelector(label)
end

---@return nil @(void)
function implot.showUserGuide()
end

---@param dst ImPlotStyle @(ImPlotStyle*)
---@return nil @(void)
function implot.styleColorsAuto(dst)
end

---@return nil @(void)
function implot.styleColorsAuto()
end

---@param dst ImPlotStyle @(ImPlotStyle*)
---@return nil @(void)
function implot.styleColorsClassic(dst)
end

---@return nil @(void)
function implot.styleColorsClassic()
end

---@param dst ImPlotStyle @(ImPlotStyle*)
---@return nil @(void)
function implot.styleColorsDark(dst)
end

---@return nil @(void)
function implot.styleColorsDark()
end

---@param dst ImPlotStyle @(ImPlotStyle*)
---@return nil @(void)
function implot.styleColorsLight(dst)
end

---@return nil @(void)
function implot.styleColorsLight()
end

