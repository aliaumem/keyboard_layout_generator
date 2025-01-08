load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@protobuf//bazel:py_proto_library.bzl", "py_proto_library")
load("@pypi//:requirements.bzl", "requirement")
load("@rules_python//python:py_binary.bzl", "py_binary")
load("@rules_python//python:py_library.bzl", "py_library")

proto_library(
    name = "finger_landmarks",
    srcs = ["finger_landmarks.proto"],
)

cc_proto_library(
    name = "finger_landmarks_cc_proto",
    visibility = ["//visibility:public"],
    deps = [":finger_landmarks"],
)

py_proto_library(
    name = "finger_landmarks_py_proto",
    deps = [":finger_landmarks"],
)

py_library(
    name = "keyboard_shape_recorder",
    srcs = ["keyboard_shape_recorder.py"],
    deps = [
        requirement("numpy"),
        requirement("opencv-python"),
    ],
)

py_library(
    name = "finger_tracking_results",
    srcs = ["finger_tracking_results.py"],
)

py_binary(
    name = "finger_landmarks_tracker",
    srcs = [
        "draw_landmarks_annotations.py",
        "landmarks_runner.py",
        "rotation_enum.py",
        "video_tracking.py",
    ],
    data = ["hand_landmarker.task"],
    main = "video_tracking.py",
    deps = [
        ":finger_tracking_results",
        ":keyboard_shape_recorder",
        requirement("mediapipe"),
        requirement("numpy"),
        requirement("opencv-python"),
    ],
)

py_binary(
    name = "results_to_proto",
    srcs = ["results_to_proto.py"],
    main = "results_to_proto.py",
    deps = [
        ":finger_landmarks_py_proto",
        ":finger_tracking_results",
    ],
)

py_binary(
    name = "print_kb",
    srcs = ["print_kb.py"],
    main = "print_kb.py",
    deps = [
        "//finger_keyboard_mapping:keyboard_shape_py_proto",
        requirement("opencv-python"),
        requirement("numpy"),
    ],
)
