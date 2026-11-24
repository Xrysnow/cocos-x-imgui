#include "imgui_impl_cc.h"
#include "cocos2d.h"
#include "renderer/backend/Backend.h"
#include "renderer/backend/gfx/DeviceGFX.h"
#ifdef CC_PLATFORM_PC
#include "gfx/backend/gfx-base/GFXDevice.h"
#endif
#include <cstring>
#include <algorithm>
#include <deque>
#include <utility>
#include <vector>

#include "imgui_internal.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#endif

#ifdef CC_PLATFORM_PC
#include "platform/desktop/GLViewImpl-desktop.h"
#include "platform/desktop/TextInputGLFW.h"
// GLFW
#ifdef _WIN32
	#include "glfw3.h"
	#undef APIENTRY
	#ifndef GLFW_EXPOSE_NATIVE_WIN32
		#define GLFW_EXPOSE_NATIVE_WIN32
	#endif
	#include "glfw3native.h"
#else
	#include <glfw3.h>
#endif // _WIN32

static_assert(
	GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300,
	"glfw version should be 3.3+");

#define GLFW_HAS_WINDOW_TOPMOST       (1) // 3.2+ GLFW_FLOATING
#define GLFW_HAS_WINDOW_HOVERED       (1) // 3.3+ GLFW_HOVERED
#define GLFW_HAS_WINDOW_ALPHA         (1) // 3.3+ glfwSetWindowOpacity
#define GLFW_HAS_PER_MONITOR_DPI      (1) // 3.3+ glfwGetMonitorContentScale
#define GLFW_HAS_VULKAN               (1) // 3.2+ glfwCreateWindowSurface
#define GLFW_HAS_FOCUS_WINDOW         (1) // 3.2+ glfwFocusWindow
#define GLFW_HAS_FOCUS_ON_SHOW        (1) // 3.3+ GLFW_FOCUS_ON_SHOW
#define GLFW_HAS_MONITOR_WORK_AREA    (1) // 3.3+ glfwGetMonitorWorkarea
// 3.3.1+ Fixed: Resizing window repositions it on MacOS #1553
#define GLFW_HAS_OSX_WINDOW_POS_FIX (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 + GLFW_VERSION_REVISION * 10 >= 3310)
// Let's be nice to people who pulled GLFW between 2019-04-16 (3.4 define) and 2019-11-29 (cursors defines) // FIXME: Remove when GLFW 3.4 is released?
#ifdef GLFW_RESIZE_NESW_CURSOR
	// 3.4+ GLFW_RESIZE_ALL_CURSOR, GLFW_RESIZE_NESW_CURSOR, GLFW_RESIZE_NWSE_CURSOR, GLFW_NOT_ALLOWED_CURSOR
	#define GLFW_HAS_NEW_CURSORS (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3400)
#else
	#define GLFW_HAS_NEW_CURSORS (0)
#endif

// Let's be nice to people who pulled GLFW between 2019-04-16 (3.4 define) and 2020-07-17 (passthrough)
#ifdef GLFW_MOUSE_PASSTHROUGH
	// 3.4+ GLFW_MOUSE_PASSTHROUGH
	#define GLFW_HAS_MOUSE_PASSTHROUGH (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3400)
#else
	#define GLFW_HAS_MOUSE_PASSTHROUGH (0)
#endif

#endif // CC_PLATFORM_PC

using namespace cc;
using namespace backend;

// GLFW data
static double g_Time = 0.0;
static bool g_MouseJustPressed[5] = { false, false, false, false, false };
static ImVec2 g_CursorPos = ImVec2(-FLT_MAX, -FLT_MAX);

#ifndef CC_PLATFORM_PC
static EventListenerTouchOneByOne* g_TouchListener = nullptr;
static EventListenerKeyboard* g_KeyboardListener = nullptr;
static int g_TouchId = -1;
static bool g_MobileKeys[512] = {};
#endif

#ifndef CC_PLATFORM_PC
static ImGuiKey ImGui_ImplCC_TranslateKey(EventKeyboard::KeyCode key)
{
	using K = EventKeyboard::KeyCode;
	if (key >= K::KEY_A && key <= K::KEY_Z)
		return (ImGuiKey)(ImGuiKey_A + (int)key - (int)K::KEY_A);
	if (key >= K::KEY_CAPITAL_A && key <= K::KEY_CAPITAL_Z)
		return (ImGuiKey)(ImGuiKey_A + (int)key - (int)K::KEY_CAPITAL_A);
	if (key >= K::KEY_0 && key <= K::KEY_9)
		return (ImGuiKey)(ImGuiKey_0 + (int)key - (int)K::KEY_0);
	if (key >= K::KEY_F1 && key <= K::KEY_F12)
		return (ImGuiKey)(ImGuiKey_F1 + (int)key - (int)K::KEY_F1);
	switch (key)
	{
#define IMGUI_COCOS_KEY(cocos, imgui) case K::cocos: return ImGuiKey_##imgui
		IMGUI_COCOS_KEY(KEY_TAB, Tab);
		IMGUI_COCOS_KEY(KEY_LEFT_ARROW, LeftArrow);
		IMGUI_COCOS_KEY(KEY_DPAD_LEFT, LeftArrow);
		IMGUI_COCOS_KEY(KEY_RIGHT_ARROW, RightArrow);
		IMGUI_COCOS_KEY(KEY_DPAD_RIGHT, RightArrow);
		IMGUI_COCOS_KEY(KEY_UP_ARROW, UpArrow);
		IMGUI_COCOS_KEY(KEY_DPAD_UP, UpArrow);
		IMGUI_COCOS_KEY(KEY_DOWN_ARROW, DownArrow);
		IMGUI_COCOS_KEY(KEY_DPAD_DOWN, DownArrow);
		IMGUI_COCOS_KEY(KEY_PG_UP, PageUp);
		IMGUI_COCOS_KEY(KEY_PG_DOWN, PageDown);
		IMGUI_COCOS_KEY(KEY_HOME, Home);
		IMGUI_COCOS_KEY(KEY_END, End);
		IMGUI_COCOS_KEY(KEY_INSERT, Insert);
		IMGUI_COCOS_KEY(KEY_DELETE, Delete);
		IMGUI_COCOS_KEY(KEY_BACKSPACE, Backspace);
		IMGUI_COCOS_KEY(KEY_SPACE, Space);
		IMGUI_COCOS_KEY(KEY_RETURN, Enter);
		IMGUI_COCOS_KEY(KEY_ENTER, Enter);
		IMGUI_COCOS_KEY(KEY_KP_ENTER, KeypadEnter);
		IMGUI_COCOS_KEY(KEY_ESCAPE, Escape);
		IMGUI_COCOS_KEY(KEY_LEFT_CTRL, LeftCtrl);
		IMGUI_COCOS_KEY(KEY_RIGHT_CTRL, RightCtrl);
		IMGUI_COCOS_KEY(KEY_LEFT_SHIFT, LeftShift);
		IMGUI_COCOS_KEY(KEY_RIGHT_SHIFT, RightShift);
		IMGUI_COCOS_KEY(KEY_LEFT_ALT, LeftAlt);
		IMGUI_COCOS_KEY(KEY_RIGHT_ALT, RightAlt);
		IMGUI_COCOS_KEY(KEY_HYPER, LeftSuper);
		IMGUI_COCOS_KEY(KEY_COMMA, Comma);
		IMGUI_COCOS_KEY(KEY_MINUS, Minus);
		IMGUI_COCOS_KEY(KEY_PERIOD, Period);
		IMGUI_COCOS_KEY(KEY_SLASH, Slash);
		IMGUI_COCOS_KEY(KEY_SEMICOLON, Semicolon);
		IMGUI_COCOS_KEY(KEY_EQUAL, Equal);
		IMGUI_COCOS_KEY(KEY_LEFT_BRACKET, LeftBracket);
		IMGUI_COCOS_KEY(KEY_BACK_SLASH, Backslash);
		IMGUI_COCOS_KEY(KEY_RIGHT_BRACKET, RightBracket);
		IMGUI_COCOS_KEY(KEY_APOSTROPHE, Apostrophe);
		IMGUI_COCOS_KEY(KEY_GRAVE, GraveAccent);
#undef IMGUI_COCOS_KEY
		default: return ImGuiKey_None;
	}
}

static void ImGui_ImplCC_MobileKey(EventKeyboard::KeyCode key, bool down)
{
	const int native = (int)key;
	if (native < 0 || native >= IM_ARRAYSIZE(g_MobileKeys)) return;
	g_MobileKeys[native] = down;
	auto& io = ImGui::GetIO();
	using K = EventKeyboard::KeyCode;
	io.AddKeyEvent(ImGuiMod_Ctrl, g_MobileKeys[(int)K::KEY_LEFT_CTRL] || g_MobileKeys[(int)K::KEY_RIGHT_CTRL]);
	io.AddKeyEvent(ImGuiMod_Shift, g_MobileKeys[(int)K::KEY_LEFT_SHIFT] || g_MobileKeys[(int)K::KEY_RIGHT_SHIFT]);
	io.AddKeyEvent(ImGuiMod_Alt, g_MobileKeys[(int)K::KEY_LEFT_ALT] || g_MobileKeys[(int)K::KEY_RIGHT_ALT]);
	io.AddKeyEvent(ImGuiMod_Super, g_MobileKeys[(int)K::KEY_HYPER]);
	const auto translated = ImGui_ImplCC_TranslateKey(key);
	if (translated != ImGuiKey_None) io.AddKeyEvent(translated, down);
}

#endif

class ImGuiTextInput
{
public:
	unsigned gamepadPlayer = 0;
	cc::input::DeviceId gamepadDevice{};
	std::uint64_t gamepadTick = 0;

	explicit ImGuiTextInput(ImGuiContext* context) : _context(context)
	{
		ImGuiContextHook hook;
		hook.Type = ImGuiContextHookType_EndFramePost;
		hook.UserData = this;
		hook.Callback = [](ImGuiContext* ctx, ImGuiContextHook* installed) {
			// Dear ImGui only calls the platform function when the geometry changes.
			// Adjacent fields can share coordinates, so check the active ID every frame.
			auto* self = static_cast<ImGuiTextInput*>(installed->UserData);
			self->update(ImGui::FindViewportByID(ctx->PlatformImeData.ViewportId), ctx->PlatformImeData);
		};
		_hookId = ImGui::AddContextHook(context, &hook);
	}

	~ImGuiTextInput()
	{
		stop();
		ImGui::RemoveContextHook(_context, _hookId);
	}

