--
implot = implot or {}

-------------------------------------------------------------------------------
-- Begin/End Plot
-------------------------------------------------------------------------------

--- Starts a 2D plotting context. If this function returns true, EndPlot() must
--- be called. title_id must be unique; use ## suffixes to disambiguate hidden IDs.
--- A false result discards queued axis links and releases their retained buffers.
---@param title_id string
---@param size ImVec2 @optional
---@param flags number @optional
---@return boolean
function implot.beginPlot(title_id, size, flags)
end

--- Starts a subplot grid; call endSubplots() only when the first result is true.
--- rows/cols must be positive integers. Optional ratio arrays must contain at least
--- rows/cols finite positive float values; extra elements/keys are ignored.
--- Returns shown, row_ratios, col_ratios as fresh snapshots (nil for omitted arrays).
--- Input tables are not modified. Keep the returned arrays for the next frame.
--- Native resizing writes ratios during beginSubplots(); no pointer survives this call.
---@param title_id string
---@param rows integer
---@param cols integer
---@param size ImVec2
---@param flags? implot.ImPlotSubplotFlags Defaults to 0.
---@param row_ratios? number[]
---@param col_ratios? number[]
---@return boolean shown
---@return number[]|nil row_ratios
---@return number[]|nil col_ratios
function implot.beginSubplots(title_id, rows, cols, size, flags, row_ratios, col_ratios)
end

--- Link an enabled axis during setup, after successful beginPlot() and before plotting/setupFinish().
--- Each non-nil link must be a writable ffi.new('double[1]', value) with a finite value.
--- Ordinary numbers, tables, raw double* pointers and other cdata types are rejected.
--- Values are read immediately; endPlot() writes the final axis limits back to link[0].
--- nil disconnects that endpoint; two nil endpoints disconnect both. This function returns no values.
--- Buffers are retained by the binding until endPlot(), then detached from cached native plots.
--- Register links for every plot submission; reuse the same buffers to share limits across plots/frames.
--- Use the Lua beginPlot()/endPlot() pair for cleanup and finish plotting before closing the Lua state.
---@param axis implot.ImAxis
---@param link_min? ffi.cdata Writable double[1], or nil for no lower link.
---@param link_max? ffi.cdata Writable double[1], or nil for no upper link.
function implot.setupAxisLinks(axis, link_min, link_max)
end

--- Queue links before beginPlot(); uses the same double[1] reference contract as setupAxisLinks().
--- The next beginPlot() reads the buffers; successful plots write them back at endPlot().
--- Buffers remain retained while pending. Failed beginPlot() releases them without a writeback.
--- Native next-plot links apply only to axes enabled at BeginPlot; for secondary axes call
--- setupAxis() followed by setupAxisLinks() instead. Repeat for each plot submission.
---@param axis implot.ImAxis
---@param link_min? ffi.cdata Writable double[1], or nil for no lower link.
---@param link_max? ffi.cdata Writable double[1], or nil for no upper link.
function implot.setNextAxisLinks(axis, link_min, link_max)
end

-- Example (retain lo/hi yourself to reuse them across frames):
-- local ffi = require('ffi')
-- local lo, hi = ffi.new('double[1]', 0), ffi.new('double[1]', 10)
-- implot.setNextAxisLinks(implot.ImAxis.X1, lo, hi)
-- if implot.beginPlot('Linked plot') then
--     -- Submit plot items here.
--     implot.endPlot()
-- end
-- Read lo[0], hi[0] after endPlot(); panning/zooming updates these cells.

-------------------------------------------------------------------------------
-- Plot Items
-------------------------------------------------------------------------------

