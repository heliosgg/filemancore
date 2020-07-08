{
  "targets": [
    {
      "target_name": "filemancore",
      "sources": [
        "./src/cpp/index.cpp",
        "./src/cpp/Utils.cpp",
        "./src/cpp/Wrapper.cpp",
        "./src/cpp/interfaces/FileInfo.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}