load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "closest_finger_mapping_test",
    srcs = ["closest_finger_mapping_test.cpp"],
    deps = [
        ":print_helpers",
        "//finger_keyboard_mapping:finger_to_key_mapping",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "hands_iteration_test",
    srcs = ["hands_iteration_test.cpp"],
    deps = [
        ":print_helpers",
        "//finger_keyboard_mapping:data_types",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "scancode_key_map_test",
    srcs = ["scancode_key_map_test.cpp"],
    deps = [
        "//finger_keyboard_mapping:data_types",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "keyboard_timeline_test",
    srcs = ["keyboard_timeline_test.cpp"],
    deps = [
        "//finger_keyboard_mapping:data_types",
        "@catch2//:catch2_main",
    ],
)

cc_library(
    name = "print_helpers",
    hdrs = ["print_helpers.hpp"],
    deps = ["//finger_keyboard_mapping:data_types"],
)
