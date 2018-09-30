#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define ROW_SIZE 16

void PrintUsage(const char* cmd) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s output.cpp [file1.txt FILE1 [file2.dat FILE2 [...]]]\n", cmd);
    fprintf(stderr, "\n");
    fprintf(stderr, "Generates the C++ source file named 'output.cpp' embedding\n"
        "the content of 'file1.txt' as const char FILE1[], etc.\n");
    fprintf(stderr, "\n");
}

int WriteByte(FILE* out, int value, size_t count) {
    return fprintf(out, "%s'\\x%02x',", (count % ROW_SIZE == 0) ? "\n    " : " ", value);
}

int WriteFile(FILE* out, FILE* in, const char* constant) {
    size_t byteCount = 0;
    int value;
    if (fprintf(out, "extern const char %s_DATA[] = {", constant) < 0) {
        return 1;
    }
    while ((value = fgetc(in)) != EOF) {
        if (WriteByte(out, value, byteCount) < 0) {
            return 1;
        }
        byteCount++;
    }
    if (WriteByte(out, 0, byteCount) < 0) {
        return 1;
    }
    if (fprintf(out, "\n};\n\n") < 0) {
        return 1;
    }
    if (fprintf(out, "extern const size_t %s_LENGTH = %zu;\n\n", constant, byteCount) < 0) {
        return 1;
    }
    return 0;
}

int Run(FILE* out, int pairCount, char** pairs) {
    int i;
    if (fprintf(out, "#include <cstddef>\n\n") < 0) {
        return 1;
    }

    for (i = 0; i < pairCount; ++i) {
        const char* fileName = pairs[2 * i + 0];
        const char* constant = pairs[2 * i + 1];

        FILE* in = fopen(fileName, "rb");
        if (!in) {
            perror("could not open input file");
            return 1;
        }
        if (WriteFile(out, in, constant) != 0) {
            return 1;
        }
        if (fclose(in) == EOF) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (!(argc >= 2 && argc % 2 == 0)) {
        PrintUsage(argv[0]);
        return 2;
    }

    FILE* out = fopen(argv[1], "wb");
    if (!out) {
        perror("could not create output file");
        return 1;
    }

    int ret = Run(out, (argc - 2) / 2, argv + 2);

    if (fclose(out) == EOF) {
        perror("could not close output file");
        return 1;
    }

    return ret;
}
