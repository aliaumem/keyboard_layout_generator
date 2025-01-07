load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "finger_keyboard_mapping",
    srcs = ["main.cpp"],
    deps = [
        ":finger_to_key_mapping",
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
    srcs = ["finger_to_key_mapping.cpp"],
    hdrs = ["finger_to_key_mapping.hpp"],
    deps = [":data_types"],
)
