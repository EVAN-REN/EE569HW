# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ren/Documents/hw/EE569/HW4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ren/Documents/hw/EE569/HW4/build

# Include any dependencies generated for this target.
include CMakeFiles/p1_executable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/p1_executable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/p1_executable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/p1_executable.dir/flags.make

CMakeFiles/p1_executable.dir/p1/main.cpp.o: CMakeFiles/p1_executable.dir/flags.make
CMakeFiles/p1_executable.dir/p1/main.cpp.o: /Users/ren/Documents/hw/EE569/HW4/p1/main.cpp
CMakeFiles/p1_executable.dir/p1/main.cpp.o: CMakeFiles/p1_executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ren/Documents/hw/EE569/HW4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/p1_executable.dir/p1/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1_executable.dir/p1/main.cpp.o -MF CMakeFiles/p1_executable.dir/p1/main.cpp.o.d -o CMakeFiles/p1_executable.dir/p1/main.cpp.o -c /Users/ren/Documents/hw/EE569/HW4/p1/main.cpp

CMakeFiles/p1_executable.dir/p1/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/p1_executable.dir/p1/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ren/Documents/hw/EE569/HW4/p1/main.cpp > CMakeFiles/p1_executable.dir/p1/main.cpp.i

CMakeFiles/p1_executable.dir/p1/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/p1_executable.dir/p1/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ren/Documents/hw/EE569/HW4/p1/main.cpp -o CMakeFiles/p1_executable.dir/p1/main.cpp.s

CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o: CMakeFiles/p1_executable.dir/flags.make
CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o: /Users/ren/Documents/hw/EE569/HW4/p1/TextureAnalysis.cpp
CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o: CMakeFiles/p1_executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ren/Documents/hw/EE569/HW4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o -MF CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o.d -o CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o -c /Users/ren/Documents/hw/EE569/HW4/p1/TextureAnalysis.cpp

CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ren/Documents/hw/EE569/HW4/p1/TextureAnalysis.cpp > CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.i

CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ren/Documents/hw/EE569/HW4/p1/TextureAnalysis.cpp -o CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.s

CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o: CMakeFiles/p1_executable.dir/flags.make
CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o: /Users/ren/Documents/hw/EE569/HW4/p1/RWImage.cpp
CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o: CMakeFiles/p1_executable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/ren/Documents/hw/EE569/HW4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o -MF CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o.d -o CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o -c /Users/ren/Documents/hw/EE569/HW4/p1/RWImage.cpp

CMakeFiles/p1_executable.dir/p1/RWImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/p1_executable.dir/p1/RWImage.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ren/Documents/hw/EE569/HW4/p1/RWImage.cpp > CMakeFiles/p1_executable.dir/p1/RWImage.cpp.i

CMakeFiles/p1_executable.dir/p1/RWImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/p1_executable.dir/p1/RWImage.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ren/Documents/hw/EE569/HW4/p1/RWImage.cpp -o CMakeFiles/p1_executable.dir/p1/RWImage.cpp.s

# Object files for target p1_executable
p1_executable_OBJECTS = \
"CMakeFiles/p1_executable.dir/p1/main.cpp.o" \
"CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o" \
"CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o"

# External object files for target p1_executable
p1_executable_EXTERNAL_OBJECTS =

/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: CMakeFiles/p1_executable.dir/p1/main.cpp.o
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: CMakeFiles/p1_executable.dir/p1/TextureAnalysis.cpp.o
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: CMakeFiles/p1_executable.dir/p1/RWImage.cpp.o
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: CMakeFiles/p1_executable.dir/build.make
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_gapi.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_stitching.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_alphamat.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_aruco.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_bgsegm.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_bioinspired.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_ccalib.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_dnn_objdetect.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_dnn_superres.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_dpm.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_face.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_freetype.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_fuzzy.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_hfs.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_img_hash.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_intensity_transform.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_line_descriptor.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_mcc.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_quality.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_rapid.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_reg.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_rgbd.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_saliency.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_sfm.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_stereo.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_structured_light.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_superres.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_surface_matching.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_tracking.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_videostab.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_viz.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_wechat_qrcode.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_xfeatures2d.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_xobjdetect.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_xphoto.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_shape.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_highgui.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_datasets.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_plot.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_text.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_ml.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_phase_unwrapping.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_optflow.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_ximgproc.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_video.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_videoio.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_imgcodecs.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_objdetect.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_calib3d.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_dnn.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_features2d.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_flann.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_photo.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_imgproc.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: /opt/homebrew/lib/libopencv_core.4.9.0.dylib
/Users/ren/Documents/hw/EE569/HW4/bin/p1_executable: CMakeFiles/p1_executable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/ren/Documents/hw/EE569/HW4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable /Users/ren/Documents/hw/EE569/HW4/bin/p1_executable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/p1_executable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/p1_executable.dir/build: /Users/ren/Documents/hw/EE569/HW4/bin/p1_executable
.PHONY : CMakeFiles/p1_executable.dir/build

CMakeFiles/p1_executable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/p1_executable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/p1_executable.dir/clean

CMakeFiles/p1_executable.dir/depend:
	cd /Users/ren/Documents/hw/EE569/HW4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ren/Documents/hw/EE569/HW4 /Users/ren/Documents/hw/EE569/HW4 /Users/ren/Documents/hw/EE569/HW4/build /Users/ren/Documents/hw/EE569/HW4/build /Users/ren/Documents/hw/EE569/HW4/build/CMakeFiles/p1_executable.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/p1_executable.dir/depend