	void update(ImGuiViewport* viewport, const ImGuiPlatformImeData& data)
	{
		const ImGuiID field = _context->ActiveId;
		if (!data.WantTextInput || !field || !viewport || viewport->ID != data.ViewportId)
		{
			stop();
			return;
		}
		const auto view = Director::getInstance()->getOpenGLView();
		if (!view)
		{
			stop();
			return;
		}
		int x = 0, y = 0;
#ifdef CC_PLATFORM_PC
		auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle);
		const auto windowId = TextInputGLFW::getWindow(window);
		const bool focused = windowId && glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
		if (focused && (_context->IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable))
			glfwGetWindowPos(window, &x, &y);
#else
		const auto windowId = view->getTextInputWindow();
		const bool focused = windowId != 0;
#endif
		if (_field != field || _viewport != viewport->ID || _session.window() != windowId)
			stop();
		if (!focused)
			return;
		if (_replaced)
			return;

		const TextInputRect caret{data.InputPos.x - x, data.InputPos.y - y, 1.0, data.InputLineHeight};
		const TextInputArea area{caret, caret, std::nullopt, TextInputSpace::window};
		if (_session)
		{
			pruneConsumedInput();
			if (_caret.x != caret.x || _caret.y != caret.y || _caret.height != caret.height)
				_session.setArea(area);
			_caret = caret;
			updateKeyboardRequest(data.WantVisible);
			return;
		}

		auto& service = view->getTextInputService();
		if (_field == field && service.hasTextFocus())
			return;

		TextInputOptions options;
		if (_context->InputTextState.ID == field)
		{
			const auto flags = _context->InputTextState.Flags;
			options.multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
			if (flags & ImGuiInputTextFlags_Password)
				options.purpose = TextInputPurpose::password;
		}
		TextInputInitialState initial;
		initial.area = area;
		auto started = service.begin(windowId, options, [this](const TextInputEvent& event) { receive(event); }, initial);
		if (started)
		{
			_session = std::move(started.session);
			_field = field;
			_viewport = viewport->ID;
			_caret = caret;
#ifdef CC_PLATFORM_PC
			_glfwWindow = window;
			TextInputGLFW::setCommitObserver(window, [](void* user, TextInputSessionId session) {
				auto* self = static_cast<ImGuiTextInput*>(user);
				if (self->_session.id() == session)
					self->_commitBoundaries.push_back(self->_context->InputEventsNextEventId);
			}, this);
#endif
			updateKeyboardRequest(data.WantVisible);
		}
	}

	void newFrame()
	{
		if (!_finishAfterKeyboardHide && !_replaced)
			return;
		pruneConsumedInput();
		if (!_pendingInput.empty())
			return;
		if (_context->ActiveId == _field)
			ImGui::ClearActiveID();
		_context->IO.ClearInputKeys();
#ifndef CC_PLATFORM_PC
		std::fill(std::begin(g_MobileKeys), std::end(g_MobileKeys), false);
#endif
		stop();
	}

	void windowDestroyed(ImGuiID viewport)
	{
		if (_viewport == viewport)
			stop();
	}

private:
	void receive(const TextInputEvent& event)
	{
		if (event.session != _session.id() || _context->ActiveId != _field)
			return;
		if (const auto* closed = std::get_if<TextInputClosed>(&event.payload))
		{
			_replaced = closed->reason == TextInputCloseReason::replaced;
			return;
		}
		auto& queue = _context->InputEventsQueue;
		const int start = queue.Size;
		if (const auto* commit = std::get_if<TextInputCommit>(&event.payload))
		{
			_context->IO.AddInputCharactersUTF8(commit->text.c_str());
#ifdef CC_PLATFORM_PC
			if (!_commitBoundaries.empty())
			{
				const auto boundary = _commitBoundaries.front();
				_commitBoundaries.pop_front();
				if (start != queue.Size)
				{
					for (int i = start; i < queue.Size; ++i)
						_pendingInput.push_back(queue[i].EventId);
					// A later key (e.g. Enter) or click may already have queued while GLFW was polling.
					// Reinsert the confirmed text ahead of those events in their original input order.
					const auto appended = queue.begin() + start;
					const auto position = std::find_if(queue.begin(), appended, [&](const ImGuiInputEvent& input) {
						return input.EventId >= boundary && !owns(input.EventId);
					});
					std::rotate(position, appended, queue.end());
				}
				return;
			}
#endif
		}
#ifndef CC_PLATFORM_PC
		else if (const auto* command = std::get_if<TextInputCommand>(&event.payload))
			queueCommand(command->action);
		else if (const auto* changed = std::get_if<TextInputStateChanged>(&event.payload))
		{
			if (changed->state.keyboardVisible == TextInputKnowledge::yes)
			{
				_keyboardWasVisible = true;
				_finishAfterKeyboardHide = false;
			}
			else if (changed->state.keyboardVisible == TextInputKnowledge::no && _keyboardWasVisible && _keyboardRequested)
				_finishAfterKeyboardHide = true;
		}
#endif
		for (int i = start; i < queue.Size; ++i)
		{
			const auto& input = queue[i];
			if (input.Type == ImGuiInputEventType_Text || (input.Type == ImGuiInputEventType_Key && input.Key.Down))
				_pendingInput.push_back(input.EventId);
		}
	}

	void updateKeyboardRequest(bool requested)
	{
		if (_keyboardRequested == requested)
			return;
		_keyboardRequested = requested;
		_finishAfterKeyboardHide = false;
		if (_session.capabilities().requestKeyboard)
			_session.requestKeyboard(requested);
	}

	void pruneConsumedInput()
	{
		const auto& queue = _context->InputEventsQueue;
		if (_pendingInput.empty())
			return;
		const auto first = std::find_if(queue.begin(), queue.end(), [&](const ImGuiInputEvent& input) {
			return owns(input.EventId);
		});
		const auto pending = first == queue.end() ? _pendingInput.end() :
			std::lower_bound(_pendingInput.begin(), _pendingInput.end(), first->EventId);
		_pendingInput.erase(_pendingInput.begin(), pending);
	}

	bool owns(ImU32 eventId) const
	{
		return std::binary_search(_pendingInput.begin(), _pendingInput.end(), eventId);
	}

#ifndef CC_PLATFORM_PC
	void queueCommand(TextInputCommandAction action)
	{
		using Command = TextInputCommandAction;
		ImGuiKey key = ImGuiKey_None;
		switch (action)
		{
		case Command::backspace: key = ImGuiKey_Backspace; break;
		case Command::deleteForward: key = ImGuiKey_Delete; break;
		case Command::left: key = ImGuiKey_LeftArrow; break;
		case Command::right: key = ImGuiKey_RightArrow; break;
		case Command::up: key = ImGuiKey_UpArrow; break;
		case Command::down: key = ImGuiKey_DownArrow; break;
		case Command::lineStart: key = ImGuiKey_Home; break;
		case Command::lineEnd: key = ImGuiKey_End; break;
		case Command::cancel: key = ImGuiKey_Escape; break;
		case Command::insertNewline:
		case Command::done:
		case Command::search:
		case Command::send:
		case Command::next:
		case Command::go: key = ImGuiKey_Enter; break;
		case Command::selectAll: break;
		}
		if (key != ImGuiKey_None)
		{
			_context->IO.AddKeyEvent(key, true);
			_context->IO.AddKeyEvent(key, false);
		}
	}
#endif

	void stop()
	{
#ifdef CC_PLATFORM_PC
		if (_glfwWindow)
			TextInputGLFW::clearCommitObserver(_glfwWindow, this);
		_glfwWindow = nullptr;
		_commitBoundaries.clear();
#endif
		if (!_pendingInput.empty())
		{
			// Leave queued key releases intact if a previous frame processed their down event.
			auto& queue = _context->InputEventsQueue;
			const auto end = std::remove_if(queue.begin(), queue.end(), [&](const ImGuiInputEvent& event) {
				return owns(event.EventId);
			});
			if (end != queue.end())
				queue.erase(end, queue.end());
			_pendingInput.clear();
		}
		_session = {};
		_field = 0;
		_viewport = 0;
		_keyboardRequested = true;
		_keyboardWasVisible = false;
		_finishAfterKeyboardHide = false;
		_replaced = false;
	}

	ImGuiContext* _context;
	ImGuiID _hookId = 0;
	ImGuiID _field = 0;
	ImGuiID _viewport = 0;
	TextInputSession _session;
	TextInputRect _caret;
	std::vector<ImU32> _pendingInput;
#ifdef CC_PLATFORM_PC
	GLFWwindow* _glfwWindow = nullptr;
	std::deque<ImU32> _commitBoundaries;
#endif
	bool _keyboardRequested = true;
	bool _keyboardWasVisible = false;
	bool _finishAfterKeyboardHide = false;
	bool _replaced = false;
};

static ImGuiTextInput* ImGui_ImplCC_GetTextInput(ImGuiContext* context)
{
	return static_cast<ImGuiTextInput*>(ImGui::GetIO(context).BackendPlatformUserData);
}

static void ImGui_ImplCC_SetImeData(ImGuiContext* context, ImGuiViewport* viewport, ImGuiPlatformImeData* data)
{
	if (auto* input = ImGui_ImplCC_GetTextInput(context))
		input->update(viewport, *data);
}

#ifdef CC_PLATFORM_PC
static GLFWcursor*          g_MouseCursors[ImGuiMouseCursor_COUNT] = { nullptr };
static GLFWwindow*          g_KeyOwnerWindows[512] = { nullptr };
// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
static GLFWmousebuttonfun   g_PrevUserCallbackMousebutton = nullptr;
static GLFWscrollfun        g_PrevUserCallbackScroll = nullptr;
static GLFWkeyfun           g_PrevUserCallbackKey = nullptr;
static GLFWwindowfocusfun   g_PrevUserCallbackWindowFocus = nullptr;
static GLFWcursorposfun     g_PrevUserCallbackCursorPos = nullptr;
static GLFWmonitorfun       g_PrevUserCallbackMonitor = nullptr;
static bool                 g_WantUpdateMonitors = true;
static bool                 g_InstalledCallbacks = false;

// Forward Declarations
static void ImGui_ImplGlfw_InitPlatformInterface();
static void ImGui_ImplGlfw_ShutdownPlatformInterface();
static void ImGui_ImplGlfw_UpdateMonitors();

#endif // CC_PLATFORM_PC

struct ProgramInfo
{
	cc::backend::Program* program = nullptr;
	// Uniforms location
	UniformLocation texture;
	UniformLocation projection;
};
static ProgramInfo g_ProgramInfo;
static ProgramInfo g_ProgramFontInfo;
static Mat4 g_Projection;
constexpr IndexFormat g_IndexFormat = sizeof(ImDrawIdx) == 2 ? IndexFormat::U_SHORT : IndexFormat::U_INT;

static std::vector<std::shared_ptr<CallbackCommand>> g_CallbackCommands;
static std::vector<std::shared_ptr<CustomCommand>> g_CustomCommands;
static Vector<ProgramState*> g_ProgramStates;
// cocos executes draw commands later; retain textures until those commands finish.
static Vector<Texture2D*> g_DrawTextures;

