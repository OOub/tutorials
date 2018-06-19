solution 'dvs_event_stream_counter'
    configurations {'release', 'debug'}
    location 'build'
    project 'dvs_event_stream_counter'
        kind 'ConsoleApp'
        language 'C++'
        location 'build'
        files {'source/*.cpp'}
        buildoptions {'-std=c++11'}
        linkoptions {'-std=c++11'}
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
