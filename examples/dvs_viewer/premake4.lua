local qt = require 'third_party/chameleon/qt'

solution 'dvs_viewer'
    configurations {'release', 'debug'}
    location 'build'
    project 'dvs_viewer'
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/*.qml', 'source/*.cpp'}
        links 'usb-1.0'
        files(qt.moc({
            'third_party/chameleon/source/background_cleaner.hpp',
            'third_party/chameleon/source/dvs_display.hpp'},
            'build/moc'))
        includedirs(qt.includedirs())
        libdirs(qt.libdirs())
        links(qt.links())
        buildoptions(qt.buildoptions())
        linkoptions(qt.linkoptions())
        defines {'SEPIA_COMPILER_WORKING_DIRECTORY="' .. project().location .. '"'}
        configuration 'release'
            targetdir 'build/release'
            defines {'NDEBUG'}
            flags {'OptimizeSpeed'}
        configuration 'debug'
            targetdir 'build/debug'
            defines {'DEBUG'}
            flags {'Symbols'}
        configuration 'linux'
            links {'pthread'}
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
        configuration 'macosx'
            includedirs '/usr/local/include'
            libdirs '/usr/local/lib'
            buildoptions {'-std=c++11'}
            linkoptions {'-std=c++11'}
        configuration 'windows'
            files {'.clang-format'}
