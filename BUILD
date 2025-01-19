load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_python//python:pip.bzl", "compile_pip_requirements")

compile_pip_requirements(
    name = "requirements",
    src = "requirements.in",
    requirements_txt = "requirements_lock.txt",
)

cc_library(
    name = "catch_string_helper",
    hdrs = ["catch_string_helper.hpp"],
    visibility = ["//visibility:public"],
    deps = [
        "@catch2",
        "@fmt",
    ],
)

package_group(
    name = "finger_keyboard_mapping",
    packages = ["//finger_keyboard_mapping/..."],
)

package_group(
    name = "finger_landmarks",
    packages = ["//finger_landmarks/..."],
)

package_group(
    name = "keylogger",
    packages = ["//keylogger/..."],
)

package_group(
    name = "statistics",
    packages = ["//statistics/..."],
)

package_group(
    name = "layout_generator",
    packages = ["//layout_generator/..."],
)