static bool ImGui_ImplCC_UpdateTexture(ImTextureData* texture);

static void AddRendererCommand(const std::function<void()>& f)
{
	const auto renderer = Director::getInstance()->getRenderer();
#ifdef CC_VERSION
	auto cmd = renderer->nextCallbackCommand();
	cmd->func = f;
	renderer->addCommand(cmd);
#else
	auto cmd = std::make_shared<CallbackCommand>();
	g_CallbackCommands.push_back(cmd);
	cmd->init(0.f);
	cmd->func = f;
	renderer->addCommand(cmd.get());
#endif // CC_VERSION
}

#ifdef CC_PLATFORM_PC
static GLFWwindow* ImGui_ImplCC_GetWindow()
{
	const auto glv = (GLViewImpl*)Director::getInstance()->getOpenGLView();
	return glv ? glv->getWindow() : nullptr;
}
#endif // CC_PLATFORM_PC

static void ImGui_ImplCC_SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height)
{
	const auto renderer = Director::getInstance()->getRenderer();
	// setup
	AddRendererCommand([=]()
	{
		renderer->setCullMode(backend::CullMode::NONE);
		renderer->setScissorTest(true);
		renderer->setDepthTest(false);
		renderer->setStencilTest(false);
		renderer->setViewPort(0, 0, fb_width, fb_height);
	});

	const auto L = draw_data->DisplayPos.x;
	const auto R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
	const auto T = draw_data->DisplayPos.y;
	const auto B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
	Mat4::createOrthographicOffCenter(L, R, B, T, -1.f, 1.f, &g_Projection);
}

struct SavedRenderState
{
	backend::CullMode cull;
	Viewport vp;
	ScissorRect scissorRect;
	bool scissorTest;
	bool depthTest;
	bool stencilTest;
};

void ImGui_ImplCC_RenderDrawData(ImDrawData* draw_data)
{
	// Glyphs may be loaded during UI submission. Upload them before resolving TexRef.
	if (draw_data->Textures)
		for (ImTextureData* texture : *draw_data->Textures)
			if (texture->Status != ImTextureStatus_OK && !ImGui_ImplCC_UpdateTexture(texture))
				return;

	const ImVec2 display_size = draw_data->DisplaySize;
	if (display_size.x <= 0.0f || display_size.y <= 0.0f)
		return;

	ImVec2 clip_scale = draw_data->FramebufferScale;
	int fb_width = (int)(display_size.x * clip_scale.x);
	int fb_height = (int)(display_size.y * clip_scale.y);
#ifdef CC_PLATFORM_PC
	if (draw_data->OwnerViewport == ImGui::GetMainViewport())
	{
		const auto& swapchains = gfx::Device::getInstance()->getSwapchains();
		// Use the current swapchain extent for both the viewport and the scissor conversion.
		if (!swapchains.empty())
		{
			if (const auto* target = swapchains.front()->getColorTexture())
			{
				fb_width = (int)target->getWidth();
				fb_height = (int)target->getHeight();
				clip_scale = ImVec2(fb_width / display_size.x, fb_height / display_size.y);
			}
		}
	}
#endif
	if (fb_width <= 0 || fb_height <= 0)
		return;

	const auto renderer = Director::getInstance()->getRenderer();
	// Each submission owns its saved state until its deferred restore runs.
	auto saved_state = std::make_shared<SavedRenderState>();
	AddRendererCommand([renderer, saved_state]()
	{
		saved_state->cull = renderer->getCullMode();
		saved_state->vp = renderer->getViewport();
		saved_state->scissorTest = renderer->getScissorTest();
		saved_state->scissorRect = renderer->getScissorRect();
		saved_state->depthTest = renderer->getDepthTest();
		saved_state->stencilTest = renderer->getStencilTest();
	});

	ImGui_ImplCC_SetupRenderState(draw_data, fb_width, fb_height);

	// Will project scissor/clipping rectangles into framebuffer space
	const ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports

	// Render command lists
	for (int n = 0; n < draw_data->CmdLists.Size; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];

		// Upload vertex/index buffers
		const auto device = static_cast<backend::DeviceGFX*>(backend::Device::getInstance());
		const auto vsize = cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
		const auto isize = cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
		Buffer* vbuffer = nullptr;
		Buffer* ibuffer = nullptr;
		if (vsize > 0 && isize > 0)
		{
			vbuffer = device->newBuffer(vsize, sizeof(ImDrawVert), BufferType::VERTEX, BufferUsage::STATIC);
			ibuffer = device->newBuffer(isize, sizeof(ImDrawIdx), BufferType::INDEX, BufferUsage::STATIC);
			vbuffer->autorelease();
			vbuffer->updateData(cmd_list->VtxBuffer.Data, vsize);
			ibuffer->autorelease();
			ibuffer->updateData(cmd_list->IdxBuffer.Data, isize);
		}

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback != nullptr)
			{
				// User callback, registered via ImDrawList::AddCallback()
				// (ImDrawCallback_ResetRenderState is a special callback value used by the user
				// to request the renderer to reset render state.)
				if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
					ImGui_ImplCC_SetupRenderState(draw_data, fb_width, fb_height);
				else
				{
					AddRendererCommand([=]()
					{
						pcmd->UserCallback(cmd_list, pcmd);
					});
				}
			}
			else
			{
				if (pcmd->ElemCount == 0 || !vbuffer || !ibuffer) continue;
				// Project scissor/clipping rectangles into framebuffer space
				ImVec4 clip_rect;
				clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
				clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
				clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
				clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

				clip_rect.x = std::max(clip_rect.x, 0.0f);
				clip_rect.y = std::max(clip_rect.y, 0.0f);
				clip_rect.z = std::min(clip_rect.z, (float)fb_width);
				clip_rect.w = std::min(clip_rect.w, (float)fb_height);
				if (clip_rect.z > clip_rect.x && clip_rect.w > clip_rect.y)
				{
					// Apply scissor/clipping rectangle
					AddRendererCommand([=]()
					{
						renderer->setScissorRect(
							clip_rect.x,
							fb_height - clip_rect.w,
							clip_rect.z - clip_rect.x,
							clip_rect.w - clip_rect.y);
					});

					const ImTextureID texture_id = pcmd->GetTexID();
					if (texture_id == ImTextureID_Invalid || texture_id == 0)
						continue;
					if (auto tex = dynamic_cast<Texture2D*>((Ref*)texture_id))
					{
						g_DrawTextures.pushBack(tex);
						auto cmd = std::make_shared<CustomCommand>();
						g_CustomCommands.push_back(cmd);
						cmd->init(0.f, BlendFunc::ALPHA_NON_PREMULTIPLIED);
						const auto pinfo = tex->getPixelFormat() == PixelFormat::A8 ? &g_ProgramFontInfo : &g_ProgramInfo;
						// create new ProgramState
						cc::IntrusivePtr state{new ProgramState(pinfo->program)};
						g_ProgramStates.pushBack(state.get());
						auto& desc = cmd->getPipelineDescriptor();
						desc.programState = state.get();
						desc.programState->setUniform(pinfo->projection, &g_Projection, sizeof(Mat4));
						desc.programState->setTexture(pinfo->texture, 0, tex->getBackendTexture());
						// In order to composite our output buffer we need to preserve alpha
						desc.blendDescriptor.sourceAlphaBlendFactor = BlendFactor::ONE;
						// set vertex/index buffer
						cmd->setIndexBuffer(ibuffer, g_IndexFormat);
						cmd->setVertexBuffer(vbuffer);
						cmd->setDrawType(CustomCommand::DrawType::ELEMENT);
						cmd->setPrimitiveType(PrimitiveType::TRIANGLE);
						cmd->setIndexDrawInfo(pcmd->IdxOffset, pcmd->ElemCount);
						renderer->addCommand(cmd.get());						
					}
					else if (auto node = dynamic_cast<Node*>((Ref*)texture_id))
					{
						const bool visible = node->isVisible();
						const auto tr = node->getNodeToParentTransform();
						node->setVisible(true);
						node->setNodeToParentTransform(tr);
						const auto& proj = Director::getInstance()->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
						node->visit(Director::getInstance()->getRenderer(), proj.getInversed() * g_Projection, 0);
						node->setVisible(visible);
					}
				}
			}
		}
	}
	// restore
	AddRendererCommand([renderer, saved_state]()
	{
		renderer->setCullMode(saved_state->cull);
		auto& vp = saved_state->vp;
		renderer->setViewPort(vp.x, vp.y, vp.w, vp.h);
		renderer->setScissorTest(saved_state->scissorTest);
		auto& sc = saved_state->scissorRect;
		renderer->setScissorRect(sc.x, sc.y, sc.width, sc.height);
		renderer->setDepthTest(saved_state->depthTest);
		renderer->setStencilTest(saved_state->stencilTest);
	});
}

void ImGui_ImplCC_RenderPlatform()
{
#ifdef CC_PLATFORM_PC
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		if (!(ImGui::GetIO().BackendFlags & ImGuiBackendFlags_RendererHasViewports))
			return;
		// no GL context to restore under gfx; platform windows only
		ImGui::UpdatePlatformWindows();
		for (auto* viewport : ImGui::GetPlatformIO().Viewports)
			if (!viewport->PlatformHandle)
				viewport->PlatformRequestClose = true;
		ImGui::RenderPlatformWindowsDefault();
	}
#endif
}

static const char* ImGui_ImplCC_GetClipboardText(ImGuiContext*)
{
#ifdef CC_PLATFORM_PC
	return glfwGetClipboardString(ImGui_ImplCC_GetWindow());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	static std::string clipboard;
	clipboard = JniHelper::callStaticStringMethod("org/cocos/lib/CocosEngine", "getClipboardString");
	return clipboard.c_str();
#else
	return "";
#endif
}

static void ImGui_ImplCC_SetClipboardText(ImGuiContext*, const char* text)
{
#ifdef CC_PLATFORM_PC
	glfwSetClipboardString(ImGui_ImplCC_GetWindow(), text);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniHelper::callStaticVoidMethod("org/cocos/lib/CocosEngine", "setClipboardString", std::string(text ? text : ""));
#else
#endif
}

#ifdef CC_PLATFORM_PC

void ImGui_ImplCC_CursorPosCallback(GLFWwindow* window, double x, double y)
{
	if (g_PrevUserCallbackCursorPos && window == ImGui_ImplCC_GetWindow())
		g_PrevUserCallbackCursorPos(window, x, y);
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		int window_x, window_y;
		glfwGetWindowPos(window, &window_x, &window_y);
		x += window_x;
		y += window_y;
	}
	ImGui::GetIO().AddMousePosEvent((float)x, (float)y);
}

