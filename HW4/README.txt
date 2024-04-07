# EE569 Homework Assignment #4
# Date: Mar 28, 2024
# Name: Kuimu Ren
# ID: 1473482531
# email: kuimuren@usc.edu
#
# Software: Visual Studio Code
# language: C++

After unzipped the file:
1.Put all the input images in images in the main directory

2.Change the address in set to the local opencv library address

3.Change the outputPath in the main method of p1/p2/p3 to the absolute address of the HW4 project

4.I have added a cmake task to launch.json that executes cmake to compile the code 
from cmakelists into an executable file, which is placed in the bin folder.

5.By executing the executable for each of the three problems in bin, 
you can execute the main method of the corresponding problem

6.After the program runs, all the resulting images will be in the resultImage under the corresponding problem

========================================================================
    CONSOLE APPLICATION : [Problem1] Project Overview
========================================================================

TextureAnalysis.h/TextureAnalysis.cpp :
    This is the solution for the problem1

RWImage.h/RWImage.cpp :
    This is the solution for the input/output of image

========================================================================
    CONSOLE APPLICATION : [Problem2] Project Overview
========================================================================
 
TextureSegmentation.h/TextureSegmentation.cpp :
    This is the solution for the problem2

RWImage.h/RWImage.cpp :
    This is the solution for the input/output of image

========================================================================
    CONSOLE APPLICATION : [Problem3] Project Overview
========================================================================

ImageMatching.h/ImageMatching.cpp :
    This is the solution for the problem3

RWImage.h/RWImage.cpp :
    This is the solution for the input/output of image