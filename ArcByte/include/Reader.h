#ifndef READER_H
#define READER_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_image.h"

typedef struct FileContext FileContext;
// Hashmap is a little too much for this implementation
typedef struct ABHashmap ABHashmap;
typedef struct ABHashEntry ABHashEntry;

FileContext* reader_init(const char* filepath);
void reader_shutdown(const FileContext * ctx);
unsigned char* reader_get_entry(const FileContext * ctx, const char* name);
ABHashmap* create_hashmap(uint8_t entry_count);
ABHashEntry get_entry(uint32_t hash);
void delete_hashmap(ABHashmap* vector);



// From https://www.programmingalgorithms.com/algorithm/fnv-hash/c/
uint32_t _fnv1a_hash(const char* str);

void* _arcbyte_malloc(size_t size);
void _arcbyte_free(void* ptr);

#define ARCBYTE_READER_IMPLEMENTATION
#ifdef ARCBYTE_READER_IMPLEMENTATION

struct FileContext {
    FILE* input_file;
    ABHashmap* hashmap;
    uint8_t data_type;
};

struct ABHashEntry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
};

struct ABHashmap {
    uint8_t entry_count;
    // Type for later
    ABHashEntry* entries;
};

inline FileContext *reader_init(const char *filepath) {
    FileContext *ctx = _arcbyte_malloc(sizeof(FileContext));

    ctx->input_file = fopen(filepath, "rb");
    if (!ctx->input_file) {
        perror("Can not open file: ");
        printf("%s!\n", filepath);
        return NULL;
    }

    fseek(ctx->input_file, 6, SEEK_SET);
    uint8_t count = 0;
    fread(&count, sizeof(uint8_t), 1, ctx->input_file);
    fread(&ctx->data_type, sizeof(uint8_t), 1, ctx->input_file);

    // Hashmap
    ctx->hashmap = create_hashmap(count);
    for (uint32_t i = 0; i < ctx->hashmap->entry_count; i++) {
        fread(&ctx->hashmap->entries[i].hash, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].offset, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].size, sizeof(uint32_t), 1, ctx->input_file);
        fseek(ctx->input_file, 12, SEEK_CUR);
    }

    return ctx;
}
inline void reader_shutdown(FileContext *ctx) {
    fclose(ctx->input_file);
    delete_hashmap(ctx->hashmap);
    _arcbyte_free(ctx);
}
inline unsigned char *reader_get_entry(const FileContext *ctx, const char *name) {
    // image entry is 4 * 6 bytes!
    switch (ctx->data_type) {
        case 1: /*Images*/ {
            const uint32_t hash = _fnv1a_hash(name);
            for (uint32_t i = 0; i < ctx->hashmap->entry_count; i++) {
                const ABHashEntry *entry = &ctx->hashmap->entries[i];
                if (hash == entry->hash) {
                    // Read the entry from the disk
                    unsigned char *image_data = malloc(entry->size);

                    fseek(ctx->input_file, entry->offset, SEEK_SET);
                    fread(image_data, entry->size, 1, ctx->input_file);

                    return image_data;
                }
            }
        }

        default:
            break;
    }
    return NULL;
}
inline ABHashmap *create_hashmap(const uint8_t entry_count) {
    ABHashmap *vect = _arcbyte_malloc(sizeof(ABHashmap));
    vect->entries = _arcbyte_malloc(sizeof(ABHashEntry) * entry_count);
    vect->entry_count = entry_count;
    memset(vect->entries, 0, sizeof(ABHashEntry) * entry_count);
    return vect;
}
inline void delete_hashmap(ABHashmap *vector) {
    _arcbyte_free(vector->entries);
    _arcbyte_free(vector);
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

inline void* _arcbyte_malloc(const size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        perror("Failed to allocate memory!!!\n");
    }
    return ptr;
}

inline void _arcbyte_free(void* ptr) {
    free(ptr);
}

#endif

#endif //READER_H
