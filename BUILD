load("@rules_python//python:pip.bzl", "compile_pip_requirements")

compile_pip_requirements(
    name = "requirements",
    src = "requirements.in",
    requirements_txt = "requirements_lock.txt",
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