-- Lua-array variants copy numeric data to packed double arrays. Counts are non-negative
-- integers and are clamped to the shortest input (except dimensional plots, which require
-- complete data). Empty histograms return 0. No data pointer is retained after a call.
-- C variants require a 1D LuaJIT numeric array or typed pointer matching ImGuiDataType:
-- S8/U8/S16/U16/S32/U32/S64/U64 use int8_t/uint8_t/.../int64_t/uint64_t; Float/Double
-- use float/double. const input is allowed. Scalars, void*, pointer-to-pointer,
-- multidimensional/struct arrays, numeric addresses and lightuserdata are rejected.
-- All builds validate type, non-null address, alignment, stride and address/span overflow.
-- Fixed and variable-length FFI arrays also validate byte capacity against
-- (count - 1) * stride + sizeof(T); BarGroups uses item_count * group_count, Heatmap
-- rows * cols. Circular Offset does not reduce the required storage span.
-- Typed pointers carry no allocation size: callers must guarantee readable capacity and
-- retain the backing allocation until PlotX returns. Casting to T* does not prove that
-- the allocation holds T values; there is no ownership/lifetime check or pointer retention.
-- Data buffers are read-only to this binding; no writeback. Use an owning ffi.new('double[?]', n)
-- array when possible so capacity can be checked, rather than casting it to double*.
-- Plotting converts values to double; very large 64-bit integers may lose plot precision.
-- All variants accept a final spec table, or an integer shorthand for spec.Flags.
-- Line/Scatter/Bubbles/Polygon/Stairs/Shaded/Bars/ErrorBars/Stems/InfLines/Digital C variants also accept
-- the legacy tail flags, offset, stride instead of spec. Stems now consistently includes
-- offset before stride; InfLines and Digital now honor stride too.
-- Array specs only allow Stride = -1 (auto) or sizeof(double) = 8. C specs allow auto or
-- a positive stride >= sizeof(T), divisible by sizeof(T). Offset is a circular index.
-- Use spec.Flags for horizontal, stacked, normalized, cumulative and density modes.
-- Spec arrays copy consecutive raw indices from 1, silently stopping at the first nil
-- and ignoring extra keys. Temporary buffers remain alive until PlotX returns;
-- cdata is not accepted for these fields.
-- Colors are packed ImGui uint32 values (use imgui.colorConvertFloat4ToU32), not ImVec4 arrays.
-- MarkerSizes are finite non-negative float radii in pixels, independent of the coordinate dtype.
-- nil/missing arrays use the corresponding scalar field. Arrays do not enable markers.
-- Arrays use drawing order and contiguous storage; spec.Offset/Stride only affect plot data.
-- Supply at least the effective point count, group_count for BarGroups (shared by every
-- series), or the final bin count for Histogram (including automatic binning).
-- Line/Fill colors apply to Line/Stairs (Fill requires Shaded), Bubbles and Bars;
-- Shaded uses Fill, Stems/InfLines use Line, and Line/Stairs/Scatter/Stems use marker arrays.
-- Other PlotX implementations currently ignore these arrays. Line segments use one flat
-- color per segment; Segments mode reads Lua color indices 1,3,5,..., not consecutive pairs.
-- CC_DEBUG >= 1 checks element ranges and the truncated prefix's capacity for supported fields.
-- An insufficient prefix still raises a capacity error. Other builds require callers
-- to supply a long enough prefix with valid values, without those strict checks.
-- Arrays are never repeated/padded and do not reduce the plot count; no values are written back.

---@class implot.ImPlotSpec
---@field LineColor ImVec4|nil
---@field LineColors integer[]|nil Packed uint32 colors in drawing order.
---@field LineWeight number|nil
---@field FillColor ImVec4|nil
---@field FillColors integer[]|nil Packed uint32 colors, multiplied by FillAlpha.
---@field FillAlpha number|nil
---@field Marker integer|nil
---@field MarkerSize number|nil
---@field MarkerSizes number[]|nil Non-negative float radii in pixels.
---@field MarkerLineColor ImVec4|nil
---@field MarkerLineColors integer[]|nil Packed uint32 marker edge colors.
---@field MarkerFillColor ImVec4|nil
---@field MarkerFillColors integer[]|nil Packed uint32 marker face colors, multiplied by FillAlpha.
---@field Size number|nil
---@field Offset integer|nil
---@field Stride integer|nil
---@field Flags integer|nil
local ImPlotSpec

--- Plot a line; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param values number[]
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotLine(label_id, values, count, xscale, xstart, spec)
end

--- Plot a line; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotLineC(label_id, dtype, values, count, xscale, xstart, spec, offset, stride)
end