void ImGui_ImplCC_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (g_PrevUserCallbackMousebutton != nullptr && window == ImGui_ImplCC_GetWindow())
		g_PrevUserCallbackMousebutton(window, button, action, mods);

	if (button >= 0 && button < IM_ARRAYSIZE(ImGui::GetIO().MouseDown) &&
		(action == GLFW_PRESS || action == GLFW_RELEASE))
		ImGui::GetIO().AddMouseButtonEvent(button, action == GLFW_PRESS);
}

void ImGui_ImplCC_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (g_PrevUserCallbackScroll != nullptr && window == ImGui_ImplCC_GetWindow())
		g_PrevUserCallbackScroll(window, xoffset, yoffset);

	ImGuiIO& io = ImGui::GetIO();
	io.AddMouseWheelEvent((float)xoffset, (float)yoffset);
}

static ImGuiKey ImGui_ImplCC_KeyToImGuiKey(int keycode, int scancode)
{
	IM_UNUSED(scancode);
	switch (keycode)
	{
		case GLFW_KEY_TAB: return ImGuiKey_Tab;
		case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
		case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
		case GLFW_KEY_UP: return ImGuiKey_UpArrow;
		case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
		case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
		case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
		case GLFW_KEY_HOME: return ImGuiKey_Home;
		case GLFW_KEY_END: return ImGuiKey_End;
		case GLFW_KEY_INSERT: return ImGuiKey_Insert;
		case GLFW_KEY_DELETE: return ImGuiKey_Delete;
		case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
		case GLFW_KEY_SPACE: return ImGuiKey_Space;
		case GLFW_KEY_ENTER: return ImGuiKey_Enter;
		case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
		case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
		case GLFW_KEY_COMMA: return ImGuiKey_Comma;
		case GLFW_KEY_MINUS: return ImGuiKey_Minus;
		case GLFW_KEY_PERIOD: return ImGuiKey_Period;
		case GLFW_KEY_SLASH: return ImGuiKey_Slash;
		case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
		case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
		case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
		case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
		case GLFW_KEY_WORLD_1: return ImGuiKey_Oem102;
		case GLFW_KEY_WORLD_2: return ImGuiKey_Oem102;
		case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
		case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
		case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
		case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
		case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
		case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
		case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
		case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
		case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
		case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
		case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
		case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
		case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
		case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
		case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
		case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
		case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
		case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
		case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
		case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
		case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
		case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
		case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
		case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
		case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
		case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
		case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
		case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
		case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
		case GLFW_KEY_MENU: return ImGuiKey_Menu;
		case GLFW_KEY_0: return ImGuiKey_0;
		case GLFW_KEY_1: return ImGuiKey_1;
		case GLFW_KEY_2: return ImGuiKey_2;
		case GLFW_KEY_3: return ImGuiKey_3;
		case GLFW_KEY_4: return ImGuiKey_4;
		case GLFW_KEY_5: return ImGuiKey_5;
		case GLFW_KEY_6: return ImGuiKey_6;
		case GLFW_KEY_7: return ImGuiKey_7;
		case GLFW_KEY_8: return ImGuiKey_8;
		case GLFW_KEY_9: return ImGuiKey_9;
		case GLFW_KEY_A: return ImGuiKey_A;
		case GLFW_KEY_B: return ImGuiKey_B;
		case GLFW_KEY_C: return ImGuiKey_C;
		case GLFW_KEY_D: return ImGuiKey_D;
		case GLFW_KEY_E: return ImGuiKey_E;
		case GLFW_KEY_F: return ImGuiKey_F;
		case GLFW_KEY_G: return ImGuiKey_G;
		case GLFW_KEY_H: return ImGuiKey_H;
		case GLFW_KEY_I: return ImGuiKey_I;
		case GLFW_KEY_J: return ImGuiKey_J;
		case GLFW_KEY_K: return ImGuiKey_K;
		case GLFW_KEY_L: return ImGuiKey_L;
		case GLFW_KEY_M: return ImGuiKey_M;
		case GLFW_KEY_N: return ImGuiKey_N;
		case GLFW_KEY_O: return ImGuiKey_O;
		case GLFW_KEY_P: return ImGuiKey_P;
		case GLFW_KEY_Q: return ImGuiKey_Q;
		case GLFW_KEY_R: return ImGuiKey_R;
		case GLFW_KEY_S: return ImGuiKey_S;
		case GLFW_KEY_T: return ImGuiKey_T;
		case GLFW_KEY_U: return ImGuiKey_U;
		case GLFW_KEY_V: return ImGuiKey_V;
		case GLFW_KEY_W: return ImGuiKey_W;
		case GLFW_KEY_X: return ImGuiKey_X;
		case GLFW_KEY_Y: return ImGuiKey_Y;
		case GLFW_KEY_Z: return ImGuiKey_Z;
		case GLFW_KEY_F1: return ImGuiKey_F1;
		case GLFW_KEY_F2: return ImGuiKey_F2;
		case GLFW_KEY_F3: return ImGuiKey_F3;
		case GLFW_KEY_F4: return ImGuiKey_F4;
		case GLFW_KEY_F5: return ImGuiKey_F5;
		case GLFW_KEY_F6: return ImGuiKey_F6;
		case GLFW_KEY_F7: return ImGuiKey_F7;
		case GLFW_KEY_F8: return ImGuiKey_F8;
		case GLFW_KEY_F9: return ImGuiKey_F9;
		case GLFW_KEY_F10: return ImGuiKey_F10;
		case GLFW_KEY_F11: return ImGuiKey_F11;
		case GLFW_KEY_F12: return ImGuiKey_F12;
		case GLFW_KEY_F13: return ImGuiKey_F13;
		case GLFW_KEY_F14: return ImGuiKey_F14;
		case GLFW_KEY_F15: return ImGuiKey_F15;
		case GLFW_KEY_F16: return ImGuiKey_F16;
		case GLFW_KEY_F17: return ImGuiKey_F17;
		case GLFW_KEY_F18: return ImGuiKey_F18;
		case GLFW_KEY_F19: return ImGuiKey_F19;
		case GLFW_KEY_F20: return ImGuiKey_F20;
		case GLFW_KEY_F21: return ImGuiKey_F21;
		case GLFW_KEY_F22: return ImGuiKey_F22;
		case GLFW_KEY_F23: return ImGuiKey_F23;
		case GLFW_KEY_F24: return ImGuiKey_F24;
		default: return ImGuiKey_None;
	}
}

void ImGui_ImplCC_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE && TextInputGLFW::isKeyConsumed(window))
		return;
	if (g_PrevUserCallbackKey != nullptr && window == ImGui_ImplCC_GetWindow())
		g_PrevUserCallbackKey(window, key, scancode, action, mods);
	if (key < 0 || key >= IM_ARRAYSIZE(g_KeyOwnerWindows) ||
		(action != GLFW_PRESS && action != GLFW_RELEASE))
		return;
	g_KeyOwnerWindows[key] = action == GLFW_PRESS ? window : nullptr;
	auto& io = ImGui::GetIO();
	io.AddKeyEvent(ImGuiMod_Ctrl, g_KeyOwnerWindows[GLFW_KEY_LEFT_CONTROL] || g_KeyOwnerWindows[GLFW_KEY_RIGHT_CONTROL]);
	io.AddKeyEvent(ImGuiMod_Shift, g_KeyOwnerWindows[GLFW_KEY_LEFT_SHIFT] || g_KeyOwnerWindows[GLFW_KEY_RIGHT_SHIFT]);
	io.AddKeyEvent(ImGuiMod_Alt, g_KeyOwnerWindows[GLFW_KEY_LEFT_ALT] || g_KeyOwnerWindows[GLFW_KEY_RIGHT_ALT]);
	io.AddKeyEvent(ImGuiMod_Super, g_KeyOwnerWindows[GLFW_KEY_LEFT_SUPER] || g_KeyOwnerWindows[GLFW_KEY_RIGHT_SUPER]);
	const auto translated = ImGui_ImplCC_KeyToImGuiKey(key, scancode);
	if (translated != ImGuiKey_None)
	{
		io.AddKeyEvent(translated, action == GLFW_PRESS);
		io.SetKeyEventNativeData(translated, key, scancode);
	}
}

void ImGui_ImplCC_WindowFocusCallback(GLFWwindow* window, int focused)
{
	if (g_PrevUserCallbackWindowFocus && window == ImGui_ImplCC_GetWindow())
		g_PrevUserCallbackWindowFocus(window, focused);
	else
		TextInputGLFW::handleFocus(window, focused == GLFW_TRUE);
	if (!focused)
		for (auto& owner : g_KeyOwnerWindows)
			if (owner == window) owner = nullptr;
	ImGui::GetIO().AddFocusEvent(focused != 0);
}

void ImGui_ImplCC_CharCallback(GLFWwindow* window, unsigned int c)
{
	TextInputGLFW::handleCharacter(window, c);
}

void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor* monitor, int event)
{
	if (g_PrevUserCallbackMonitor) g_PrevUserCallbackMonitor(monitor, event);
	g_WantUpdateMonitors = true;
}

#endif

static void ImGui_ImplCC_DestroyTexture(ImTextureData* texture)
{
	auto* native_texture = static_cast<Texture2D*>(texture->BackendUserData);
	CC_SAFE_RELEASE(native_texture);
	texture->BackendUserData = nullptr;
	texture->SetTexID(ImTextureID_Invalid);
	texture->SetStatus(ImTextureStatus_Destroyed);
}

static bool ImGui_ImplCC_UpdateTexture(ImTextureData* texture)
{
	if (texture->Status == ImTextureStatus_WantCreate)
	{
		IM_ASSERT(texture->GetTexID() == ImTextureID_Invalid && texture->BackendUserData == nullptr);
		IM_ASSERT(texture->Format == ImTextureFormat_RGBA32 || texture->Format == ImTextureFormat_Alpha8);
		cc::IntrusivePtr native_texture{new Texture2D()};
		native_texture->setAntiAliasTexParameters();
		const auto format = texture->Format == ImTextureFormat_Alpha8 ? PixelFormat::A8 : PixelFormat::RGBA8888;
		if (!native_texture->initWithData(texture->GetPixels(), texture->GetSizeInBytes(),
			format, texture->Width, texture->Height))
		{
			return false;
		}
		texture->SetTexID((ImTextureID)native_texture.get());
		texture->BackendUserData = native_texture.detach();
		texture->SetStatus(ImTextureStatus_OK);
	}
	else if (texture->Status == ImTextureStatus_WantUpdates)
	{
		auto* native_texture = static_cast<Texture2D*>(texture->BackendUserData);
		IM_ASSERT(native_texture != nullptr);
		// Texture2D expects tightly packed rows, unlike the atlas's full-width pitch.
		std::vector<unsigned char> pixels;
		for (const ImTextureRect& rect : texture->Updates)
		{
			const size_t pitch = static_cast<size_t>(rect.w) * texture->BytesPerPixel;
			pixels.resize(pitch * rect.h);
			for (int y = 0; y < rect.h; ++y)
				std::memcpy(pixels.data() + y * pitch, texture->GetPixelsAt(rect.x, rect.y + y), pitch);
			if (!native_texture->updateWithSubData(pixels.data(), rect.x, rect.y, rect.w, rect.h))
				return false;
		}
		texture->SetStatus(ImTextureStatus_OK);
	}
	else if (texture->Status == ImTextureStatus_WantDestroy && texture->UnusedFrames > 0)
		ImGui_ImplCC_DestroyTexture(texture);
	return true;
}

