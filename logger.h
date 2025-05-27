#ifndef LOGGER_H
#define LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

void logger_init(int local_capacity, int total_capacity);
void logger_log(long timestamp_us, double a, double b, double c);
void logger_flush(const char* filename);
void logger_free();

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H

// =============== Optional Header-Only Mode ===============
#ifdef LOGGER_IMPLEMENTATION
#ifndef LOGGER_IMPLEMENTED
#define LOGGER_IMPLEMENTED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    long timestamp_us;
    double a, b, c;
} LogEntry;

static LogEntry* local_buffer = NULL;
static LogEntry* total_buffer = NULL;
static size_t local_capacity = 0;
static size_t total_capacity = 0;
static size_t local_index = 0;
static size_t total_index = 0;

void logger_init(int local_cap, int total_cap) {
    local_capacity = local_cap;
    total_capacity = total_cap;
    local_index = 0;
    total_index = 0;

    local_buffer = (LogEntry*)malloc(sizeof(LogEntry) * local_capacity);
    total_buffer = (LogEntry*)malloc(sizeof(LogEntry) * total_capacity);
}

void logger_log(long timestamp_us, double a, double b, double c) {
    if (!local_buffer || !total_buffer) return;

    local_buffer[local_index++] = (LogEntry){ timestamp_us, a, b, c };

    if (local_index == local_capacity) {
        if (total_index + local_capacity <= total_capacity) {
            memcpy(total_buffer + total_index, local_buffer, sizeof(LogEntry) * local_capacity);
            total_index += local_capacity;
        } else {
            // Drop if full
            fprintf(stderr, "[LOGGER] Warning: total buffer full, discarding data.\n");
        }
        local_index = 0;
    }
}

void logger_flush(const char* filename) {
    if (!total_buffer) return;

    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "[LOGGER] Could not open %s for writing\n", filename);
        return;
    }

    fprintf(f, "timestamp_us,a,b,c\n");
    for (size_t i = 0; i < total_index; ++i) {
        fprintf(f, "%ld,%.10f,%.10f,%.10f\n",
                total_buffer[i].timestamp_us,
                total_buffer[i].a,
                total_buffer[i].b,
                total_buffer[i].c);
    }

    fclose(f);
    printf("[LOGGER] Wrote %zu samples to %s\n", total_index, filename);
}

void logger_free() {
    free(local_buffer);
    free(total_buffer);
    local_buffer = NULL;
    total_buffer = NULL;
}

#endif // LOGGER_IMPLEMENTED
#endif // LOGGER_IMPLEMENTATION
