# slang_shader_compiler

custom compiler using slang reflection API

- compile slang shader
- output these information to c header file:
  - spv binary (hex dump)
  - resource binding layout
- compile 1 endpoint per file

usage:
```
PROGRAM <code output file> <layout output file> <project root dir> [.slang files...]
```