--- Plot a line from paired coordinates.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotLine(label_id, xs, ys, count, spec)
end

--- Plot a line from paired coordinates.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotLineC(label_id, dtype, xs, ys, count, spec, offset, stride)
end

--- Plot markers; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param values number[]
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotScatter(label_id, values, count, xscale, xstart, spec)
end

--- Plot markers; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotScatterC(label_id, dtype, values, count, xscale, xstart, spec, offset, stride)
end

--- Plot markers from paired coordinates.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotScatter(label_id, xs, ys, count, spec)
end

--- Plot markers from paired coordinates.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotScatterC(label_id, dtype, xs, ys, count, spec, offset, stride)
end

--- Plot bubbles; radii are in plot units. xscale defaults to 1, xstart to 0.
--- Count is clamped to the shortest input array; zero count submits no item.
---@param label_id string
---@param values number[]
---@param radii number[]
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotBubbles(label_id, values, radii, count, xscale, xstart, spec)
end

--- Plot bubbles; radii are in plot units. xscale defaults to 1, xstart to 0.
--- All buffers use dtype and the same byte stride; caller must provide sufficient live storage.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param radii ffi.cdata
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotBubblesC(label_id, dtype, values, radii, count, xscale, xstart, spec, offset, stride)
end

--- Plot bubbles from paired coordinates; radii are in plot units.
--- Count is clamped to the shortest input array; zero count submits no item.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param radii number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotBubbles(label_id, xs, ys, radii, count, spec)
end

--- Plot bubbles from paired coordinates; radii are in plot units.
--- All buffers use dtype and the same byte stride; caller must provide sufficient live storage.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param radii ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotBubblesC(label_id, dtype, xs, ys, radii, count, spec, offset, stride)
end

--- Plot a closed polygon with counter-clockwise vertices.
--- For concave shapes set spec.Flags = implot.ImPlotPolygonFlags.ImPlotPolygonFlags_Concave.
--- Count is clamped to the shortest input array; zero count submits no item.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotPolygon(label_id, xs, ys, count, spec)
end

--- Plot a closed polygon with counter-clockwise vertices.
--- For concave shapes set spec.Flags = implot.ImPlotPolygonFlags.ImPlotPolygonFlags_Concave.
--- All buffers use dtype and the same byte stride; caller must provide sufficient live storage.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotPolygonC(label_id, dtype, xs, ys, count, spec, offset, stride)
end

--- Plot stairs; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param values number[]
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotStairs(label_id, values, count, xscale, xstart, spec)
end

--- Plot stairs; xscale defaults to 1, xstart to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotStairsC(label_id, dtype, values, count, xscale, xstart, spec, offset, stride)
end

--- Plot stairs from paired coordinates.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotStairs(label_id, xs, ys, count, spec)
end

--- Plot stairs from paired coordinates.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotStairsC(label_id, dtype, xs, ys, count, spec, offset, stride)
end

--- Shade to yref (default 0); xscale defaults to 1, xstart to 0.
---@param label_id string
---@param values number[]
---@param count integer
---@param yref? number
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotShaded(label_id, values, count, yref, xscale, xstart, spec)
end

--- Shade to yref (default 0); xscale defaults to 1, xstart to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param yref? number
---@param xscale? number
---@param xstart? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotShadedC(label_id, dtype, values, count, yref, xscale, xstart, spec, offset, stride)
end

--- Shade paired coordinates to yref (default 0).
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param yref? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotShaded(label_id, xs, ys, count, yref, spec)
end

--- Shade paired coordinates to yref (default 0).
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param yref? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotShadedC(label_id, dtype, xs, ys, count, yref, spec, offset, stride)
end

--- Shade between two curves.
---@param label_id string
---@param xs number[]
---@param ys1 number[]
---@param ys2 number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotShaded(label_id, xs, ys1, ys2, count, spec)
end

--- Shade between two curves.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys1 ffi.cdata
---@param ys2 ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotShadedC(label_id, dtype, xs, ys1, ys2, count, spec, offset, stride)
end

