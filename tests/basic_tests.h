//
// Created by mateus on 31/05/2020.
//
#include "include/Random_keys.h"

#ifndef MIN_PERDAS_BASIC_TESTS_H
#define MIN_PERDAS_BASIC_TESTS_H

class BasicTests {
public:
    static void readNetworkTest(char *networkSourceFile);

    static void brandaoTest(string input_file);

    static void Capacitor_Test_GA(string savename);

    static void print_tree(const string savefolder, Graph *g);

    static  void testRandomKeysAG(string networkSourceFile, int populationSize, int numGenerations);

    static void testCalculationOfObjectFunctionOpmized(string networkSourceFile);

    static void testRandomKeys(char *networkSourceFile);
};

#endif //MIN_PERDAS_BASIC_TESTS_H