bool ImGui_ImplCC_CreateDeviceObjects()
{
	static auto vertex_shader = R"(
#ifndef GL_ES
#define lowp
#define mediump
#endif

#if __VERSION__ >= 300

layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoord;
layout(location=2) in vec4 a_color;
layout(std140, binding=0) uniform VSBlock
{
    mat4 u_MVPMatrix;
};
layout(location=0) out lowp vec4 v_fragmentColor;
layout(location=1) out mediump vec2 v_texCoord;

#else

attribute vec2 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;
uniform mat4 u_MVPMatrix;
varying lowp vec4 v_fragmentColor;
varying mediump vec2 v_texCoord;

#endif

void main()
{
    gl_Position = u_MVPMatrix * vec4(a_position.xy, 0.0, 1.0);
    v_fragmentColor = a_color;
    v_texCoord = a_texCoord;
}
)";
	static auto fragment_shader = R"(
#ifdef GL_ES
precision lowp float;
#endif

#if __VERSION__ >= 300
layout(location=0) in vec4 v_fragmentColor;
layout(location=1) in vec2 v_texCoord;
layout(binding=2) uniform sampler2D u_texture;
layout(location=0) out vec4 cc_FragColor;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;
#endif

void main()
{
#if __VERSION__ >= 300
    cc_FragColor = v_fragmentColor * texture(u_texture, v_texCoord);
#else
    gl_FragColor = v_fragmentColor * texture2D(u_texture, v_texCoord);
#endif
}
)";
	static auto fragment_shader_font = R"(
#ifdef GL_ES
precision lowp float;
#endif

#if __VERSION__ >= 300
layout(location=0) in vec4 v_fragmentColor;
layout(location=1) in vec2 v_texCoord;
layout(binding=2) uniform sampler2D u_texture;
layout(location=0) out vec4 cc_FragColor;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture;
#endif

void main()
{
#if __VERSION__ >= 300
	float a = texture(u_texture, v_texCoord.st).r;
    cc_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * a);
#else
	float a = texture2D(u_texture, v_texCoord.st).r;
    gl_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * a);
#endif
}
)";
	CC_SAFE_RELEASE(g_ProgramInfo.program);
	CC_SAFE_RELEASE(g_ProgramFontInfo.program);
	g_ProgramInfo.program = backend::Device::getInstance()->newProgram(
		vertex_shader, fragment_shader);
	g_ProgramFontInfo.program = backend::Device::getInstance()->newProgram(
		vertex_shader, fragment_shader_font);
	IM_ASSERT(g_ProgramInfo.program);
	IM_ASSERT(g_ProgramFontInfo.program);
	if (!g_ProgramInfo.program || !g_ProgramFontInfo.program)
		return false;

	for (auto& p : { &g_ProgramInfo,&g_ProgramFontInfo })
	{
		p->texture = p->program->getUniformLocation(TEXTURE);
		p->projection = p->program->getUniformLocation(MVP_MATRIX);
		IM_ASSERT(bool(p->texture));
		IM_ASSERT(bool(p->projection));
		auto layout = p->program->getVertexLayout();
		layout->setAttribute("a_position", 0,
			VertexFormat::FLOAT2, 0, false);
		layout->setAttribute("a_texCoord", 1,
			VertexFormat::FLOAT2, offsetof(ImDrawVert, uv), false);
		layout->setAttribute("a_color", 2,
			VertexFormat::UBYTE4, offsetof(ImDrawVert, col), true);
		layout->setStride(sizeof(ImDrawVert));
	}

	return true;
}

void ImGui_ImplCC_DestroyDeviceObjects()
{
	// Shutdown may have no next frame to release the last draw's buffers.
	g_CallbackCommands.clear();
	g_CustomCommands.clear();
	g_ProgramStates.clear();
	g_DrawTextures.clear();
	CC_SAFE_RELEASE_NULL(g_ProgramInfo.program);
	CC_SAFE_RELEASE_NULL(g_ProgramFontInfo.program);
	for (ImTextureData* texture : ImGui::GetPlatformIO().Textures)
		if (texture->RefCount == 1)
			ImGui_ImplCC_DestroyTexture(texture);
}

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the back-end to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

bool ImGui_ImplCC_Init(bool install_callbacks)
{
	g_Time = 0.0;
	ImGui::CreateContext();

	// Setup backend capabilities flags
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;
	auto& platform_io = ImGui::GetPlatformIO();
	platform_io.Renderer_TextureMaxWidth = platform_io.Renderer_TextureMaxHeight =
		Configuration::getInstance()->getMaxTextureSize();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

#ifdef CC_PLATFORM_PC
	// gfx currently renders only the main viewport. Keep OS viewports disabled.
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	const auto window = ImGui_ImplCC_GetWindow();
	// We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	// We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	// We can create multi-viewports on the Platform side (optional)
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// We can set io.MouseHoveredViewport correctly (optional, not easy)
	io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;
#endif
	// gfx renderer does not support multi-viewports rendering
#endif // CC_PLATFORM_PC

	io.BackendPlatformName = "imgui_impl_cocos";
	io.BackendRendererName = "imgui_impl_cocos";
	// disable auto load and save
	io.IniFilename = nullptr;
	io.BackendPlatformUserData = new ImGuiTextInput(ImGui::GetCurrentContext());
	platform_io.Platform_SetImeDataFn = ImGui_ImplCC_SetImeData;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	platform_io.Platform_SetClipboardTextFn = ImGui_ImplCC_SetClipboardText;
	platform_io.Platform_GetClipboardTextFn = ImGui_ImplCC_GetClipboardText;
#endif

#ifdef CC_PLATFORM_PC
	platform_io.Platform_SetClipboardTextFn = ImGui_ImplCC_SetClipboardText;
	platform_io.Platform_GetClipboardTextFn = ImGui_ImplCC_GetClipboardText;

	// Create mouse cursors
	// (By design, on X11 cursors are user configurable and some cursors may be missing. When a cursor doesn't exist,
	// GLFW will emit an error which will often be printed by the app, so we temporarily disable error reporting.
	// Missing cursors will return NULL and our _UpdateMouseCursor() function will use the Arrow cursor instead.)
	GLFWerrorfun prev_error_callback = glfwSetErrorCallback(nullptr);
	g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
#if GLFW_HAS_NEW_CURSORS
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
#endif
	glfwSetErrorCallback(prev_error_callback);

	// Chain GLFW callbacks: our callbacks will call the user's previously installed callbacks, if any.
	g_PrevUserCallbackMousebutton = nullptr;
	g_PrevUserCallbackScroll = nullptr;
	g_PrevUserCallbackKey = nullptr;
	g_PrevUserCallbackWindowFocus = nullptr;
	g_PrevUserCallbackCursorPos = nullptr;
	g_PrevUserCallbackMonitor = nullptr;
	if (install_callbacks)
	{
		g_InstalledCallbacks = true;
		g_PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplCC_MouseButtonCallback);
		g_PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplCC_ScrollCallback);
		g_PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplCC_KeyCallback);
		g_PrevUserCallbackWindowFocus = glfwSetWindowFocusCallback(window, ImGui_ImplCC_WindowFocusCallback);
		g_PrevUserCallbackCursorPos = glfwSetCursorPosCallback(window, ImGui_ImplCC_CursorPosCallback);
		g_PrevUserCallbackMonitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
	}

	// Update monitors the first time (note: monitor callback are broken in GLFW 3.2 and earlier, see github.com/glfw/glfw/issues/784)
	ImGui_ImplGlfw_UpdateMonitors();

	// Our mouse update function expect PlatformHandle to be filled for the main viewport
	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	main_viewport->PlatformHandle = (void*)window;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	main_viewport->PlatformHandleRaw = glfwGetWin32Window(window);
#endif
	ImGui_ImplGlfw_InitPlatformInterface();
#else
	/*
	auto e = cocos2d::EventListenerMouse::create();
	e->onMouseDown = [](cocos2d::EventMouse* ev)
	{
		const auto b = (int)ev->getMouseButton();
		if (0 <= b && b < 5)
			g_MouseJustPressed[b] = true;
	};
	e->onMouseUp = [](cocos2d::EventMouse* ev)
	{
		const auto b = (int)ev->getMouseButton();
		if (0 <= b && b < 5)
			g_MouseJustPressed[b] = false;
	};
	e->onMouseMove = [](cocos2d::EventMouse* ev)
	{
		g_CursorPos.x = ev->getCursorX();
		g_CursorPos.y = ev->getCursorY();
	};
	e->onMouseScroll = [](cocos2d::EventMouse* ev)
	{
		auto& _io = ImGui::GetIO();
		_io.MouseWheelH += (float)ev->getScrollX();
		_io.MouseWheel += (float)ev->getScrollY();
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(e, 1);
	*/
	auto e1 = cc::EventListenerTouchOneByOne::create();
	//e1->setSwallowTouches(true);
	e1->onTouchBegan = [](Touch* touch, Event*)
	{
		if (g_TouchId != -1) return false;
		g_TouchId = touch->getID();
		const auto loc = touch->getLocationInView();
		g_CursorPos.x = loc.x;
		g_CursorPos.y = loc.y;
		g_MouseJustPressed[0] = true;
		return true;
	};
	e1->onTouchMoved = [](Touch* touch, Event*)
	{
		if (g_TouchId != touch->getID()) return;
		const auto loc = touch->getLocationInView();
		g_CursorPos.x = loc.x;
		g_CursorPos.y = loc.y;
		g_MouseJustPressed[0] = true;
	};
	e1->onTouchEnded = [](Touch* touch, Event*)
	{
		if (g_TouchId != touch->getID()) return;
		g_TouchId = -1;
		const auto loc = touch->getLocationInView();
		g_CursorPos.x = loc.x;
		g_CursorPos.y = loc.y;
		g_MouseJustPressed[0] = false;
	};
	e1->onTouchCancelled = [](Touch* touch, Event*)
	{
		if (g_TouchId != touch->getID()) return;
		g_TouchId = -1;
		g_CursorPos = ImVec2(-FLT_MAX, -FLT_MAX);
		g_MouseJustPressed[0] = false;
	};
	cc::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(e1, -1);
	g_TouchListener = e1;

	using KeyCode = cc::EventKeyboard::KeyCode;
	std::fill(std::begin(g_MobileKeys), std::end(g_MobileKeys), false);

	auto e2 = cc::EventListenerKeyboard::create();
	e2->onKeyPressed = [](KeyCode k, cc::Event*)
	{
		ImGui_ImplCC_MobileKey(k, true);
	};
	e2->onKeyReleased = [](KeyCode k, cc::Event*)
	{
		ImGui_ImplCC_MobileKey(k, false);
	};
	cc::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(e2, 1);
	g_KeyboardListener = e2;
