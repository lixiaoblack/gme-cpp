{
  "targets": [{
    "target_name": "gme_native",
    "sources": [ "src/native/gme.cpp" ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "<(module_root_dir)/GME_Win_sdk_3.1.0.367/include"
    ],
    "dependencies": [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    "defines": [ 
      "NAPI_DISABLE_CPP_EXCEPTIONS",
      "BUILDING_NODE_EXTENSION"
    ],
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "xcode_settings": {
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      "CLANG_CXX_LIBRARY": "libc++",
      "MACOSX_DEPLOYMENT_TARGET": "10.15"
    },
    "msvs_settings": {
      "VCCLCompilerTool": {
        "ExceptionHandling": 1
      }
    },
    "conditions": [
      ["OS==\"win\"", {
        "libraries": [
          "<(module_root_dir)/GME_Win_sdk_3.1.0.367/lib/x64/GMESDK.lib"
        ],
        "copies": [{
          "destination": "<(PRODUCT_DIR)",
          "files": [
            "<(module_root_dir)/GME_Win_sdk_3.1.0.367/lib/x64/GMESDK.dll",
            "<(module_root_dir)/GME_Win_sdk_3.1.0.367/lib/x64/txffmpeg.dll",
            "<(module_root_dir)/GME_Win_sdk_3.1.0.367/lib/x64/txsoundtouch.dll"
          ]
        }]
      }],
      ["OS==\"mac\"", {
        "defines": [
          "GME_MOCK_MODE"
        ]
      }]
    ]
  }]
} 