load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:py_proto_library.bzl", "py_proto_library")
load("@pypi//:requirements.bzl", "requirement")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@rules_python//python:py_binary.bzl", "py_binary")

package(default_visibility = ["//:finger_keyboard_mapping"])

cc_binary(
    name = "finger_keyboard_mapping",
    srcs = ["main.cpp"],
    deps = [
        ":keyboard_timeline_proto_conversion",
        ":proto_conversion",
        "//finger_keyboard_mapping/hands:finger_print_helpers",
        "//finger_keyboard_mapping/keyboard:keyboard_shape_cc_proto",
        "//finger_keyboard_mapping/mapping:finger_to_key_mapping",
        "//finger_keyboard_mapping/mapping:map_keylog_timeling",
        "//keylogger:keylog_cc_proto",
        "@protobuf//:protobuf_headers",
    ],
)

cc_library(
    name = "geometry",
    hdrs = [
        "mapping_geometry.hpp",
        "mapping_geometry_literals.inl",
    ],
)

cc_library(
    name = "static_vector",
    hdrs = ["static_vector.hpp"],
)

cc_library(
    name = "keyboard_timeline",
    srcs = ["keyboard_timeline.cpp"],
    hdrs = ["keyboard_timeline.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//finger_keyboard_mapping/hands:fingers_and_hands",
        "//finger_keyboard_mapping/keyboard:key",
    ],
)

cc_library(
    name = "proto_conversion",
    hdrs = ["from_proto.hpp"],
    deps = [
        "//finger_keyboard_mapping/hands:fingers_and_hands",
        "//finger_landmarks:finger_landmarks_cc_proto",
    ],
)

proto_library(
    name = "keyboard_timeline_proto",
    srcs = ["keyboard_timeline.proto"],
)

cc_proto_library(
    name = "keyboard_timeline_cc_proto",
    deps = [":keyboard_timeline_proto"],
)

cc_library(
    name = "keyboard_timeline_proto_conversion",
    hdrs = ["keyboard_timeline_proto.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":keyboard_timeline",
        ":keyboard_timeline_cc_proto",
    ],
)

py_proto_library(
    name = "keyboard_timeline_py_proto",
    visibility = ["//visibility:public"],
    deps = [":keyboard_timeline_proto"],
)

py_binary(
    name = "print_kb",
    srcs = ["print_kb.py"],
    main = "print_kb.py",
    deps = [
        ":keyboard_timeline_py_proto",
        "//finger_keyboard_mapping/keyboard:keyboard_shape_py_proto",
        "//finger_landmarks:finger_landmarks_py_proto",
        requirement("opencv-python"),
        requirement("numpy"),
    ],
)
