//
// Copyright (c) 2014-present, Facebook, Inc.
// All rights reserved.
//
// This source code is licensed under the University of Illinois/NCSA Open
// Source License found in the LICENSE file in the root directory of this
// source tree. An additional grant of patent rights can be found in the
// PATENTS file in the same directory.
//

#pragma once

#include <sys/mman.h>

namespace ds2 {
namespace Host {
namespace Linux {
namespace X86_64 {
namespace Syscalls {

namespace {
static uint8_t const gMmapCode[] = {
    0x48, 0xc7, 0xc0, 0x00, 0x00, 0x00, 0x00, // 00: movq $sysno, %rax
    0x48, 0x31, 0xff,                         // 07: xorq %rdi, %rdi
    0x48, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x00, // 0a: movq $XXXXXXXX, %rsi
    0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00, // 11: movq $XXXXXXXX, %rdx
    0x49, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00, // 18: movq $XXXXXXXX, %r10
    0x49, 0xc7, 0xc0, 0xff, 0xff, 0xff, 0xff, // 1f: movq $-1, %r8
    0x4d, 0x31, 0xc9,                         // 26: xorq %r9, %r9
    0x0f, 0x05,                               // 29: syscall
    0xcc                                      // 2b: int3
};

static uint8_t const gMunmapCode[] = {
    0x48, 0xc7, 0xc0, 0x00, 0x00, 0x00, 0x00, // 00: movq $sysno, %rax
    0x48, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, // 07: movq $XXXXXXXXXXXXXXXX, %rdi
    0x48, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x00, // 11: movq $XXXXXXXX, %rsi
    0x0f, 0x05,                               // 18: syscall
    0xcc                                      // 1a: int3
};
}

static inline void PrepareMmapCode(size_t size, uint32_t protection,
                                   ByteVector &codestr) {
  codestr.assign(&gMmapCode[0], &gMmapCode[sizeof(gMmapCode)]);

  uint8_t *code = &codestr[0];
  *reinterpret_cast<uint32_t *>(code + 0x03) = 9; // __NR_mmap
  *reinterpret_cast<uint32_t *>(code + 0x0d) = size;
  *reinterpret_cast<uint32_t *>(code + 0x14) = protection;
  *reinterpret_cast<uint32_t *>(code + 0x1b) = MAP_ANON | MAP_PRIVATE;
}

static inline void PrepareMunmapCode(uint64_t address, size_t size,
                                     ByteVector &codestr) {
  codestr.assign(&gMunmapCode[0], &gMunmapCode[sizeof(gMunmapCode)]);

  uint8_t *code = &codestr[0];
  *reinterpret_cast<uint32_t *>(code + 0x03) = 11; // __NR_munmap
  *reinterpret_cast<uint64_t *>(code + 0x09) = address;
  *reinterpret_cast<uint32_t *>(code + 0x14) = size;
}
}
}
}
}
}
