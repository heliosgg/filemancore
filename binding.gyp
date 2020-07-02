{
  "targets": [
    {
      "target_name": "filemancore",
      "sources": [
        "./src/index.cpp",
        "./src/Utils.cpp",
        "./src/Wrapper.cpp",
        "./src/interfaces/FileInfo.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}