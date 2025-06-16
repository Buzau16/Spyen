#include <Archiver.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef enum DataTypes {
    NONE,
    TEXTURE,
    SOUND,
    OTHER
} DataType;

struct FileData {
    char* output_name;
    char sig[4];
    uint8_t major;
    uint8_t minor;
};

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

struct ImageEntry {
    uint32_t hash;
    uint32_t offset;
    uint32_t size;
    uint16_t width;
    uint16_t height;
    uint8_t channels;
    void* data;
};

#define OTHER_ENTRY_NO_PTR_SIZE 12
#define IMAGE_ENTRY_NO_PTR_SIZE 20

struct File {
    Header header;
    Entry* other_entries;
    ImageEntry* image_entries;
    uint32_t current_data_offset;
    DataType type;
};

FILE* output_file;
File binary_file;

bool packer_init(const char *filename) {
    if (output_file) {
        printf("File already opened!\n");
        return EXIT_FAILURE;
    }
    printf("Initializing arcbyte\n");

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

    printf("Shutting down\n");

    // write the header entry count
    fseek(output_file, 6, SEEK_SET);
    _write_u16_le(binary_file.header.entry_count);


    switch (binary_file.type) {
        case NONE: {
            printf("Data type is NONE!!");
            return EXIT_FAILURE;
        }
        case TEXTURE: {
            binary_file.current_data_offset = sizeof(Header) + (IMAGE_ENTRY_NO_PTR_SIZE * binary_file.header.entry_count);
            for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
                _write_u32_le(binary_file.image_entries[i].hash);
                _write_u32_le(binary_file.current_data_offset);
                binary_file.current_data_offset += binary_file.image_entries[i].size;
                _write_u32_le(binary_file.image_entries[i].size);
                _write_u16_le(binary_file.image_entries[i].width);
                _write_u16_le(binary_file.image_entries[i].height);
                _write_u16_le(binary_file.image_entries[i].channels);
            }

            for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
                fwrite(binary_file.image_entries[i].data, binary_file.image_entries[i].size, 1, output_file);
                free(binary_file.image_entries[i].data);
            }

            free(binary_file.image_entries);
            fclose(output_file);

            printf("Shutdown complete! Enjoy your file!\n");
            return EXIT_SUCCESS;
        }
        case SOUND: {
            break;
        }
        case OTHER: {
            binary_file.current_data_offset = sizeof(Header) + (OTHER_ENTRY_NO_PTR_SIZE * binary_file.header.entry_count);

            // write the entries, NO DATA!
            for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
                _write_u32_le(binary_file.other_entries[i].hash);
                _write_u32_le(binary_file.current_data_offset);
                binary_file.current_data_offset += binary_file.other_entries[i].size;
                _write_u32_le(binary_file.other_entries[i].size);
            }

            // Write the data
            for (uint32_t i = 0; i < binary_file.header.entry_count; i++) {
                fwrite(binary_file.other_entries[i].data, binary_file.other_entries[i].size, 1, output_file);
                free(binary_file.other_entries[i].data);
            }

            free(binary_file.other_entries);
            fclose(output_file);

            printf("Shutdown complete! Enjoy your file!\n");
            return EXIT_SUCCESS;
        }
    }


    printf("Failed to shutdown!\n");
    return EXIT_FAILURE;
}
void packer_set_mode(const uint8_t mode) {
    binary_file.type = mode;
}
bool packer_write_header(const char *signature, uint8_t version_major, uint8_t version_minor) {
    if (!output_file) {
        printf("Cannot write to an unopened file! %s\n", __FUNCTION__);
        return EXIT_FAILURE;
    }
    printf("Writing the header\n");

    _write_u32_le(_char_to_u32_le(signature));
    _write_u8_le(version_major);
    _write_u8_le(version_minor);
    _write_u16_le(0);

    return EXIT_SUCCESS;
}
//
bool packer_add_entry(const char *name, const char *filepath, const void *data, const size_t size) {
    printf("Adding entry: %s\n", name);
    uint32_t INDEX = binary_file.header.entry_count;
    switch (binary_file.type) {
        case NONE: {
            printf("No type set! %s\n", __FUNCTION__);
            return EXIT_FAILURE;
        }
        case TEXTURE: {
            _resize_entry_buffer(sizeof(ImageEntry) * (binary_file.header.entry_count + 1));
            int x, y, c;
            unsigned char *image_data = stbi_load(filepath, &x, &y, &c, 0);
            if (!image_data) {
                printf("Failed to load image: %s\n", filepath);
                return EXIT_FAILURE;
            }

            binary_file.image_entries[INDEX].width = (uint16_t) x;
            binary_file.image_entries[INDEX].height = (uint16_t) y;
            binary_file.image_entries[INDEX].channels = (uint8_t) c;
            binary_file.image_entries[INDEX].hash = _fnv1a_hash(name);
            binary_file.image_entries[INDEX].size = binary_file.image_entries[INDEX].width *
                                                    binary_file.image_entries[INDEX].height *
                                                    binary_file.image_entries[INDEX].channels;
            binary_file.image_entries[INDEX].data = malloc(binary_file.image_entries[INDEX].size);
            memcpy(binary_file.image_entries[INDEX].data, image_data, binary_file.image_entries[INDEX].size);

            stbi_image_free(image_data);
            binary_file.header.entry_count++;
            return EXIT_SUCCESS;
        }
        case SOUND: {
            break;
        }
        case OTHER: {
            if (size == 0) {
                printf("Please provide the data's size!\n");
                return EXIT_FAILURE;
            }
            if (data == NULL) {
                printf("Please provide the data!\n");
                return EXIT_FAILURE;
            }
            _resize_entry_buffer(sizeof(Entry) * (binary_file.header.entry_count + 1));

            binary_file.other_entries[INDEX].hash = _fnv1a_hash(name);
            binary_file.other_entries[INDEX].size = size;
            binary_file.other_entries[INDEX].data = malloc(size);
            memcpy(binary_file.other_entries[INDEX].data, data, size);

            binary_file.header.entry_count++;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}
bool _resize_entry_buffer(const size_t size) {
    switch (binary_file.type) {
        case NONE: {
            printf("DATA TYPE IS NONE!!\n");
            return EXIT_FAILURE;
        }
        case TEXTURE: {
            ImageEntry* temp = realloc(binary_file.image_entries, size);
            if (!temp) {
                perror("FAILED TO REALLOCATE THE BUFFER!");
                return EXIT_FAILURE;
            }
            binary_file.image_entries = temp;
            return EXIT_SUCCESS;
        }
        case SOUND:{
            break;
        }
        case OTHER: {
            Entry* temp = realloc(binary_file.other_entries, size);
            if (!temp) {
                perror("FAILED TO REALLOCATE THE BUFFER!");
                return EXIT_FAILURE;
            }
            binary_file.other_entries = temp;
            return EXIT_SUCCESS;
        }
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

    FileData fd = {
        .output_name = "asp.aby",
        .major = 0,
        .minor = 1,
        .sig = "ARBY"
    };

    bool process_files = false;
    bool process_directory = false;

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
            fd.output_name = output_name;
        } else if (strncmp(argv[i], "-S=", 3) == 0) {
            strncpy(fd.sig, argv[i] + 3, 4);
        } else if (strncmp(argv[i], "-M=", 3) == 0) {
            fd.major = _char_to_u8_le(argv[i][3]);
        } else if (strncmp(argv[i], "-m=", 3) == 0) {
            fd.minor = _char_to_u8_le(argv[i][3]);
        } else if (strcmp(argv[i], "-i") == 0) {
            packer_set_mode(TEXTURE);
        } else if (strcmp(argv[i], "-s") == 0) {
            packer_set_mode(SOUND);
        } else if (strcmp(argv[i], "-f") == 0) {
            process_files = true;
        } else if (strcmp(argv[i], "-d") == 0) {
            process_directory = true;
        }
    }

    packer_init(fd.output_name);
    packer_write_header(fd.sig, fd.major, fd.minor);

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (process_files) {
                packer_add_entry(_truncate_filepath(argv[i]), argv[i], NULL, 0);
            } else if (process_directory) {
                // TODO: implement!
            }
        }
    }

    packer_shutdown();
    return EXIT_SUCCESS;
}