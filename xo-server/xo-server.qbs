import qbs
import "../helpers.js" as Helpers

QtGuiApplication {
    Depends { name: "Qt"; submodules: ["widgets", "network"] }
    Depends { name: "cpp" }

    cpp.cxxFlags: Helpers.commonCxxFlags(qbs)
    cpp.linkerFlags: Helpers.commonLinkerFlags(qbs)
    Properties {
        condition: qbs.targetOS === "osx"
        cpp.minimumOsxVersion: "10.7"
    }

    files: [
        "*.h",
        "*.cpp",
        "*.ui"
    ]

    Group {
        name: "Deployment"
        qbs.install: true
        fileTagsFilter: product.type
    }
}
