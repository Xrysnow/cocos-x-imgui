set(LIB_NAME cc_imgui)

set(HEADER
    CCIMGUI.h
    CCImGuiLayer.h
    CCImGuiColorTextEdit.h
    imgui_impl_cocos2dx.h
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/misc/cpp/imgui_stdlib.h
    imgui_markdown/imgui_markdown.h
    ImGuiColorTextEdit/TextEditor.h
    implot/implot.h
    implot/implot_internal.h
)

set(SOURCE
    CCIMGUI.cpp
    CCImGuiLayer.cpp
    CCImGuiColorTextEdit.cpp
    imgui_impl_cocos2dx.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
    ImGuiColorTextEdit/TextEditor.cpp
    implot/implot.cpp
    implot/implot_demo.cpp
    implot/implot_items.cpp
)

if(BUILD_LUA_LIBS)
    list(APPEND HEADER
        lua-bindings/imgui_lua.hpp
        lua-bindings/imgui_lua_internal.hpp
        lua-bindings/implot_lua.hpp
        lua-bindings/lua_imgui_auto.hpp
        lua-bindings/lua_imguiDrawList_auto.hpp
        lua-bindings/lua_imguiFont_auto.hpp
        lua-bindings/lua_imguiIO_auto.hpp
        lua-bindings/lua_imguiStyle_auto.hpp
        lua-bindings/lua_imguiViewport_auto.hpp
        lua-bindings/lua_ImGuiColorTextEdit_auto.hpp
        lua-bindings/lua_implot_auto.hpp
    )
    list(APPEND SOURCE
        lua-bindings/imgui_lua.cpp
        lua-bindings/implot_lua.cpp
        lua-bindings/lua_imguiDrawList_auto.cpp
        lua-bindings/lua_imguiFont_auto.cpp
        lua-bindings/lua_imguiIO_auto.cpp
        lua-bindings/lua_imguiStyle_auto.cpp
        lua-bindings/lua_imguiViewport_auto.cpp
        lua-bindings/lua_imgui_auto.cpp
        lua-bindings/lua_ImGuiColorTextEdit_auto.cpp
        lua-bindings/lua_implot_auto.cpp
    )
endif()

add_library(${LIB_NAME} STATIC ${SOURCE} ${HEADER})
target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp
)
# Inherit the core's headers and consumer definitions, including DLL imports.
target_link_libraries(${LIB_NAME} PUBLIC cocos2d)

if(BUILD_LUA_LIBS)
    target_include_directories(${LIB_NAME} PRIVATE lua-bindings)
    target_link_libraries(${LIB_NAME} PUBLIC luacocos2d)
endif()

if(WINDOWS OR MACOSX OR LINUX)
    # The platform backend calls GLFW directly, also with a shared cocos core.
    target_link_libraries(${LIB_NAME} PRIVATE glfw)
endif()
if(WINDOWS)
    target_link_libraries(${LIB_NAME} PRIVATE imm32)
endif()

set_target_properties(${LIB_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
if(XCODE OR VS)
    cocos_mark_code_files(${LIB_NAME})
endif()