#endif // CC_PLATFORM_PC
	return true;
}

void ImGui_ImplCC_Shutdown()
{
#ifndef CC_PLATFORM_PC
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(g_TouchListener);
	dispatcher->removeEventListener(g_KeyboardListener);
	g_TouchListener = nullptr;
	g_KeyboardListener = nullptr;
	g_TouchId = -1;
	g_CursorPos = ImVec2(-FLT_MAX, -FLT_MAX);
	std::fill(std::begin(g_MouseJustPressed), std::end(g_MouseJustPressed), false);
#endif
#ifdef CC_PLATFORM_PC
	ImGui_ImplGlfw_ShutdownPlatformInterface();
	const auto g_Window = ImGui_ImplCC_GetWindow();
	if (g_InstalledCallbacks)
	{
		if (g_Window)
		{
			glfwSetMouseButtonCallback(g_Window, g_PrevUserCallbackMousebutton);
			glfwSetScrollCallback(g_Window, g_PrevUserCallbackScroll);
			glfwSetKeyCallback(g_Window, g_PrevUserCallbackKey);
			glfwSetWindowFocusCallback(g_Window, g_PrevUserCallbackWindowFocus);
			glfwSetCursorPosCallback(g_Window, g_PrevUserCallbackCursorPos);
		}
		glfwSetMonitorCallback(g_PrevUserCallbackMonitor);
		g_InstalledCallbacks = false;
	}
	std::fill(std::begin(g_KeyOwnerWindows), std::end(g_KeyOwnerWindows), nullptr);

	for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	{
		glfwDestroyCursor(g_MouseCursors[cursor_n]);
		g_MouseCursors[cursor_n] = nullptr;
	}
#endif // CC_PLATFORM_PC
	ImGui::DestroyPlatformWindows();
	delete ImGui_ImplCC_GetTextInput(ImGui::GetCurrentContext());
	ImGui::GetIO().BackendPlatformUserData = nullptr;
	ImGui::GetPlatformIO().Platform_SetImeDataFn = nullptr;
	ImGui_ImplCC_DestroyDeviceObjects();
	ImGui::GetIO().BackendFlags &= ~ImGuiBackendFlags_RendererHasTextures;
	ImGui::DestroyContext();
}

static void ImGui_ImplCC_UpdateMousePosAndButtons()
{
#ifdef CC_PLATFORM_PC
	const auto g_Window = ImGui_ImplCC_GetWindow();
	ImGuiIO& io = ImGui::GetIO();
	// Poll only when the application elected not to install our callbacks.
	if (!g_InstalledCallbacks)
		for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); ++i)
			io.AddMouseButtonEvent(i, glfwGetMouseButton(g_Window, i) == GLFW_PRESS);
	const ImVec2 mouse_pos_backup = io.MousePos;
	ImVec2 mouse_pos(-FLT_MAX, -FLT_MAX);
	ImGuiID hovered_viewport = 0;
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	for (int n = 0; n < platform_io.Viewports.Size; n++)
	{
		ImGuiViewport* viewport = platform_io.Viewports[n];
		GLFWwindow* window = (GLFWwindow*)viewport->PlatformHandle;
		if (!window)
			continue;
#ifdef __EMSCRIPTEN__
		const bool focused = true;
		IM_ASSERT(platform_io.Viewports.Size == 1);
#else
		const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
#endif
		if (focused)
		{
			if (io.WantSetMousePos)
			{
				glfwSetCursorPos(window, (double)(mouse_pos_backup.x - viewport->Pos.x), (double)(mouse_pos_backup.y - viewport->Pos.y));
			}
			else
			{
				double mouse_x, mouse_y;
				glfwGetCursorPos(window, &mouse_x, &mouse_y);
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					// Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
					int window_x, window_y;
					glfwGetWindowPos(window, &window_x, &window_y);
					mouse_pos = ImVec2((float)mouse_x + window_x, (float)mouse_y + window_y);
				}
				else
				{
					// Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
					mouse_pos = ImVec2((float)mouse_x, (float)mouse_y);
				}
			}
		}

		// (Optional) When using multiple viewports: set io.MouseHoveredViewport to the viewport the OS mouse cursor is hovering.
		// Important: this information is not easy to provide and many high-level windowing library won't be able to provide it correctly, because
		// - This is _ignoring_ viewports with the ImGuiViewportFlags_NoInputs flag (pass-through windows).
		// - This is _regardless_ of whether another viewport is focused or being dragged from.
		// If ImGuiBackendFlags_HasMouseHoveredViewport is not set by the backend, imgui will ignore this field and infer the information by relying on the
		// rectangles and last focused time of every viewports it knows about. It will be unaware of other windows that may be sitting between or over your windows.
		// [GLFW] FIXME: This is currently only correct on Win32. See what we do below with the WM_NCHITTEST, missing an equivalent for other systems.
		// See https://github.com/glfw/glfw/issues/1236 if you want to help in making this a GLFW feature.
#if GLFW_HAS_MOUSE_PASSTHROUGH || (GLFW_HAS_WINDOW_HOVERED && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		const bool window_no_input = (viewport->Flags & ImGuiViewportFlags_NoInputs) != 0;
#if GLFW_HAS_MOUSE_PASSTHROUGH
		glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, window_no_input);
#endif
		if (glfwGetWindowAttrib(window, GLFW_HOVERED) && !window_no_input)
			hovered_viewport = viewport->ID;
#endif
	}
	if (io.WantSetMousePos) mouse_pos = mouse_pos_backup;
	io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);
	io.AddMouseViewportEvent(hovered_viewport);
#else
	ImGuiIO& io = ImGui::GetIO();

	// Mark the source so ImGui applies its touch-specific input rules.
	io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);

	// Queue the position before the buttons: ImGui trickles the input queue and
	// would defer a position queued after a button change, dropping the first tap.
	const ImVec2 mouse_pos_backup = io.MousePos;
	ImVec2 mouse_pos(-FLT_MAX, -FLT_MAX);
	if (io.WantSetMousePos)
	{
		mouse_pos = mouse_pos_backup;
	}
	else
	{
		if (g_CursorPos.x != -FLT_MAX && g_CursorPos.y != -FLT_MAX)
		{
			// convert g_CursorPos
			const auto glv = cc::Director::getInstance()->getOpenGLView();
			const auto rect = glv->getViewPortRect();
			mouse_pos.x = g_CursorPos.x * glv->getScaleX() + rect.origin.x;
			mouse_pos.y = g_CursorPos.y * glv->getScaleY() + rect.origin.y;
		}
	}
	io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);

	// g_MouseJustPressed represents touch state on mobile platforms
	for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
	{
		io.AddMouseButtonEvent(i, g_MouseJustPressed[i]);
	}
#endif // CC_PLATFORM_PC
}

