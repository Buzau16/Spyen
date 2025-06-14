#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/*
MIT License

Copyright 2025 Neagu Cristian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

typedef struct Header Header;
typedef struct Entry Entry;
typedef struct ImageEntry ImageEntry;
typedef struct SoundEntry SoundEntry; //TODO: to be implemented!
typedef struct File File;
typedef struct FileData FileData;

bool packer_init(const char* filename);
bool packer_shutdown();
void packer_set_mode(uint8_t mode);
bool packer_write_header(const char* signature, uint8_t version_major, uint8_t version_minor);
// filepath is used only for texture/sound datatypes!
bool packer_add_entry(const char* name, const char* filepath, const void * data, size_t size);

bool _resize_entry_buffer(size_t size);
void _write_u32_le(uint32_t val);
void _write_u16_le(uint16_t val);
void _write_u8_le(uint8_t val);

// Use only for 4 characters
uint32_t _char_to_u32_le(const char* str);
uint8_t _char_to_u8_le(char str);

// From https://www.programmingalgorithms.com/algorithm/fnv-hash/c/
uint32_t _fnv1a_hash(const char* str);

char* _truncate_filepath(const char* filepath);

#endif
