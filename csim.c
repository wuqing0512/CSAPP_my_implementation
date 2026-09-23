/*
File        :   csim.c
Author      :   Wuqing
Time        :   
Affiliation :   ZJU AA
Note        :   This code is intended solely for 
personal study and review purposes. If there are
any copyright issues, please contact me for removal.
The code implementation of the former has less function
call overhead, but I'm still debugging it. The latter
one is easier to understand and already passed the test.
Email       :   3240104276@zju.edu.cn
*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <getopt.h>
// #include "cachelab.h"

// #define inf 100000
// // define cache parameters
// bool v = false;
// int s, E, b, t;
// // considering the E is not too big, we use timestamp rather than hash table
// // in the LRU. the time complexity rises to O(E).
// int timestamp = 0;
// unsigned int hit = 0, miss = 0, eviction = 0;

// // define struct poniter set which has E lines.
// typedef struct line* set;
// typedef struct line {
//     bool valid;
//     int tag;
//     int count;
// } line;

// void print_help() {
//     puts("FORMAT  :\n./foo [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
//     puts("OPTION  :\n");
//     puts("-h: Optional help flag that prints usage info\n");
//     puts("-v: Optional verbose flag that displays trace info\n");
//     puts("-s: <s>: Number of set index bits (S = 2^s is the number of sets)\n");
//     puts("-E: <E>: Associativity (number of lines per set)\n");
//     puts("-b: <b>: Number of block bits (B = 2^b is the block size)\n");
//     puts("-t: <tracefile>: Name of the valgrind trace to replay\n");
// }

// void access_cache(set* cache, uint64_t addr, int is_Modify) {
//     int set_idx = addr >> b & ((1 << s) - 1);
//     int tag_idx = addr >> (b + s);
//     int min_cnt = inf;
//     int tmp = -1;
//     // refresh the timestamp
//     timestamp++;

//     for (int i = 0; i < E; i++) {
//         // If is_Modify, it may bring 1 Miss + 1 Hit or 2 Hits
//         line* current_line = cache[set_idx] + i;
//         if (current_line->valid && current_line->tag == tag_idx) {
//             hit++;
//             hit += is_Modify;
//             current_line->count = timestamp;
//             if (v) { printf("hit\n"); }
//             // If is_Modify, it will store again.
//             if (!is_Modify) { return; }
//         }
//     }
//     miss++;
//     // perform store
//     for (int i = 0; i < E; i++) {
//         line* current_line = cache[set_idx] + i;
//         if (current_line->valid) {
//             // min_cnt = current_line->count < min_cnt ? current_line->count : min_cnt;
//             tmp = current_line->count < min_cnt ? i : tmp;
//             continue;
//         }
//         // find spare line to store
//         current_line->valid = true;
//         current_line->tag = tag_idx;
//         current_line->count = timestamp;
//         // if store successfully, return in advance
//         return; 
//     }
//     eviction++;
//     // perform eviction
//     line* current_line = cache[set_idx] + tmp;
//     current_line->tag = tag_idx;
//     current_line->count = timestamp;
//     return;
// }
    
// int main(int argc, char* argv[]) {
//     int size;
//     char operation;
//     uint64_t addr;
//     FILE* trace_file;

//     if (argc == 1) {
//         print_help();
//         exit(0); // use exit rather than return
//     }
//     // get parameters
//     int param;
//     while((param = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
//         switch (param) {
//         case 'h':
//             print_help();
//             exit(0);
//         case 'v':
//             v = true;
//             break;
//         case 's':
//             s = atoi(optarg); // optarg point to the current param
//             break;
//         case 'E':
//             E = atoi(optarg);
//             break;
//         case 'b':
//             b = atoi(optarg);
//             break;
//         case 't':
//             trace_file = fopen(optarg, "r");
//             break;
//         default:
//             print_help();
//             exit(0);
//         }
//      }
//      // verify the validation
//     if (s <= 0 || E <= 0 || b <= 0 || s + b > 64 || !trace_file) {
//         print_help();
//         exit(1); // exit with usage help
//     }
//     // memory allocation
//     set* cache = (set *)malloc(sizeof(set) * (1 << s)); // cache is array of sets
//     if (!cache) { printf("malloc error!\n");  exit(1); }
//     for (int i = 0; i < (1 << s); i++) {
//         cache[i] = (set)malloc(sizeof(line) * E);
//         // initialize the lines
//         for (int j = 0; j < E; j++) {
//             (cache[i] + j)->valid = false;
//             (cache[i] + j)->tag = -1;
//             (cache[i] + j)->count = -1;
//         }
//     }

//     while (fscanf(trace_file, " %c %lx,%d\n", &operation, &addr, &size) == 3) {
//         timestamp++;
//         if (v) {
//             printf("%c %lx,%d\n", operation, addr, size);
//         }
//         switch (operation) {
//         case 'I':   continue;
//         case 'M':
//             access_cache(cache, addr, 1);
//             break;
//         case 'L':
//         case 'S':
//             access_cache(cache, addr, 0);
//             break;
//         }
//     }
//     for (int i = 0; i < (1 << s); i++) {
//         free(cache[i]);
//     }
//     free(cache);
//     cache = NULL;
//     printSummary(hit, miss, eviction);
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include "cachelab.h"

#define inf 100000
// define cache parameters
bool v = false;
int s, E, b, t;
// considering the E is not too big, we use timestamp rather than hash table
// in the LRU. the time complexity rises to O(E).
int timestamp = 0;
unsigned int hit = 0, miss = 0, eviction = 0;

// define struct pointer set which has E lines.
typedef struct line* set;
typedef struct line {
    bool valid;
    int tag;
    int count;
} line;

void print_help() {
    puts("FORMAT  :\n./foo [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    puts("OPTION  :\n");
    puts("-h: Optional help flag that prints usage info\n");
    puts("-v: Optional verbose flag that displays trace info\n");
    puts("-s: <s>: Number of set index bits (S = 2^s is the number of sets)\n");
    puts("-E: <E>: Associativity (number of lines per set)\n");
    puts("-b: <b>: Number of block bits (B = 2^b is the block size)\n");
    puts("-t: <tracefile>: Name of the valgrind trace to replay\n");
}

void access_cache(set* cache, uint64_t addr) {
    int set_idx = (addr >> b) & ((1 << s) - 1);
    int tag_idx = addr >> (b + s);
    
    timestamp++;
    set current_set = cache[set_idx];

    // Hit
    for (int i = 0; i < E; i++) {
        line* current_line = cache[set_idx] + i;
        if (current_line->valid && current_line->tag == tag_idx) {
            hit++;
            current_line->count = timestamp;
            if (v) { printf("hit "); }
            return;
        }
    }

    // Miss
    miss++;
    if (v) { printf("miss "); }

    // Find the spare line
    for (int i = 0; i < E; i++) {
        line* current_line = cache[set_idx] + i;
        if (!current_line->valid) {
            current_line->valid = true;
            current_line->tag = tag_idx;
            current_line->count = timestamp;
            return;
        }
    }

    // Eviction
    eviction++;
    if (v) { printf("eviction "); }

    int min_cnt = current_set[0].count;
    int evict_idx = 0;
    for (int i = 1; i < E; i++) {
        line* current_line = cache[set_idx] + i;
        if (current_line->count < min_cnt) {
            min_cnt = current_line->count;
            evict_idx = i;
        }
    }

    current_set[evict_idx].tag = tag_idx;
    current_set[evict_idx].count = timestamp;
}
    
int main(int argc, char* argv[]) {
    int size;
    char operation;
    uint64_t addr;
    FILE* trace_file = NULL;

    if (argc == 1) {
        print_help();
        exit(0);
    }
    
    // get parameters
    int param;
    while((param = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (param) {
        case 'h':
            print_help();
            exit(0);
        case 'v':
            v = true;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = fopen(optarg, "r");
            break;
        default:
            print_help();
            exit(0);
        }
     }
     
    // verify the validation
    if (s <= 0 || E <= 0 || b <= 0 || s + b > 64 || !trace_file) {
        print_help();
        exit(1);
    }
    
    // memory allocation
    set* cache = (set *)malloc(sizeof(set) * (1 << s));
    if (!cache) { printf("malloc error!\n"); exit(1); }
    for (int i = 0; i < (1 << s); i++) {
        cache[i] = (set)malloc(sizeof(line) * E);
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = false;
            cache[i][j].tag = -1;
            cache[i][j].count = -1;
        }
    }

    while (fscanf(trace_file, " %c %lx,%d", &operation, &addr, &size) == 3) {
        if (v) {
            printf("%c %lx,%d ", operation, addr, size);
        }
        switch (operation) {
        case 'I':  
            continue;
        case 'M':
            // Modify = Load + Store
            access_cache(cache, addr);
            access_cache(cache, addr);
            break;
        case 'L':
        case 'S':
            access_cache(cache, addr);
            break;
        }
        if (v) {
            printf("\n");
        }
    }

    for (int i = 0; i < (1 << s); i++) {
        free(cache[i]);
    }
    free(cache);
    cache = NULL;
    
    fclose(trace_file);
    printSummary(hit, miss, eviction);
    return 0;
}