#!/bin/bash

doxygen Doxyfile

make -C ./doc/latex
