cmake_minimum_required(VERSION 3.19)

project(Calculator)

add_executable(Calculator   main.c 
                            common_types.h
                            lexer.h 
                            lexer.c 
                            token_tree.h 
                            token_tree.c) 