--- Plot bars; bar_size defaults to 0.67, shift to 0.
---@param label_id string
---@param values number[]
---@param count integer
---@param bar_size? number
---@param shift? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotBars(label_id, values, count, bar_size, shift, spec)
end

--- Plot bars; bar_size defaults to 0.67, shift to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param bar_size? number
---@param shift? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotBarsC(label_id, dtype, values, count, bar_size, shift, spec, offset, stride)
end

--- Plot bars from paired coordinates.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param bar_size number
---@param spec? implot.ImPlotSpec|integer
function implot.plotBars(label_id, xs, ys, count, bar_size, spec)
end

--- Plot bars from paired coordinates.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param bar_size number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotBarsC(label_id, dtype, xs, ys, count, bar_size, spec, offset, stride)
end

--- Requires item_count labels and item_count * group_count values; group_size defaults to 0.67, shift to 0.
---@param label_ids string[]
---@param values number[]
---@param item_count integer
---@param group_count integer
---@param group_size? number
---@param shift? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotBarGroups(label_ids, values, item_count, group_count, group_size, shift, spec)
end

--- Requires item_count labels and item_count * group_count values; group_size defaults to 0.67, shift to 0.
---@param label_ids string[]
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param item_count integer
---@param group_count integer
---@param group_size? number
---@param shift? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotBarGroupsC(label_ids, dtype, values, item_count, group_count, group_size, shift, spec)
end

--- Plot symmetric error bars.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param err number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotErrorBars(label_id, xs, ys, err, count, spec)
end

--- Plot symmetric error bars.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param err ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotErrorBarsC(label_id, dtype, xs, ys, err, count, spec, offset, stride)
end

--- Plot asymmetric error bars.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param neg number[]
---@param pos number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotErrorBars(label_id, xs, ys, neg, pos, count, spec)
end

--- Plot asymmetric error bars.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param neg ffi.cdata
---@param pos ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotErrorBarsC(label_id, dtype, xs, ys, neg, pos, count, spec, offset, stride)
end

--- Plot stems; ref/start default to 0, scale to 1.
---@param label_id string
---@param values number[]
---@param count integer
---@param ref? number
---@param scale? number
---@param start? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotStems(label_id, values, count, ref, scale, start, spec)
end

--- Plot stems; ref/start default to 0, scale to 1.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param ref? number
---@param scale? number
---@param start? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotStemsC(label_id, dtype, values, count, ref, scale, start, spec, offset, stride)
end

--- Plot stems from paired coordinates; ref defaults to 0.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param ref? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotStems(label_id, xs, ys, count, ref, spec)
end

--- Plot stems from paired coordinates; ref defaults to 0.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param ref? number
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotStemsC(label_id, dtype, xs, ys, count, ref, spec, offset, stride)
end

--- Plot infinite lines; use Flags for horizontal lines.
---@param label_id string
---@param values number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotInfLines(label_id, values, count, spec)
end

--- Plot infinite lines; use Flags for horizontal lines.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotInfLinesC(label_id, dtype, values, count, spec, offset, stride)
end

--- Plot a pie chart; label_fmt defaults to "%.1f", angle0 to 90. Count is also clamped to labels.
---@param label_ids string[]
---@param values number[]
---@param count integer
---@param x number
---@param y number
---@param radius number
---@param label_fmt? string
---@param angle0? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotPieChart(label_ids, values, count, x, y, radius, label_fmt, angle0, spec)
end

--- Plot a pie chart; label_fmt defaults to "%.1f", angle0 to 90. Count is also clamped to labels.
---@param label_ids string[]
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param x number
---@param y number
---@param radius number
---@param label_fmt? string
---@param angle0? number
---@param spec? implot.ImPlotSpec|integer
function implot.plotPieChartC(label_ids, dtype, values, count, x, y, radius, label_fmt, angle0, spec)
end

--- Requires rows * cols values. Scale defaults to auto (0, 0), format to "%.1f", bounds to (0, 0) / (1, 1).
---@param label_id string
---@param values number[]
---@param rows integer
---@param cols integer
---@param scale_min? number
---@param scale_max? number
---@param label_fmt? string
---@param bounds_min? implot.ImPlotPoint
---@param bounds_max? implot.ImPlotPoint
---@param spec? implot.ImPlotSpec|integer
function implot.plotHeatmap(label_id, values, rows, cols, scale_min, scale_max, label_fmt, bounds_min, bounds_max, spec)
end

