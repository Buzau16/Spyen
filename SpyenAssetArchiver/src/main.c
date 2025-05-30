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

#define ENTRY_NO_PTR_SIZE 12

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

    binary_file.current_data_offset = 0;
    binary_file.header.entry_count = 0;

    return EXIT_SUCCESS;
}
bool packer_shutdown() {
    if (!output_file) {
        printf("Can not close an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }

    // write the header entry count
    _write_u16_le(binary_file.header.entry_count);

    binary_file.current_data_offset = sizeof(Header) + (ENTRY_NO_PTR_SIZE * binary_file.header.entry_count);

    // write the entries, NO DATA!
    for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
        _write_u32_le(binary_file.entries[i].hash);
        _write_u32_le(binary_file.current_data_offset);
        binary_file.current_data_offset += binary_file.entries[i].size;
        _write_u32_le(binary_file.entries[i].size);
    }

    // Write the data
    for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
        fwrite(binary_file.entries[i].data, binary_file.entries[i].size, 1, output_file);
        free(binary_file.entries[i].data);
    }

    free(binary_file.entries);
    fclose(output_file);

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

#define INDEX binary_file.header.entry_count

    binary_file.entries = realloc(binary_file.entries, sizeof(Entry) * (binary_file.header.entry_count + 1));

    binary_file.entries[INDEX].hash = _fnv1a_hash(name);
    binary_file.entries[INDEX].size = size;
    binary_file.entries[INDEX].data = malloc(size);
    memcpy(binary_file.entries[INDEX].data, data, size);

    binary_file.header.entry_count++;
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
    const uint32_t FNV_PRIME = 0x1000193;
    uint32_t result = 0x811c9dc5;
    for (; *str; str++) {
        result ^= (uint8_t)(*str);
        result *= FNV_PRIME;
    }
    return result;
}

int main(int argv, char *argc[]) {
    packer_init("spak");
    typedef struct Position {
        float x;
        float y;
    } Position;
    Position pos = {2.4f, 1.2f};
    packer_write_header("SPAW", 0, 1);
    float scale = 1.0f;
    float rotation = 45.0f;
    packer_add_entry("position", &pos, sizeof(pos));
    packer_add_entry("scale", &scale, sizeof(scale));
    packer_add_entry("rotation", &rotation, sizeof(rotation));
    packer_shutdown();

    return 0;
}
