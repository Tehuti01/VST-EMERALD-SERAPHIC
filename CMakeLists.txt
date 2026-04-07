cmake_minimum_required(VERSION 3.25)

project(SeraphicGenesis VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM.cmake
set(CPM_DOWNLOAD_VERSION 0.40.0)
if(NOT EXISTS "${CMAKE_BINARY_DIR}/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
  message(STATUS "Downloading CPM.cmake ${CPM_DOWNLOAD_VERSION}")
  file(DOWNLOAD
       "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
       "${CMAKE_BINARY_DIR}/CPM_${CPM_DOWNLOAD_VERSION}.cmake"
  )
endif()
include("${CMAKE_BINARY_DIR}/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

# Fetch JUCE
CPMAddPackage(
  NAME JUCE
  GITHUB_REPOSITORY juce-framework/JUCE
  GIT_TAG 8.0.0 # Or latest stable
  OPTIONS
    "JUCE_BUILD_EXAMPLES OFF"
    "JUCE_BUILD_EXTRAS OFF"
)

# Fetch Pamplejuce if we want to use its helpers (optional, let's keep it simple first)

# Define the plugin
juce_add_plugin(SeraphicGenesis
    COMPANY_NAME "Seraphic"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    FORMATS VST3 AU CLAP Standalone
    PRODUCT_NAME "Seraphic Genesis"
)

# Sources
target_sources(SeraphicGenesis PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/DSP/WavetableOscillator.cpp
    Source/DSP/WavetableOscillator.h
    Source/DSP/VitalityEngine.cpp
    Source/DSP/VitalityEngine.h
    Source/DSP/SeraphicFilter.h
    Source/UI/EmeraldTabletFrame.h
    Source/UI/SeraphicLookAndFeel.cpp
    Source/UI/SeraphicLookAndFeel.h
    Source/UI/EmeraldCoreShader.cpp
    Source/UI/EmeraldCoreShader.h
)

# Link JUCE modules
target_link_libraries(SeraphicGenesis PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_extra
    juce::juce_opengl
    juce::juce_dsp
)

# Compiler flags
if(MSVC)
    target_compile_options(SeraphicGenesis PRIVATE /W4)
else()
    target_compile_options(SeraphicGenesis PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- RELEASE STAGE: Organization & Packaging ---
set(RELEASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Release")

add_custom_command(TARGET SeraphicGenesis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Moving binaries to Release folder..."
    
    # Copy artifacts (using generic paths, CMAKE_BUILD_TYPE will dictate actual location)
    COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:SeraphicGenesis>" "${RELEASE_DIR}/Artifacts"
    
    # Zip the Artifacts for professional distribution while keeping the raw folder
    COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${RELEASE_DIR}/Seraphic_Genesis_v1.0.0.zip" --format=zip "${RELEASE_DIR}/Artifacts"
    
    COMMENT "Organizing release artifacts and creating distribution zip..."
)
