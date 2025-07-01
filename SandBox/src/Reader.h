#ifndef READER_H
#define READER_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileContext FileContext;
// Hashmap is a little too much for this implementation
typedef struct ABHashmap ABHashmap;
typedef struct ABHashEntry ABHashEntry;

FileContext* reader_init(const char* filepath);
void reader_shutdown(FileContext * ctx);
void* reader_get_entry(const FileContext * ctx, const char* name);
ABHashmap* create_hashmap(uint8_t entry_count);
ABHashEntry get_entry(FileContext *ctx, const char *name);
void delete_hashmap(ABHashmap* vector);


// From https://www.programmingalgorithms.com/algorithm/fnv-hash/c/
uint32_t _fnv1a_hash(const char* str);

#ifdef __cplusplus
}
#endif

#ifdef ARCBYTE_IMPLEMENTATION
#include "Reader.h"

struct FileContext {
    FILE* input_file;
    ABHashmap* hashmap;
    uint8_t data_type;
};

struct ABHashEntry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
};

struct ABHashmap {
    uint8_t entry_count;
    // Type for later
    ABHashEntry* entries;
};

inline FileContext *reader_init(const char *filepath) {
    FileContext *ctx = (FileContext*)malloc(sizeof(FileContext));

    printf("Initializing Reader\n");
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
    if (!ctx->hashmap) {
        printf("PANIC PANIC!\n");
    }
    for (uint32_t i = 0; i < ctx->hashmap->entry_count; i++) {
        fread(&ctx->hashmap->entries[i].hash, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].offset, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].size, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].width, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].height, sizeof(uint32_t), 1, ctx->input_file);
        fread(&ctx->hashmap->entries[i].channels, sizeof(uint32_t), 1, ctx->input_file);
    }

    return ctx;
}
inline void reader_shutdown(FileContext *ctx) {
    fclose(ctx->input_file);
    delete_hashmap(ctx->hashmap);
    free(ctx);
}
inline void* reader_get_entry(const FileContext *ctx, const char *name) {
    // image entry is 4 * 6 bytes!
    printf("Getting entry: %s", name);
    switch (ctx->data_type) {
        case 1: /*Images*/ {
            const uint32_t hash = _fnv1a_hash(name);
            for (uint32_t i = 0; i < ctx->hashmap->entry_count; i++) {
                const ABHashEntry entry = ctx->hashmap->entries[i];
                if (hash == entry.hash) {
                    // Read the entry from the disk
                    void* image_data = malloc(entry.size);

                    fseek(ctx->input_file, entry.offset, SEEK_SET);
                    fread(image_data, entry.size, 1, ctx->input_file);

                    return image_data;
                }else {
                    perror("Failed to find entry!\n");
                }
            }
        }

        default:
            break;
    }
    return NULL;
}
inline ABHashmap *create_hashmap(const uint8_t entry_count) {
    ABHashmap *vect = (ABHashmap*)malloc(sizeof(ABHashmap));
    if (!vect) {
        printf("Failed to allocate memory\n");
    }
    vect->entries = (ABHashEntry*)malloc(sizeof(ABHashEntry) * entry_count);
    if (!vect->entries) {
        printf("Failed to allocate memory\n");
    }
    vect->entry_count = entry_count;
    memset(vect->entries, 0, sizeof(ABHashEntry) * entry_count);
    return vect;
}
inline ABHashEntry get_entry(FileContext *ctx, const char *name) {
    const uint32_t hash = _fnv1a_hash(name);
    for (uint32_t i = 0; i < ctx->hashmap->entry_count; i++) {
        const ABHashEntry entry = ctx->hashmap->entries[i];
        if (hash == entry.hash) {
            return entry;
        }
    }
    perror("Failed to find entry!\n");
}
inline void delete_hashmap(ABHashmap *vector) {
    free(vector->entries);
    free(vector);
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
