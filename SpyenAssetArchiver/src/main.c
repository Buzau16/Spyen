#include <Archiver.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Header {
    uint32_t signature;
    uint8_t version_major;
    uint8_t version_minor;
    uint16_t entry_count;
};

struct Entry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
    void* data;
};

struct File {
    Header header;
    Entry* entries;
    uint32_t current_data_offset;
};

FILE* output_file;
File binary_file;

bool packer_init(const char *filename) {
    if (output_file) {
        printf("File already opened!\n");
        return EXIT_FAILURE;
    }

    output_file = fopen(filename, "wb");

    if (!output_file) {
        printf("Failed to open file: %s\n", filename);
        return EXIT_FAILURE;
    }

    binary_file.current_data_offset = sizeof(Header);
    binary_file.header.entry_count = 0;

    return EXIT_SUCCESS;
}
bool packer_shutdown() {
    if (!output_file) {
        printf("Can not close an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
bool packer_write_header(const char *signature, uint8_t version_major, uint8_t version_minor) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }

    _write_u32_le(_char_to_u32_le(signature));
    _write_u8_le(version_major);
    _write_u8_le(version_minor);

    return EXIT_SUCCESS;
}
bool packer_add_entry(const char *name, void *data, size_t size) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void _write_u32_le(const uint32_t val) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return;
    }

    const uint8_t bytes[4] = {
         val        & 0xff,
        (val >>  8) & 0xff,
        (val >> 16) & 0xff,
        (val >> 24) & 0xff
    };
    fwrite(&bytes, 1, 4, output_file);
}

void _write_u16_le(const uint16_t val) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return;
    }

    const uint8_t bytes[2] = {
             val       & 0xff,
            (val >> 8) & 0xff
    };
    fwrite(&bytes, 1, 2, output_file);
}

void _write_u8_le(const uint8_t val) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return;
    }

    fwrite(&val, 1, 1, output_file);
}

uint32_t _char_to_u32_le(const char *str) {
    if (strlen(str) > 4) {
        printf("Please use 4 characters or it will cut off!\n");
    }

    uint32_t result = 0;
    for (uint32_t i = 0; i < 4; i++) {
        result |= (uint32_t)(unsigned char)str[i] << 8 * i;
    }
    return result;
}

uint32_t _fnv1a_hash(const char *str) {
    const uint32_t FNV_PRIME = 0x811c9dc5;
    uint32_t result = 0x811c9dc5;
    for (uint32_t i = 0; i < strlen(str); str++, i++) {
        result *= FNV_PRIME;
        result ^= (*str);
    }
    return result;
}

int main(int argv, char *argc[]) {
    packer_init("spak");
    packer_write_header("SPAW", 0, 1);
    float scale = 1.0f;
    packer_add_entry("scale", &scale, sizeof(scale));
    packer_shutdown();

    return 0;
}