--- Requires rows * cols values. Scale defaults to auto (0, 0), format to "%.1f", bounds to (0, 0) / (1, 1).
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param rows integer
---@param cols integer
---@param scale_min? number
---@param scale_max? number
---@param label_fmt? string
---@param bounds_min? implot.ImPlotPoint
---@param bounds_max? implot.ImPlotPoint
---@param spec? implot.ImPlotSpec|integer
function implot.plotHeatmapC(label_id, dtype, values, rows, cols, scale_min, scale_max, label_fmt, bounds_min, bounds_max, spec)
end

--- Return the largest bin count/density. Defaults: Sturges bins, scale 1, automatic range.
---@param label_id string
---@param values number[]
---@param count integer
---@param bins? number
---@param bar_scale? number
---@param range? implot.ImPlotRange
---@param spec? implot.ImPlotSpec|integer
---@return number
function implot.plotHistogram(label_id, values, count, bins, bar_scale, range, spec)
end

--- Return the largest bin count/density. Defaults: Sturges bins, scale 1, automatic range.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param values ffi.cdata
---@param count integer
---@param bins? number
---@param bar_scale? number
---@param range? implot.ImPlotRange
---@param spec? implot.ImPlotSpec|integer
---@return number
function implot.plotHistogramC(label_id, dtype, values, count, bins, bar_scale, range, spec)
end

--- Return the largest bin count/density. Defaults: Sturges bins and automatic range.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param x_bins? number
---@param y_bins? number
---@param range? implot.ImPlotRect
---@param spec? implot.ImPlotSpec|integer
---@return number
function implot.plotHistogram2D(label_id, xs, ys, count, x_bins, y_bins, range, spec)
end

--- Return the largest bin count/density. Defaults: Sturges bins and automatic range.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param x_bins? number
---@param y_bins? number
---@param range? implot.ImPlotRect
---@param spec? implot.ImPlotSpec|integer
---@return number
function implot.plotHistogram2DC(label_id, dtype, xs, ys, count, x_bins, y_bins, range, spec)
end

--- Plot digital signals.
---@param label_id string
---@param xs number[]
---@param ys number[]
---@param count integer
---@param spec? implot.ImPlotSpec|integer
function implot.plotDigital(label_id, xs, ys, count, spec)
end

--- Plot digital signals.
---@param label_id string
---@param dtype imgui.ImGuiDataType
---@param xs ffi.cdata
---@param ys ffi.cdata
---@param count integer
---@param spec? implot.ImPlotSpec|integer
---@param offset? integer Legacy flags form only; default 0.
---@param stride? integer Legacy flags form only; default sizeof(T).
function implot.plotDigitalC(label_id, dtype, xs, ys, count, spec, offset, stride)
end

--- Plots text with the shared spec conversion (including flags shorthand).
---@param text string
---@param x number
---@param y number
---@param pix_offset? ImVec2 Defaults to (0, 0).
---@param spec? implot.ImPlotSpec|integer
function implot.plotText(text, x, y, pix_offset, spec)
end

--- Adds a legend entry with the shared spec conversion.
---@param label_id string
---@param spec? implot.ImPlotSpec|integer
function implot.plotDummy(label_id, spec)
end

--- Plots an axis-aligned image; bounds are double-precision plot coordinates (y-up),
--- UVs are texture coordinates (y-down). Missing/nil options use native defaults.
---@param label_id string
---@param texture cc.Texture2D
---@param bounds_min ImPlotPoint
---@param bounds_max ImPlotPoint
---@param uv0? ImVec2 Defaults to (0, 0).
---@param uv1? ImVec2 Defaults to (1, 1).
---@param tint_col? ImVec4 Defaults to (1, 1, 1, 1).
---@param spec? implot.ImPlotSpec|integer
function implot.plotImage(label_id, texture, bounds_min, bounds_max, uv0, uv1, tint_col, spec)
end

