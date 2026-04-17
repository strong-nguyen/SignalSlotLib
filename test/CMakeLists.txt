include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

# For Windows: Fix the conflict run-time library (MT vs MD)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Declare test exe
add_executable(unit_tests "Test_DirectConnect.cpp" "SignalContainer.h" "SignalContainer.cpp")

# Link with gtest
target_link_libraries(unit_tests GTest::gtest_main SignalSlotLib)

# Allow cmake to discover test cases
include(GoogleTest)
gtest_discover_tests(unit_tests
    DISCOVERY_TIMEOUT 60
    PROPERTIES DISCOVERY_MODE PRE_TEST)