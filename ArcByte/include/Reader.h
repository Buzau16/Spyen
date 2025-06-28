#ifndef READER_H
#define READER_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"

typedef struct FileContext FileContext;
FileContext* reader_init(const char* filepath);
unsigned char* reader_get_entry(FileContext* ctx, const char* name);

// From https://www.programmingalgorithms.com/algorithm/fnv-hash/c/
uint32_t _fnv1a_hash(const char* str);

#define ARCBYTE_READER_IMPLEMENTATION
#ifdef ARCBYTE_READER_IMPLEMENTATION

struct FileContext {
    FILE* input_file;
    uint8_t entry_count;
    uint8_t data_type;
};

inline FileContext *reader_init(const char *filepath) {
    FileContext *ctx = malloc(sizeof(FileContext));

    ctx->input_file = fopen(filepath, "rb");
    if (!ctx->input_file) {
        perror("Can not open file: ");
        printf("%s!\n", filepath);
        return NULL;
    }

    fseek(ctx->input_file, 6, SEEK_SET);
    fread(&ctx->entry_count, sizeof(uint8_t), 1, ctx->input_file);
    fread(&ctx->data_type, sizeof(uint8_t), 1, ctx->input_file);

    return ctx;
}
inline unsigned char *reader_get_entry(FileContext *ctx, const char *name) {
    // image entry is 4 * 6 bytes!

    fseek(ctx->input_file, 8, SEEK_SET);

    switch (ctx->data_type) {
        case 1: /*Images*/ {
            const uint32_t hash = _fnv1a_hash(name);
            uint32_t read_hash = 0;
            for (uint32_t i = ftell(ctx->input_file); i < 8 + 24 * ctx->entry_count; i+=24) {
                fread(&read_hash, sizeof(uint32_t), 1, ctx->input_file);
                if (read_hash == hash) {

                    // read the offset and the size
                    uint32_t offset = 0;
                    uint32_t size = 0;
                    fread(&offset, sizeof(uint32_t), 1, ctx->input_file);
                    fread(&size, sizeof(uint32_t), 1, ctx->input_file);

                    unsigned char *image_data = malloc(size);

                    fseek(ctx->input_file, offset, SEEK_SET);
                    fread(image_data, size, 1, ctx->input_file);


                    return image_data;
                }
            }
        }

        default:
            break;
    }
}

inline uint32_t _fnv1a_hash(const char *str) {
    const uint32_t FNV_PRIME = 0x1000193;
    uint32_t result = 0x811c9dc5;
    for (; *str; str++) {
        result ^= (uint8_t) (*str);
        result *= FNV_PRIME;
    }
    return result;
}

#endif

#endif //READER_H
