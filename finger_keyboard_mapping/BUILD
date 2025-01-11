load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:py_proto_library.bzl", "py_proto_library")
load("@pypi//:requirements.bzl", "requirement")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@rules_python//python:py_binary.bzl", "py_binary")

package(default_visibility = ["__subpackages__"])

cc_binary(
    name = "finger_keyboard_mapping",
    srcs = ["main.cpp"],
    deps = [
        ":finger_to_key_mapping",
        ":keyboard_shape_cc_proto",
        ":proto_conversion",
        "//keylogger:keylog_cc_proto",
        "@protobuf//:protobuf_headers",
    ],
)

cc_binary(
    name = "dump_keyboard_shape",
    srcs = ["dump_keyboard_shape.cpp"],
    deps = [
        ":keyboard_shape_cc_proto",
        ":keyboard_shape_to_proto",
    ],
)

cc_library(
    name = "data_types",
    srcs = ["scancode_key_map.cpp"],
    hdrs = [
        "frame.hpp",
        "key.hpp",
        "mapping_geometry.hpp",
        "scancode_key_map.hpp",
        "static_vector.hpp",
    ],
)

cc_library(
    name = "proto_conversion",
    hdrs = ["from_proto.hpp"],
    deps = [
        ":data_types",
        "//finger_landmarks:finger_landmarks_cc_proto",
    ],
)

cc_library(
    name = "finger_to_key_mapping",
    srcs = [
        "finger_to_key_mapping.cpp",
        "keyboard_default_shape.cpp",
    ],
    hdrs = ["finger_to_key_mapping.hpp"],
    deps = [":data_types"],
)

cc_library(
    name = "keyboard_shape_to_proto",
    hdrs = ["keyboard_shape_to_proto.hpp"],
    deps = [
        ":finger_to_key_mapping",
        ":keyboard_shape_cc_proto",
    ],
)

proto_library(
    name = "keyboard_shape_proto",
    srcs = ["keyboard_shape.proto"],
)

cc_proto_library(
    name = "keyboard_shape_cc_proto",
    deps = [":keyboard_shape_proto"],
)

py_proto_library(
    name = "keyboard_shape_py_proto",
    visibility = ["//visibility:public"],
    deps = [":keyboard_shape_proto"],
)

py_binary(
    name = "print_kb",
    srcs = ["print_kb.py"],
    main = "print_kb.py",
    deps = [
        ":keyboard_shape_py_proto",
        "//finger_landmarks:finger_landmarks_py_proto",
        requirement("opencv-python"),
        requirement("numpy"),
    ],
)
