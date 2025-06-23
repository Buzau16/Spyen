#include <Archiver.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef enum DataTypes {
    NONE,
    TEXTURE,
    SOUND
} DataType;

struct BinaryContext {
    char* output_name;
    char sig[4];
    uint8_t major;
    uint8_t minor;
    uint16_t entry_count;
    DataType type;
    uint32_t data_offset;
};

struct Header {
    uint32_t signature;
    uint8_t version_major;
    uint8_t version_minor;
    uint16_t entry_count;
};

struct ImageEntry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    // 32_ts for the sake of readability!
};

FILE* output_file;

bool packer_init(BinaryContext** context) {
    if (output_file) {
        printf("File already opened!\n");
        return EXIT_FAILURE;
    }
    printf("Initializing arcbyte\n");

    *context = malloc(sizeof(BinaryContext));

    strncpy((*context)->sig, "ARBY", 4);
    (*context)->output_name = malloc(14);
    strncpy((*context)->output_name, "arcbyte_assets", 14);
    (*context)->major = 0;
    (*context)->minor = 0;
    (*context)->entry_count = 0;
    (*context)->type = NONE;
    (*context)->data_offset = sizeof(Header);

    return EXIT_SUCCESS;
}
bool packer_open_file(char *filename) {
    output_file = fopen(filename, "wb");

    if (!output_file) {
        printf("Failed to open file: %s\n", filename);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
bool packer_shutdown(BinaryContext** context) {
    if (!output_file) {
        printf("Can not close an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }

    printf("Shutting down\n");
    fclose(output_file);
    free((*context)->output_name);
    free(*context);

    printf("Shutdown complete! Enjoy your file!\n");
    return EXIT_SUCCESS;

}
void packer_set_mode(BinaryContext* context, const uint8_t mode) {
    context->type = mode;
}
bool packer_write_header(BinaryContext* context, const char *signature, uint8_t version_major, uint8_t version_minor, uint16_t entry_count) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }
    printf("Writing the header\n");

    _write_u32_le(_char_to_u32_le(signature));
    _write_u8_le(version_major);
    _write_u8_le(version_minor);
    _write_u16_le(entry_count);

    uint32_t tpof;
    if (context->type == TEXTURE) {
        tpof = entry_count * sizeof(ImageEntry);
    }else if (context->type == SOUND) {
        //todo: tpof = entry_count;
    }
    context->data_offset += tpof;

    return EXIT_SUCCESS;
}
//
bool packer_write_entry(BinaryContext* context, const char *name, const char *filepath) {
    printf("Writing entry: %s\n", name);

    switch (context->type) {
        case NONE: {
            printf("No type set! %s\n", __FUNCTION__);
            return EXIT_FAILURE;
        }
        case TEXTURE: {
            int x, y, c;
            unsigned char *image_data = stbi_load(filepath, &x, &y, &c, 0);
            if (!image_data) {
                printf("Failed to load image: %s\n", filepath);
                return EXIT_FAILURE;
            }

            _write_u32_le(_fnv1a_hash(name));
            _write_u32_le(context->data_offset);

            _write_u32_le(x*y*c);
            _write_u32_le(x);
            _write_u32_le(y);
            _write_u32_le(c);

            long location = ftell(output_file);
            // Write the data
            fseek(output_file, context->data_offset, SEEK_SET);
            fwrite(image_data, x * y * c, 1, output_file);
            context->data_offset += x * y * c;
            fseek(output_file, location, SEEK_SET);

            stbi_image_free(image_data);

            return EXIT_SUCCESS;
        }
        case SOUND: {
            break;
        }
    }
    return EXIT_FAILURE;
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
        result |= (uint32_t) (unsigned char) str[i] << 8 * i;
    }
    return result;
}
uint8_t _char_to_u8_le(const char str) {
    return (unsigned char) str;
}

uint32_t _fnv1a_hash(const char *str) {
    const uint32_t FNV_PRIME = 0x1000193;
    uint32_t result = 0x811c9dc5;
    for (; *str; str++) {
        result ^= (uint8_t) (*str);
        result *= FNV_PRIME;
    }
    return result;
}

char* _truncate_filepath(const char* filepath) {
    const char* last_slash = strrchr(filepath, '/');
    const char* last_backslash = strrchr(filepath, '\\');

    const char* filename_start = last_slash > last_backslash ? last_slash : last_backslash;
    if (filename_start)
        filename_start++;
    else
        filename_start = filepath;

    const char* dot = strrchr(filename_start, '.');

    size_t len = dot ? (size_t)(dot - filename_start) : strlen(filename_start);
    char* result = (char*)malloc(len + 1);
    if (!result) return NULL; // possible vulnerability

    strncpy(result, filename_start, len);
    result[len] = '\0';

    return result;
}

/*
 * Command example
 * arcbyte -f(1 file) -i(image) example1.png
 * arcbyte -fs(files) -i(image) example1.png example2.png etc.
 * arcbyte -d(directory) -i assets/textures/...
 * -fn=name(filename)
 */

// argc
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s [-n=] [-S=] [-M=] [-m=] [-f|-d] [-i|-s] [files/directory...]\n", argv[0]);
        printf("Options:\n");
        printf("  -n=  Output filename\n");
        printf("  -S=  Signature (must be 4 characters!)\n");
        printf("  -M=  File's version major\n");
        printf("  -m=  File's version minor\n");
        printf("  -f   Process files\n");
        printf("  -d   Process a directory\n");
        printf("  -i   Image mode\n");
        printf("  -s   Sound mode\n");
        return EXIT_FAILURE;
    }

    BinaryContext* context;

    bool process_files = false;
    bool process_directory = false;

    packer_init(&context);

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-n=", 3) == 0) {
            size_t len = strlen(argv[i] + 3) + strlen(".aby") + 1;
            char *output_name = malloc(len);
            if (!output_name) {
                perror("malloc");
                return EXIT_FAILURE;
            }
            strcpy(output_name, argv[i] + 3);
            strcat(output_name, ".aby");
            context->output_name = output_name;
        } else if (strncmp(argv[i], "-S=", 3) == 0) {
            strncpy(context->sig, argv[i] + 3, 4);
        } else if (strncmp(argv[i], "-M=", 3) == 0) {
            context->major = (uint8_t)atoi(argv[i] + 3);
        } else if (strncmp(argv[i], "-m=", 3) == 0) {
            context->minor = (uint8_t)atoi(argv[i] + 3);
        } else if (strcmp(argv[i], "-i") == 0) {
            packer_set_mode(context, TEXTURE);
        } else if (strcmp(argv[i], "-s") == 0) {
            packer_set_mode(context, SOUND);
        } else if (strcmp(argv[i], "-f") == 0) {
            process_files = true;
        } else if (strcmp(argv[i], "-d") == 0) {
            process_directory = true;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') continue;
        context->entry_count++;
    }

    packer_open_file(context->output_name);
    packer_write_header(context, context->sig, context->major, context->minor, context->entry_count);

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (process_files) {
                packer_write_entry(context, _truncate_filepath(argv[i]), argv[i]);
            } else if (process_directory) {
                // TODO: implement!
            }
        }
    }

    packer_shutdown(&context);
    return EXIT_SUCCESS;
}