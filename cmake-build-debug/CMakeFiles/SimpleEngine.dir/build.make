# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\hlahm\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5457.51\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\hlahm\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\212.5457.51\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\hlahm\Documents\gitProjects\SimpleEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SimpleEngine.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/SimpleEngine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SimpleEngine.dir/flags.make

CMakeFiles/SimpleEngine.dir/source/main.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/source/main.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/source/main.cpp.obj: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SimpleEngine.dir/source/main.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\source\main.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\main.cpp

CMakeFiles/SimpleEngine.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/source/main.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\main.cpp > CMakeFiles\SimpleEngine.dir\source\main.cpp.i

CMakeFiles/SimpleEngine.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/source/main.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\main.cpp -o CMakeFiles\SimpleEngine.dir\source\main.cpp.s

CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj: ../source/window/ve_window.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\source\window\ve_window.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\window\ve_window.cpp

CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\window\ve_window.cpp > CMakeFiles\SimpleEngine.dir\source\window\ve_window.cpp.i

CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\window\ve_window.cpp -o CMakeFiles\SimpleEngine.dir\source\window\ve_window.cpp.s

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj: ../source/vulkan-backend/ve_backend.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_backend.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_backend.cpp

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_backend.cpp > CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_backend.cpp.i

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_backend.cpp -o CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_backend.cpp.s

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj: ../source/vulkan-backend/ve_validation_layer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_validation_layer.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_validation_layer.cpp

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_validation_layer.cpp > CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_validation_layer.cpp.i

CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\vulkan-backend\ve_validation_layer.cpp -o CMakeFiles\SimpleEngine.dir\source\vulkan-backend\ve_validation_layer.cpp.s

CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj: ../source/renderer/ve_renderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\source\renderer\ve_renderer.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\renderer\ve_renderer.cpp

CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\renderer\ve_renderer.cpp > CMakeFiles\SimpleEngine.dir\source\renderer\ve_renderer.cpp.i

CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\source\renderer\ve_renderer.cpp -o CMakeFiles\SimpleEngine.dir\source\renderer\ve_renderer.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj: ../external/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\imgui.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj: ../external/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_demo.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_demo.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_demo.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\imgui_demo.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_demo.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_demo.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj: ../external/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_draw.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_draw.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_draw.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\imgui_draw.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_draw.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_draw.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj: ../external/imgui/imgui_widgets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_widgets.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_widgets.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_widgets.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\imgui_widgets.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_widgets.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_widgets.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj: ../external/imgui/imgui_tables.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_tables.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_tables.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_tables.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\imgui_tables.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\imgui_tables.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\imgui_tables.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj: ../external/imgui/backends/imgui_impl_vulkan.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_vulkan.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_vulkan.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_vulkan.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_vulkan.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_vulkan.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_vulkan.cpp.s

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj: CMakeFiles/SimpleEngine.dir/flags.make
CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj: CMakeFiles/SimpleEngine.dir/includes_CXX.rsp
CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj: ../external/imgui/backends/imgui_impl_glfw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_glfw.cpp.obj -c C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_glfw.cpp

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.i"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_glfw.cpp > CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_glfw.cpp.i

CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.s"
	C:\msys64\mingw64\bin\clang++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\hlahm\Documents\gitProjects\SimpleEngine\external\imgui\backends\imgui_impl_glfw.cpp -o CMakeFiles\SimpleEngine.dir\external\imgui\backends\imgui_impl_glfw.cpp.s

# Object files for target SimpleEngine
SimpleEngine_OBJECTS = \
"CMakeFiles/SimpleEngine.dir/source/main.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj" \
"CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj"

# External object files for target SimpleEngine
SimpleEngine_EXTERNAL_OBJECTS =

SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/source/main.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/source/window/ve_window.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_backend.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/source/vulkan-backend/ve_validation_layer.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/source/renderer/ve_renderer.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/imgui.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/imgui_demo.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/imgui_draw.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/imgui_widgets.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/imgui_tables.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_vulkan.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/external/imgui/backends/imgui_impl_glfw.cpp.obj
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/build.make
SimpleEngine.exe: C:/GLFW/lib-mingw-w64/libglfw3.a
SimpleEngine.exe: C:/VulkanSDK/1.2.189.2/Lib/vulkan-1.lib
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/linklibs.rsp
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/objects1.rsp
SimpleEngine.exe: CMakeFiles/SimpleEngine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable SimpleEngine.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SimpleEngine.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SimpleEngine.dir/build: SimpleEngine.exe
.PHONY : CMakeFiles/SimpleEngine.dir/build

CMakeFiles/SimpleEngine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SimpleEngine.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SimpleEngine.dir/clean

CMakeFiles/SimpleEngine.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\hlahm\Documents\gitProjects\SimpleEngine C:\Users\hlahm\Documents\gitProjects\SimpleEngine C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug C:\Users\hlahm\Documents\gitProjects\SimpleEngine\cmake-build-debug\CMakeFiles\SimpleEngine.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SimpleEngine.dir/depend
