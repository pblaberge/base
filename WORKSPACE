workspace(name = "com_pblaberge_base")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "rules_cc",
    commit = "699ec5298546a0285a57f77dc47d72363c4d2517",
    remote = "git@github.com:bazelbuild/rules_cc.git",
    shallow_since = "1604088357 -0700",
)

http_archive(
    name = "com_google_googletest",
    sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
    strip_prefix = "googletest-release-1.10.0",
    urls = [
        "https://github.com/google/googletest/archive/release-1.10.0.tar.gz",
    ],
)

http_archive(
    name = "com_github_google_benchmark",
    sha256 = "dccbdab796baa1043f04982147e67bb6e118fe610da2c65f88912d73987e700c",
    strip_prefix = "benchmark-1.5.2",
    urls = [
        "https://github.com/google/benchmark/archive/v1.5.2.tar.gz",
    ],
)

http_archive(
    name = "com_google_absl",
    sha256 = "bf3f13b13a0095d926b25640e060f7e13881bd8a792705dd9e161f3c2b9aa976",
    strip_prefix = "abseil-cpp-20200923.2",
    urls = [
        "https://github.com/abseil/abseil-cpp/archive/20200923.2.tar.gz",
    ],
)
