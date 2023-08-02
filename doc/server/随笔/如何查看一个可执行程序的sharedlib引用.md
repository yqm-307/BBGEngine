再linux下，可以使用readelf工具查看elf文件头。文件头中直接会包含到动态库，如下：

```

yqm@ubuntuyqm:~/Git/owner/my_mmo/project/server/build/bin$ readelf -d gameserver 

Dynamic section at offset 0x87b28 contains 33 entries:
  Tag        Type                         Name/Value
 0x0000000000000001 (NEEDED)             Shared library: [libybbt.so]
 0x0000000000000001 (NEEDED)             Shared library: [libinih.so.0]
 0x0000000000000001 (NEEDED)             Shared library: [libstdc++.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libm.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libgcc_s.so.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
 0x000000000000001d (RUNPATH)            Library runpath: [/home/yqm/Git/owner/my_mmo/project/server/thirdparty/libs]
 0x000000000000000c (INIT)               0x4d000
 0x000000000000000d (FINI)               0x6f5f4
 0x0000000000000019 (INIT_ARRAY)         0x887d8
 0x000000000000001b (INIT_ARRAYSZ)       96 (bytes)
 0x000000000000001a (FINI_ARRAY)         0x88838
 0x000000000000001c (FINI_ARRAYSZ)       8 (bytes)
 0x000000006ffffef5 (GNU_HASH)           0x3b0
 0x0000000000000005 (STRTAB)             0xfd90
 0x0000000000000006 (SYMTAB)             0x3b08
 0x000000000000000a (STRSZ)              238894 (bytes)
 0x000000000000000b (SYMENT)             24 (bytes)
 0x0000000000000015 (DEBUG)              0x0
 0x0000000000000003 (PLTGOT)             0x88d78
 0x0000000000000002 (PLTRELSZ)           1680 (bytes)
 0x0000000000000014 (PLTREL)             RELA
 0x0000000000000017 (JMPREL)             0x4bca8
 0x0000000000000007 (RELA)               0x4b438
 0x0000000000000008 (RELASZ)             2160 (bytes)
 0x0000000000000009 (RELAENT)            24 (bytes)
 0x000000000000001e (FLAGS)              BIND_NOW
 0x000000006ffffffb (FLAGS_1)            Flags: NOW PIE
 0x000000006ffffffe (VERNEED)            0x4b2f8
 0x000000006fffffff (VERNEEDNUM)         4
 0x000000006ffffff0 (VERSYM)             0x4a2be
 0x000000006ffffff9 (RELACOUNT)          63
 0x0000000000000000 (NULL)               0x0

```