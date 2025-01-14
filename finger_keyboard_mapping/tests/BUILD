load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "closest_finger_mapping_test",
    srcs = ["closest_finger_mapping_test.cpp"],
    deps = [
        ":print_helpers",
        "//finger_keyboard_mapping/mapping:closest_finger_mapping",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "hands_iteration_test",
    srcs = ["hands_iteration_test.cpp"],
    deps = [
        ":print_helpers",
        "//finger_keyboard_mapping/hands:fingers_and_hands",
        "//finger_keyboard_mapping/hands:maybe_finger",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "scancode_key_map_test",
    srcs = ["scancode_key_map_test.cpp"],
    deps = [
        "//finger_keyboard_mapping/keyboard:scancode_key_map",
        "@catch2//:catch2_main",
    ],
)

cc_test(
    name = "keyboard_timeline_test",
    srcs = ["keyboard_timeline_test.cpp"],
    deps = [
        "//finger_keyboard_mapping:keyboard_timeline",
        "@catch2//:catch2_main",
    ],
)

cc_library(
    name = "print_helpers",
    hdrs = ["print_helpers.hpp"],
    deps = ["//finger_keyboard_mapping/hands:finger_print_helpers"],
)
