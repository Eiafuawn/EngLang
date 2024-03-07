#!/bin/env bash
clang++ -c ast.cpp -o ast.o
clang++ -c lexer.cpp -o lexer.o
clang++ -c parser.cpp -o parser.o
clang++ -c main.cpp -o main.o

clang++ lexer.o parser.o ast.o main.o -o test 

