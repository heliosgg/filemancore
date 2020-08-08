{
  "targets": [
    {
      "target_name": "filemancore",
      "sources": [
        "./src/cpp/index.cpp",
        "./src/cpp/Wrapper.cpp",
        "./src/cpp/classes/FileInfo.cpp",
        "./src/cpp/utils/Utils.cpp",
        "./src/cpp/utils/win/FileSystem.cpp",
        "./src/cpp/utils/win/OS.cpp",
        "./src/cpp/utils/win/Path.cpp",
        "./src/cpp/utils/win/Time.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}