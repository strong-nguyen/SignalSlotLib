# src/CMakeLists.txt

# Khai báo thư viện interface (không tạo ra file .obj hay .lib)
add_library(SignalSlotLib INTERFACE)

# Chỉ định rằng bất kỳ ai link tới SignalSlotLib cũng sẽ tự động thấy các file header ở đây
target_include_directories(SignalSlotLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(SignalSlotLib INTERFACE cxx_std_20)