static void ImGui_ImplCC_UpdateMouseCursor()
{
#ifdef CC_PLATFORM_PC
	const auto g_Window = ImGui_ImplCC_GetWindow();
	ImGuiIO& io = ImGui::GetIO();
	if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(g_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	for (int n = 0; n < platform_io.Viewports.Size; n++)
	{
		GLFWwindow* window = (GLFWwindow*)platform_io.Viewports[n]->PlatformHandle;
		if (!window)
			continue;
		if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
		{
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else
		{
			// Show OS mouse cursor
			// FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
			glfwSetCursor(window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
#endif // CC_PLATFORM_PC
}

void ImGui_ImplCC_SetGamepadPlayer(unsigned player)
{
	if (auto* context = ImGui::GetCurrentContext())
		if (auto* state = ImGui_ImplCC_GetTextInput(context))
			state->gamepadPlayer = player;
}

static void ImGui_ImplCC_ClearGamepad(ImGuiIO& io)
{
	for (int key = ImGuiKey_GamepadStart; key <= ImGuiKey_GamepadRStickDown; ++key)
		io.AddKeyAnalogEvent(static_cast<ImGuiKey>(key), false, 0.0f);
}

static void ImGui_ImplCC_UpdateNativeGamepad(ImGuiIO& io)
{
	using namespace cc::input;
	auto* state = ImGui_ImplCC_GetTextInput(ImGui::GetCurrentContext());
	const auto frame = Director::getInstance()->getGamepads().frame();
	std::optional<Pad> selected;
	DeviceId id{};
	if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)
	{
		const auto devices = frame.devices();
		for (auto device : devices.value.value_or(std::span<const DeviceId>{}))
		{
			const auto found = frame.find(device);
			if (!found || !*found.value)
				continue;
			const auto& pad = **found.value;
			if (pad.mapped().value.value_or(false) && pad.player().value.value_or(std::nullopt) == state->gamepadPlayer)
			{
				selected = pad;
				id = device;
				break;
			}
		}
	}
	const bool changed = id != state->gamepadDevice;
	if (changed && state->gamepadDevice != DeviceId{})
		ImGui_ImplCC_ClearGamepad(io);
	state->gamepadDevice = id;
	if (!selected)
		return;

	io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	constexpr std::pair<Button, ImGuiKey> buttons[]{
		{Button::Start, ImGuiKey_GamepadStart}, {Button::Back, ImGuiKey_GamepadBack},
		{Button::West, ImGuiKey_GamepadFaceLeft}, {Button::East, ImGuiKey_GamepadFaceRight},
		{Button::North, ImGuiKey_GamepadFaceUp}, {Button::South, ImGuiKey_GamepadFaceDown},
		{Button::DpadLeft, ImGuiKey_GamepadDpadLeft}, {Button::DpadRight, ImGuiKey_GamepadDpadRight},
		{Button::DpadUp, ImGuiKey_GamepadDpadUp}, {Button::DpadDown, ImGuiKey_GamepadDpadDown},
		{Button::LeftShoulder, ImGuiKey_GamepadL1}, {Button::RightShoulder, ImGuiKey_GamepadR1},
		{Button::LeftStick, ImGuiKey_GamepadL3}, {Button::RightStick, ImGuiKey_GamepadR3},
	};
	const auto tick = frame.tick().value.value_or(0);
	if (!changed && state->gamepadTick != tick)
	{
		if (auto events = frame.events())
		{
			auto& cursor = *events.value;
			for (auto next = cursor.next(); next && *next.value; next = cursor.next())
			{
				const auto& event = **next.value;
				const auto* edge = event.device == id ? std::get_if<ButtonEdge>(&event.payload) : nullptr;
				if (!edge)
					continue;
				for (const auto& [button, key] : buttons)
					if (edge->button == button)
						io.AddKeyEvent(key, edge->down);
			}
		}
	}
	state->gamepadTick = tick;
	for (const auto& [button, key] : buttons)
		io.AddKeyEvent(key, selected->down(button).value.value_or(false));

	const auto analog = [&](ImGuiKey key, Axis axis, float sign, float deadZone) {
		const auto axisValue = selected->axis(axis).value.value_or(std::nullopt);
		const auto value = std::clamp((axisValue.value_or(0) * sign - deadZone) / (1 - deadZone), 0.0f, 1.0f);
		io.AddKeyAnalogEvent(key, value > 0.1f, value);
	};
	analog(ImGuiKey_GamepadL2, Axis::LeftTrigger, 1, 0.125f);
	analog(ImGuiKey_GamepadR2, Axis::RightTrigger, 1, 0.125f);
	analog(ImGuiKey_GamepadLStickLeft, Axis::LeftX, -1, 0.25f);
	analog(ImGuiKey_GamepadLStickRight, Axis::LeftX, 1, 0.25f);
	analog(ImGuiKey_GamepadLStickUp, Axis::LeftY, -1, 0.25f);
	analog(ImGuiKey_GamepadLStickDown, Axis::LeftY, 1, 0.25f);
	analog(ImGuiKey_GamepadRStickLeft, Axis::RightX, -1, 0.25f);
	analog(ImGuiKey_GamepadRStickRight, Axis::RightX, 1, 0.25f);
	analog(ImGuiKey_GamepadRStickUp, Axis::RightY, -1, 0.25f);
	analog(ImGuiKey_GamepadRStickDown, Axis::RightY, 1, 0.25f);
}
static void ImGui_ImplCC_UpdateGamepads()
{
	auto& io = ImGui::GetIO();
	io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
	if (ImGui_ImplCC_GetTextInput(ImGui::GetCurrentContext()))
		ImGui_ImplCC_UpdateNativeGamepad(io);
}

static void ImGui_ImplGlfw_UpdateMonitors()
{
#ifdef CC_PLATFORM_PC
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	int monitors_count = 0;
	GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitors_count);
	platform_io.Monitors.resize(0);
	for (int n = 0; n < monitors_count; n++)
	{
		ImGuiPlatformMonitor monitor;
		int x, y;
		glfwGetMonitorPos(glfw_monitors[n], &x, &y);
		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfw_monitors[n]);
		if (!vid_mode || vid_mode->width <= 0 || vid_mode->height <= 0) continue;
		monitor.MainPos = monitor.WorkPos = ImVec2((float)x, (float)y);
		monitor.MainSize = monitor.WorkSize = ImVec2((float)vid_mode->width, (float)vid_mode->height);
#if GLFW_HAS_MONITOR_WORK_AREA
		int w, h;
		glfwGetMonitorWorkarea(glfw_monitors[n], &x, &y, &w, &h);
		if (w > 0 && h > 0) // Workaround a small GLFW issue reporting zero on monitor changes: https://github.com/glfw/glfw/pull/1761
		{
			monitor.WorkPos = ImVec2((float)x, (float)y);
			monitor.WorkSize = ImVec2((float)w, (float)h);
		}
#endif
#if GLFW_HAS_PER_MONITOR_DPI
		// Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
		float x_scale, y_scale;
		glfwGetMonitorContentScale(glfw_monitors[n], &x_scale, &y_scale);
		monitor.DpiScale = x_scale;
#endif
		platform_io.Monitors.push_back(monitor);
	}
	g_WantUpdateMonitors = false;
#endif // CC_PLATFORM_PC
}

void ImGui_ImplCC_NewFrame()
{
	if (auto* input = ImGui_ImplCC_GetTextInput(ImGui::GetCurrentContext()))
		input->newFrame();
	g_CallbackCommands.clear();
	g_CustomCommands.clear();
	g_ProgramStates.clear();
	g_DrawTextures.clear();

	if (!g_ProgramInfo.program || !g_ProgramFontInfo.program)
		ImGui_ImplCC_CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	const auto* view = Director::getInstance()->getOpenGLView();
#ifdef CC_PLATFORM_PC
	const auto* desktopView = static_cast<const GLViewImpl*>(view);
	const auto windowSize = desktopView->getRenderWindowSize();
	const auto framebufferSize = desktopView->getRenderFramebufferSize();
#else
	const auto windowSize = view->getFrameSize();
	const auto framebufferSize = windowSize;
#endif // CC_PLATFORM_PC
	io.DisplaySize = ImVec2(windowSize.width, windowSize.height);
	if (windowSize.width > 0 && windowSize.height > 0)
		io.DisplayFramebufferScale = ImVec2(framebufferSize.width / windowSize.width,
			framebufferSize.height / windowSize.height);
#ifdef CC_PLATFORM_PC
	if (g_WantUpdateMonitors)
		ImGui_ImplGlfw_UpdateMonitors();
#endif // CC_PLATFORM_PC

	// Setup time step
	const auto deltaTime = Director::getInstance()->getDeltaTime();
	// cocos resets the next frame's delta to zero after changing FPS or resuming.
	// ImGui requires a positive time step even on those reset frames.
	io.DeltaTime = deltaTime > 0.0f ? deltaTime : 1.0f / 60.0f;

	ImGui_ImplCC_UpdateMousePosAndButtons();
	ImGui_ImplCC_UpdateMouseCursor();

	// Update game controllers (if enabled and available)
	ImGui_ImplCC_UpdateGamepads();

	ImGui::NewFrame();
}

#ifdef CC_PLATFORM_PC

//--------------------------------------------------------------------------------------------------------
// MULTI-VIEWPORT / PLATFORM INTERFACE SUPPORT
// This is an _advanced_ and _optional_ feature, allowing the backend to create and handle multiple viewports simultaneously.
// If you are new to dear imgui or creating a new binding for dear imgui, it is recommended that you completely ignore this section first..
//--------------------------------------------------------------------------------------------------------

// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
struct ImGuiViewportDataGlfw
{
	GLFWwindow* Window;
	bool        WindowOwned;
	int         IgnoreWindowPosEventFrame;
	int         IgnoreWindowSizeEventFrame;

	ImGuiViewportDataGlfw()  { Window = NULL; WindowOwned = false; IgnoreWindowSizeEventFrame = IgnoreWindowPosEventFrame = -1; }
	~ImGuiViewportDataGlfw() { IM_ASSERT(Window == NULL); }
};

static void ImGui_ImplGlfw_WindowCloseCallback(GLFWwindow* window)
{
	if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
		viewport->PlatformRequestClose = true;
}

// GLFW may dispatch window pos/size events after calling glfwSetWindowPos()/glfwSetWindowSize().
// However: depending on the platform the callback may be invoked at different time:
// - on Windows it appears to be called within the glfwSetWindowPos()/glfwSetWindowSize() call
// - on Linux it is queued and invoked during glfwPollEvents()
// Because the event doesn't always fire on glfwSetWindowXXX() we use a frame counter tag to only
// ignore recent glfwSetWindowXXX() calls.
static void ImGui_ImplGlfw_WindowPosCallback(GLFWwindow* window, int, int)
{
	TextInputGLFW::handleGeometry(window);
	if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
	{
		if (ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData)
		{
			bool ignore_event = (ImGui::GetFrameCount() <= data->IgnoreWindowPosEventFrame + 1);
			//data->IgnoreWindowPosEventFrame = -1;
			if (ignore_event)
				return;
		}
		viewport->PlatformRequestMove = true;
	}
}

static void ImGui_ImplGlfw_WindowSizeCallback(GLFWwindow* window, int, int)
{
	TextInputGLFW::handleGeometry(window);
	if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
	{
		if (ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData)
		{
			bool ignore_event = (ImGui::GetFrameCount() <= data->IgnoreWindowSizeEventFrame + 1);
			//data->IgnoreWindowSizeEventFrame = -1;
			if (ignore_event)
				return;
		}
		viewport->PlatformRequestResize = true;
	}
}

static void ImGui_ImplGlfw_CreateWindow(ImGuiViewport* viewport)
{
	ImGuiViewportDataGlfw* data = IM_NEW(ImGuiViewportDataGlfw)();
	viewport->PlatformUserData = data;

	// GLFW 3.2 unfortunately always set focus on glfwCreateWindow() if GLFW_VISIBLE is set, regardless of GLFW_FOCUSED
	// With GLFW 3.3, the hint GLFW_FOCUS_ON_SHOW fixes this problem
	glfwWindowHint(GLFW_VISIBLE, false);
	glfwWindowHint(GLFW_FOCUSED, false);
#if GLFW_HAS_FOCUS_ON_SHOW
	glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);
#endif
	glfwWindowHint(GLFW_DECORATED, (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true);
#if GLFW_HAS_WINDOW_TOPMOST
	glfwWindowHint(GLFW_FLOATING, (viewport->Flags & ImGuiViewportFlags_TopMost) ? true : false);
#endif
	// gfx renderer does not own a shareable GL context
	GLFWwindow* share_window = nullptr;
	data->Window = glfwCreateWindow((int)viewport->Size.x, (int)viewport->Size.y, "No Title Yet", nullptr, share_window);
	if (!data->Window)
		return;
	data->WindowOwned = true;
	viewport->PlatformHandle = (void*)data->Window;
	if (const auto view = Director::getInstance()->getOpenGLView())
		TextInputGLFW::attachWindow(view->getTextInputService(), data->Window);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	viewport->PlatformHandleRaw = glfwGetWin32Window(data->Window);
#endif
	glfwSetWindowPos(data->Window, (int)viewport->Pos.x, (int)viewport->Pos.y);

	// Install GLFW callbacks for secondary viewports
	glfwSetMouseButtonCallback(data->Window, ImGui_ImplCC_MouseButtonCallback);
	glfwSetScrollCallback(data->Window, ImGui_ImplCC_ScrollCallback);
	glfwSetKeyCallback(data->Window, ImGui_ImplCC_KeyCallback);
	glfwSetCharCallback(data->Window, ImGui_ImplCC_CharCallback);
	glfwSetWindowFocusCallback(data->Window, ImGui_ImplCC_WindowFocusCallback);
	glfwSetCursorPosCallback(data->Window, ImGui_ImplCC_CursorPosCallback);
	glfwSetWindowCloseCallback(data->Window, ImGui_ImplGlfw_WindowCloseCallback);
	glfwSetWindowPosCallback(data->Window, ImGui_ImplGlfw_WindowPosCallback);
	glfwSetWindowSizeCallback(data->Window, ImGui_ImplGlfw_WindowSizeCallback);
	glfwSetWindowContentScaleCallback(data->Window, [](GLFWwindow* window, float, float) {
		TextInputGLFW::handleGeometry(window);
	});
}

static void ImGui_ImplGlfw_DestroyWindow(ImGuiViewport* viewport)
{
	if (auto* input = ImGui_ImplCC_GetTextInput(ImGui::GetCurrentContext()))
		input->windowDestroyed(viewport->ID);
	if (ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData)
	{
		if (data->WindowOwned)
		{
			const auto window = data->Window;
			TextInputGLFW::detachWindow(window);
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			HWND hwnd = (HWND)viewport->PlatformHandleRaw;
			::RemovePropA(hwnd, "IMGUI_VIEWPORT");
#endif

			// Release any keys that were pressed in the window being destroyed and are still held down,
			// because we will not receive any release events after window is destroyed.
			for (int i = 0; i < IM_ARRAYSIZE(g_KeyOwnerWindows); i++)
				if (g_KeyOwnerWindows[i] == window)
					ImGui_ImplCC_KeyCallback(window, i, 0, GLFW_RELEASE, 0); // Later params are only used for main viewport, on which this function is never called.

			glfwDestroyWindow(window);
		}
		data->Window = nullptr;
		IM_DELETE(data);
	}
	viewport->PlatformUserData = viewport->PlatformHandle = viewport->PlatformHandleRaw = nullptr;
}

// We have submitted https://github.com/glfw/glfw/pull/1568 to allow GLFW to support "transparent inputs".
// In the meanwhile we implement custom per-platform workarounds here (FIXME-VIEWPORT: Implement same work-around for Linux/OSX!)
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
static WNDPROC g_GlfwWndProc = NULL;
static LRESULT CALLBACK WndProcNoInputs(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCHITTEST)
	{
		// Let mouse pass-through the window. This will allow the back-end to set io.MouseHoveredViewport properly (which is OPTIONAL).
		// The ImGuiViewportFlags_NoInputs flag is set while dragging a viewport, as want to detect the window behind the one we are dragging.
		// If you cannot easily access those viewport flags from your windowing/event code: you may manually synchronize its state e.g. in
		// your main loop after calling UpdatePlatformWindows(). Iterate all viewports/platform windows and pass the flag to your windowing system.
		ImGuiViewport* viewport = (ImGuiViewport*)::GetPropA(hWnd, "IMGUI_VIEWPORT");
		if (viewport && (viewport->Flags & ImGuiViewportFlags_NoInputs))
			return HTTRANSPARENT;
	}
	return ::CallWindowProc(g_GlfwWndProc, hWnd, msg, wParam, lParam);
}
#endif

static void ImGui_ImplGlfw_ShowWindow(ImGuiViewport* viewport)
{
	ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
	if (!data->Window)
		return;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// GLFW hack: Hide icon from task bar
	HWND hwnd = (HWND)viewport->PlatformHandleRaw;
	if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
	{
		LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		ex_style &= ~WS_EX_APPWINDOW;
		ex_style |= WS_EX_TOOLWINDOW;
		::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
	}

	// GLFW hack: install hook for WM_NCHITTEST message handler
#if !GLFW_HAS_MOUSE_PASSTHROUGH && GLFW_HAS_WINDOW_HOVERED
	::SetPropA(hwnd, "IMGUI_VIEWPORT", viewport);
	if (g_GlfwWndProc == NULL)
		g_GlfwWndProc = (WNDPROC)::GetWindowLongPtr(hwnd, GWLP_WNDPROC);
	::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcNoInputs);
#endif

#if !GLFW_HAS_FOCUS_ON_SHOW
	// GLFW hack: GLFW 3.2 has a bug where glfwShowWindow() also activates/focus the window.
	// The fix was pushed to GLFW repository on 2018/01/09 and should be included in GLFW 3.3 via a GLFW_FOCUS_ON_SHOW window attribute.
	// See https://github.com/glfw/glfw/issues/1189
	// FIXME-VIEWPORT: Implement same work-around for Linux/OSX in the meanwhile.
	if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
	{
		::ShowWindow(hwnd, SW_SHOWNA);
		return;
	}
#endif
#endif

	glfwShowWindow(data->Window);
}