--- Uses sprite UVs and the shared spec conversion. The default tint is opaque white.
---@param label_id string
---@param sprite cc.Sprite
---@param bounds_min ImPlotPoint
---@param bounds_max ImPlotPoint
---@param tint_col? ImVec4 Defaults to (1, 1, 1, 1).
---@param spec? implot.ImPlotSpec|integer
function implot.plotImage(label_id, sprite, bounds_min, bounds_max, tint_col, spec)
end

-------------------------------------------------------------------------------
-- Plot Tools
-------------------------------------------------------------------------------

-- These functions require an active plot. Outputs append clicked, hovered, held
-- after changed and the updated coordinates. The booleans are returned even with NoInputs.

---@param id integer
---@param x number
---@param y number
---@param col ImVec4
---@param size? number Defaults to 4.
---@param flags? implot.ImPlotDragToolFlags Defaults to 0.
---@return boolean changed
---@return number x
---@return number y
---@return boolean clicked
---@return boolean hovered
---@return boolean held
function implot.dragPoint(id, x, y, col, size, flags)
end

---@param id integer
---@param x number
---@param col ImVec4
---@param thickness? number Defaults to 1.
---@param flags? implot.ImPlotDragToolFlags Defaults to 0.
---@return boolean changed
---@return number x
---@return boolean clicked
---@return boolean hovered
---@return boolean held
function implot.dragLineX(id, x, col, thickness, flags)
end

---@param id integer
---@param y number
---@param col ImVec4
---@param thickness? number Defaults to 1.
---@param flags? implot.ImPlotDragToolFlags Defaults to 0.
---@return boolean changed
---@return number y
---@return boolean clicked
---@return boolean hovered
---@return boolean held
function implot.dragLineY(id, y, col, thickness, flags)
end

--- Returns all four updated coordinates, followed by interaction state.
---@param id integer
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param col ImVec4
---@param flags? implot.ImPlotDragToolFlags Defaults to 0.
---@return boolean changed
---@return number x1
---@return number y1
---@return number x2
---@return number y2
---@return boolean clicked
---@return boolean hovered
---@return boolean held
function implot.dragRect(id, x1, y1, x2, y2, col, flags)
end

-------------------------------------------------------------------------------
-- Plot Utils
-------------------------------------------------------------------------------

--- Set the X axis ticks and optionally the labels for the next plot.
---@param values number[]
---@param n_ticks number
---@param labels string[] @optional
---@param keep_default boolean @optional
function implot.setNextPlotTicksX(values, n_ticks, labels, keep_default)
end

--- Set the X axis ticks and optionally the labels for the next plot.
---@param x_min number
---@param x_max number
---@param n_ticks number
---@param labels string @optional
---@param keep_default boolean @optional
function implot.setNextPlotTicksX(x_min, x_max, n_ticks, labels, keep_default)
end

--- Set the Y axis ticks and optionally the labels for the next plot.
---@param values number[]
---@param n_ticks number
---@param labels string[] @optional
---@param show_default boolean @optional
---@param y_axis number @optional, ImPlotYAxis
function implot.setNextPlotTicksY(values, n_ticks, labels, show_default, y_axis)
end

--- Set the Y axis ticks and optionally the labels for the next plot.
---@param x_min number
---@param x_max number
---@param n_ticks number
---@param labels string @optional
---@param show_default boolean @optional
---@param y_axis number @optional, ImPlotYAxis
function implot.setNextPlotTicksY(x_min, x_max, n_ticks, labels, show_default, y_axis)
end

-------------------------------------------------------------------------------
-- Plot and Item Styling
-------------------------------------------------------------------------------

-- Use PushStyleX to temporarily modify your ImPlotStyle. The modification
-- will last until the matching call to PopStyleX. You MUST call a pop for
-- every push, otherwise you will leak memory! This behaves just like ImGui.

--- Temporarily modify a style color. Don't forget to call PopStyleColor!
---@param idx number
---@param col number|ImVec4
function implot.pushStyleColor(idx, col)
end

