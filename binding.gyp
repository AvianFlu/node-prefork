{
  "targets": [
    {
      "target_name": "prefork",
      "sources": [ "src/prefork.cc" ],
      "dependencies": [
        "aeternum"
      ]
    },
    {
      "target_name": "aeternum",
      "type": "static_library",
      "include_dirs": [
        "deps/aeternum"
      ],
      "sources": [
        "deps/aeternum/aeternum.c"
      ]
    }
  ]
}
