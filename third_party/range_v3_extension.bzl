load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def declare_range_v3(ctx):
    RANGEV3_VERSION = "7e6f34b1e820fb8321346888ef0558a0ec842b8e"
    RANGEV3_INTEGRITY = "sha256-g5VRjhceWHPlHbnvFToXtb1G5pqZtk/WxEtFtT+Y188="

    http_archive(
        name = "range-v3",
        integrity = RANGEV3_INTEGRITY,
        strip_prefix = "range-v3-{}".format(RANGEV3_VERSION),
        url = "https://github.com/ericniebler/range-v3/archive/{}.tar.gz".format(RANGEV3_VERSION),
    )

range_v3 = module_extension(
    implementation = declare_range_v3,
)
