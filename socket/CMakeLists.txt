set(INC_DIR "inc")
set(SRC_DIR "src")
set(TEST_DIR "test")

# ---------- GOOGLE TEST BLOCK BEGIN ---------- #
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
# ----------  GOOGLE TEST BLOCK END  ----------- #




# ----------   TESTING BLOCK BEGIN   ----------- #
enable_testing()

# ADDRESS TEST
add_executable(address_tests ${TEST_DIR}/address_tests.cpp)
target_link_libraries(address_tests PUBLIC gtest_main)
target_include_directories(address_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${INC_DIR})

gtest_discover_tests(address_tests)
# ----------    TESTING BLOCK END    ----------- #