static ImVec2 ImGui_ImplGlfw_GetWindowPos(ImGuiViewport* viewport)
{
	auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle);
	if (!window)
		return viewport->Pos;
	int x = 0, y = 0;
	glfwGetWindowPos(window, &x, &y);
	return ImVec2((float)x, (float)y);
}

static void ImGui_ImplGlfw_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
{
	ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
	if (!data || !data->Window)
		return;
	data->IgnoreWindowPosEventFrame = ImGui::GetFrameCount();
	glfwSetWindowPos(data->Window, (int)pos.x, (int)pos.y);
}

static ImVec2 ImGui_ImplGlfw_GetWindowSize(ImGuiViewport* viewport)
{
	auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle);
	if (!window)
		return viewport->Size;
	int w = 0, h = 0;
	glfwGetWindowSize(window, &w, &h);
	return ImVec2((float)w, (float)h);
}

static void ImGui_ImplGlfw_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
	ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
	if (!data || !data->Window)
		return;
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC && !GLFW_HAS_OSX_WINDOW_POS_FIX
	// Native OS windows are positioned from the bottom-left corner on macOS, whereas on other platforms they are
	// positioned from the upper-left corner. GLFW makes an effort to convert macOS style coordinates, however it
	// doesn't handle it when changing size. We are manually moving the window in order for changes of size to be based
	// on the upper-left corner.
	int x, y, width, height;
	glfwGetWindowPos(data->Window, &x, &y);
	glfwGetWindowSize(data->Window, &width, &height);
	glfwSetWindowPos(data->Window, x, y - height + size.y);
#endif
	data->IgnoreWindowSizeEventFrame = ImGui::GetFrameCount();
	glfwSetWindowSize(data->Window, (int)size.x, (int)size.y);
}

static void ImGui_ImplGlfw_SetWindowTitle(ImGuiViewport* viewport, const char* title)
{
	if (auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle))
		glfwSetWindowTitle(window, title);
}

static void ImGui_ImplGlfw_SetWindowFocus(ImGuiViewport* viewport)
{
#if GLFW_HAS_FOCUS_WINDOW
	if (auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle))
		glfwFocusWindow(window);
#else
	// FIXME: What are the effect of not having this function? At the moment imgui doesn't actually call SetWindowFocus - we set that up ahead, will answer that question later.
	(void)viewport;
#endif
}

static bool ImGui_ImplGlfw_GetWindowFocus(ImGuiViewport* viewport)
{
	auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle);
	return window && glfwGetWindowAttrib(window, GLFW_FOCUSED) == GLFW_TRUE;
}

static bool ImGui_ImplGlfw_GetWindowMinimized(ImGuiViewport* viewport)
{
	auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle);
	return !window || glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GLFW_TRUE;
}

#if GLFW_HAS_WINDOW_ALPHA
static void ImGui_ImplGlfw_SetWindowAlpha(ImGuiViewport* viewport, float alpha)
{
	if (auto* window = static_cast<GLFWwindow*>(viewport->PlatformHandle))
		glfwSetWindowOpacity(window, alpha);
}
#endif

static void ImGui_ImplGlfw_RenderWindow(ImGuiViewport* viewport, void*)
{
	// gfx renderer does not render into secondary platform windows
}

static void ImGui_ImplGlfw_SwapBuffers(ImGuiViewport* viewport, void*)
{
	// gfx renderer presents through its own swapchain; nothing to do here
}

//--------------------------------------------------------------------------------------------------------
// Vulkan support (the Vulkan renderer needs to call a platform-side support function to create the surface)
//--------------------------------------------------------------------------------------------------------

// Avoid including <vulkan.h> so we can build without it
#if 0 && GLFW_HAS_VULKAN
#ifndef VULKAN_H_
#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
#if defined(__LP64__) || defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef struct object##_T *object;
#else
#define VK_DEFINE_NON_DISPATCHABLE_HANDLE(object) typedef uint64_t object;
#endif
VK_DEFINE_HANDLE(VkInstance)
VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkSurfaceKHR)
struct VkAllocationCallbacks;
enum VkResult { VK_RESULT_MAX_ENUM = 0x7FFFFFFF };
#endif // VULKAN_H_
extern "C" { extern GLFWAPI VkResult glfwCreateWindowSurface(VkInstance instance, GLFWwindow* window, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface); }
static int ImGui_ImplCC_CreateVkSurface(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface)
{
	ImGuiViewportDataGlfw* data = (ImGuiViewportDataGlfw*)viewport->PlatformUserData;
	IM_ASSERT(g_ClientApi == GlfwClientApi_Vulkan);
	VkResult err = glfwCreateWindowSurface((VkInstance)vk_instance, data->Window, (const VkAllocationCallbacks*)vk_allocator, (VkSurfaceKHR*)out_vk_surface);
	return (int)err;
}
#endif // GLFW_HAS_VULKAN

static void ImGui_ImplGlfw_InitPlatformInterface()
{
	// Register platform interface (will be coupled with a renderer interface)
	ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
	platform_io.Platform_CreateWindow = ImGui_ImplGlfw_CreateWindow;
	platform_io.Platform_DestroyWindow = ImGui_ImplGlfw_DestroyWindow;
	platform_io.Platform_ShowWindow = ImGui_ImplGlfw_ShowWindow;
	platform_io.Platform_SetWindowPos = ImGui_ImplGlfw_SetWindowPos;
	platform_io.Platform_GetWindowPos = ImGui_ImplGlfw_GetWindowPos;
	platform_io.Platform_SetWindowSize = ImGui_ImplGlfw_SetWindowSize;
	platform_io.Platform_GetWindowSize = ImGui_ImplGlfw_GetWindowSize;
	platform_io.Platform_SetWindowFocus = ImGui_ImplGlfw_SetWindowFocus;
	platform_io.Platform_GetWindowFocus = ImGui_ImplGlfw_GetWindowFocus;
	platform_io.Platform_GetWindowMinimized = ImGui_ImplGlfw_GetWindowMinimized;
	platform_io.Platform_SetWindowTitle = ImGui_ImplGlfw_SetWindowTitle;
	platform_io.Platform_RenderWindow = ImGui_ImplGlfw_RenderWindow;
	platform_io.Platform_SwapBuffers = ImGui_ImplGlfw_SwapBuffers;
#if GLFW_HAS_WINDOW_ALPHA
	platform_io.Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
#endif
#if 0 && GLFW_HAS_VULKAN
	platform_io.Platform_CreateVkSurface = ImGui_ImplCC_CreateVkSurface;
#endif

	// Register main window handle (which is owned by the main application, not by us)
	// This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
	ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGuiViewportDataGlfw* data = IM_NEW(ImGuiViewportDataGlfw)();
	const auto window = ImGui_ImplCC_GetWindow();
	data->Window = window;
	data->WindowOwned = false;
	main_viewport->PlatformUserData = data;
	main_viewport->PlatformHandle = (void*)window;
}

static void ImGui_ImplGlfw_ShutdownPlatformInterface()
{
}

#endif // CC_PLATFORM_PC
