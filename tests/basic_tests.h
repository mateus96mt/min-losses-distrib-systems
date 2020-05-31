//
// Created by mateus on 31/05/2020.
//
#include "include/Random_keys.h"

#ifndef MIN_PERDAS_BASIC_TESTS_H
#define MIN_PERDAS_BASIC_TESTS_H

class BasicTests {
public:
    static void readNetworkTest(string networkSourceFile);

    static void brandaoTest(string input_file);

    static void Capacitor_Test_GA(string savename);

    static void print_tree(const string savefolder, Graph *g);
};

#endif //MIN_PERDAS_BASIC_TESTS_H
