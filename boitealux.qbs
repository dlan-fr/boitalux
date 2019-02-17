import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "/home/msipc/Projects/common/of_v0.10.1_linux64gcc5_release/libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '/home/msipc/Projects/common/of_v0.10.1_linux64gcc5_release'

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            "src/enum.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
        ]

        of.addons: [
            'ofx3DModelLoader','ofxAssimpModelLoader'
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // create a console window when the application start
        consoleApplication: true

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }

        Depends {
                    name: "shmdata"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
    // will compile OF only once for all your projects
    // otherwise compiled per project with qbs


    property bool precompileOfMain: false  // precompile ofMain.h
    // faster to recompile when including ofMain.h
    // but might use a lot of space per project


    DynamicLibrary {
            name: "shmdata"
            Depends { name: "cpp" }

            Export {
                   Depends { name: "cpp" }
                   cpp.includePaths: ["/usr/local/include"]
                    cpp.dynamicLibraries: ["/usr/local/lib/libshmdata-1.3.so"]
            }
        }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
