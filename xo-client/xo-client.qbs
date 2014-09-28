import qbs
import "../helpers.js" as Helpers

QtGuiApplication {
    Depends { name: "Qt"; submodules: ["network", "widgets", "qml", "quick", "svg"] }
    Depends { name: "cpp" }

    cpp.cxxFlags: Helpers.commonCxxFlags(qbs)
    cpp.linkerFlags: Helpers.commonLinkerFlags(qbs)
    Properties {
        condition: qbs.targetOS === "osx"
        cpp.minimumOsxVersion: "10.7"
    }

    Group {
        name: "Native code"
        files: ["*.h", "*.cpp", "../xo-server/commands.h"]
    }

    Group {
        name: "User Interface"
        files: ["*.qml", "*.js"]
    }

    Group {
        name: "Resources"
        files: "*.qrc"
    }

    Group {
        name: "Deployment"
        qbs.install: true
        fileTagsFilter: product.type
    }
}