--- Temporarily modify a style variable. Don't forget to call PopStyleVar!
---@param idx number
---@param val number|ImVec2
function implot.pushStyleVar(idx, val)
end

-------------------------------------------------------------------------------
-- Colormaps
-------------------------------------------------------------------------------

-- Item styling is based on colormaps when the relevant ImPlotCol_XXX is set to
-- IMPLOT_AUTO_COL (default). Several built-in colormaps are available. You can
-- add and then push/pop your own colormaps as well. To permanently set a colormap,
-- modify the Colormap index member of your ImPlotStyle.
--
-- Colormap data will be ignored and a custom color will be used if you have done one of the following:
--     1) Modified an item style color in your ImPlotStyle to anything other than IMPLOT_AUTO_COL.
--     2) Pushed an item style color using PushStyleColor().
--     3) Set the next item style with a SetNextXXXStyle function.


--- Add a new colormap. The color data will be copied. The colormap can be used by pushing either the returned index or the
--- string name with PushColormap. The colormap name must be unique and the size must be greater than 1. You will receive
--- an assert otherwise! By default colormaps are considered to be qualitative (i.e. discrete). If you want to create a
--- continuous colormap, set #qual=false. This will treat the colors you provide as keys, and ImPlot will build a linearly
--- interpolated lookup table. The memory footprint of this table will be exactly ((size-1)*255+1)*4 bytes.
---@param name string
---@param cols number[]|ImVec4[]
---@param qual boolean @optional, true
---@return implot.ImPlotColormap
function implot.addColormap(name, cols, qual)
end

--- Temporarily switch to one of the built-in (i.e. ImPlotColormap_XXX) or user-added colormaps (i.e. a return value of AddColormap). Don't forget to call PopColormap!
---@param cmap implot.ImPlotColormap
function implot.pushColormap(cmap)
end

--- Push a colormap by string name. Use built-in names such as "Default", "Deep", "Jet", etc. or a string you provided to AddColormap. Don't forget to call PopColormap!
---@param name string
function implot.pushColormap(name)
end

--- Shows a horizontal slider with a colormap gradient background. Optionally returns the color sampled at t in [0 1].
---@param label string
---@param t number
---@param format string @optional, ""
---@param cmap implot.ImPlotColormap @optional, IMPLOT_AUTO
---@return boolean,number,ImVec4 @changed, new t, color
function implot.colormapSlider(label, t, format, cmap)
end

--- When items in a plot sample their color from a colormap, the color is cached and does not change
--- unless explicitly overriden. Therefore, if you change the colormap after the item has already been plotted,
--- item colors will NOT update. If you need item colors to resample the new colormap, then use this
--- function to bust the cached colors. If #plot_title_id is NULL, then every item in EVERY existing plot
--- will be cache busted. Otherwise only the plot specified by #plot_title_id will be busted. For the
--- latter, this function must be called in the same ImGui ID scope that the plot is in. You should rarely if ever
--- need this function, but it is available for applications that require runtime colormap swaps (e.g. Heatmaps demo).
---@param plot_title_id string|nil
function implot.bustColorCache(plot_title_id)
end

--- Render icons similar to those that appear in legends (nifty for data lists).
---@param col number|ImVec4
function implot.itemIcon(col)
end

-------------------------------------------------------------------------------
-- Miscellaneous
-------------------------------------------------------------------------------

--- Shows ImPlot metrics/debug information window.
---@param p_popen boolean @optional
---@return boolean|nil
function implot.showMetricsWindow(p_popen)
end

-------------------------------------------------------------------------------
-- Demo (add implot_demo.cpp to your sources!)
-------------------------------------------------------------------------------

--- Shows the ImPlot demo window.
---@param p_popen boolean @optional
---@return boolean|nil
function implot.showDemoWindow(p_popen)
end

-------------------------------------------------------------------------------

---@class implot.ImPlotPoint
local ImPlotPoint = { x = 0, y = 0 }

---@class implot.ImPlotRange
local ImPlotRange = { min = 0, max = 0 }

---@class implot.ImPlotLimits
local ImPlotLimits = { x = ImPlotRange, y = ImPlotRange }

