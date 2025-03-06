#include "parser.h"
#include <cmath>
#include <limits>

#pragma once

void parse_arguments(int* argc, char** &argv, char* &input_file, char* &output_file, int* maxiter, int* freq){
    for (int i = 1; i < *argc; i++) {

        if ((!strcmp(argv[i], "-i")) || (!strcmp(argv[i], "--input"))) {
            input_file = new char[strlen(argv[i+1]) + 1];
            strcpy(input_file, argv[i+1]);
            input_file[strlen(argv[i+1])] = '\0';
            i++;

        } else if((!strcmp(argv[i], "-o")) || (!strcmp(argv[i], "--output"))){
            output_file = new char[strlen(argv[i+1]) + 1];
            strcpy(output_file, argv[i+1]);
            output_file[strlen(argv[i+1])] = '\0';
            i++;

        } else if ((!strcmp(argv[i], "-m")) || (!strcmp(argv[i], "--max-iter"))){
            *maxiter = std::stoi(argv[i + 1]);
            i++;

        } else if ((!strcmp(argv[i], "-f")) || (!strcmp(argv[i], "--freq"))){
            *freq = std::stoi(argv[i + 1]);
            i++;
        }
    }
}
