load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:py_proto_library.bzl", "py_proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["__subpackages__"])

cc_binary(
    name = "finger_keyboard_mapping",
    srcs = ["main.cpp"],
    deps = [
        ":finger_to_key_mapping",
        ":keyboard_shape_cc_proto",
        ":proto_conversion",
        "@protobuf//:protobuf_headers",
    ],
)

cc_library(
    name = "data_types",
    hdrs = [
        "frame.hpp",
        "mapping_geometry.hpp",
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
