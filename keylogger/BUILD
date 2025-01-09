load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "keylogger",
    srcs = ["keylogger_main.cpp"],
    linkopts = ["-DEFAULTLIB:user32.lib "],
    deps = [":keylog_cc_proto"],
)

proto_library(
    name = "keylog_proto",
    srcs = ["keylog.proto"],
)

cc_proto_library(
    name = "keylog_cc_proto",
    deps = [":keylog_proto"],
)
