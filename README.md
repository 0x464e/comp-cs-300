# COMP.CS.300 Data Structures and Algorithms 1 
This repository contains the project work I did for my university's first data structures & algorithms C++ course. The folders prg1 and prg2 contain the first and second part of the project.  
There isn't much reason to look at the prg1 folder, since prg2 just adds onto prg1.  
**Note: only the data structures class was implemented by me, the rest was implemented by this course's professor.**

In this project/course the student learns to consider efficiency and make efficiency estimations using the O, Omega and Theta notations, use STL algorithms, create different types of algorithms and use appropriate data structures in C++.  
Personally I used many STL algorithms, created recursive algorithms, and created/used graph algorithms (BFS, DFS, A*, Kruskal).

![img](https://i.imgur.com/JWrwQe7.png)

## Project's assignment
You can find the project assignment pdfs for [part 1](https://github.com/0x464e/comp-cs-300/blob/master/prg1/prg1-game-of-taxes-en.pdf) and [part 2](https://github.com/0x464e/comp-cs-300/blob/master/prg2/prg2-connecting-towns-en.pdf) of the project in their respective folders.  
By reading the assignments, you should understand fully how to use the program and how everything should work <sub><sup>(in my implementation everything does work as it should)</sup></sub>.  
I did, however, move the many `.txt` testing files that are referenced to their own subfolder due to them causing so much clutter.  


## Dependencies

The GUI is written with Qt, so Qt binaries are required for compiling/building.  
Qt is found at https://www.qt.io/.  
You could also run a non GUI version of this app, see more in the Building section.

## Building

### Building with Qt Creator
This is going to be the easiest alternative and I'd probably recommend this if you want everything to work easily.  
Qt creator is Qt's own IDE.
* Clone this repository  
`git clone https://github.com/0x464e/comp-cs-300`
* Install Qt (and Qt Creator) from https://www.qt.io/
* Launch Qt Creator
* Open an existing project from the `.pro` file in this repository and configure with the defaults settings will suffice
    * Choose Build → Build Project 
    * **Or** just run it directly with Qt Creator by hitting `Run` in the bottom left corner
* By default the build output will be found just outside your project's directory in a build folder
* Run the binary

### Build with Visual Studio
This is doable as well, but you still need to install Qt, of course.
* Clone this repository  
`git clone https://github.com/0x464e/comp-cs-300`
* Install Qt from https://www.qt.io/
* Install Qt Visual Studio Tools from the [Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123) and follow their setup instructions
* Launch Visual Studio and open the `.pro` in this repository file by choosing  
Qt VS Tools → Open Qt Project File (.pro)...
* Build a release or just run it straight with Visual studio
* The resulting Windows binary is found in your release/ folder and can be ran standalone as long as your Qt binaries are found in your PATH  
To be able to run it without depending on having Qt installed, see [Deployment](#deployment)

### Building in the command line
A valid option as long as your build environment is set up correctly.  
But still, Qt of course needs to be installed
* Clone this repository  
`git clone https://github.com/0x464e/comp-cs-300`
* Install Qt from https://www.qt.io/
* Ensure that your Qt binaries are found in your PATH  
<sub><sup>(So add your Qt's `bin/` folder to path, if it isn't there already.)</sup></sub>
* Add the following line to the `.pro` file found in this repository:  
`INCLUDEPATH += .`
* Run  
    ```
    qmake -makefile
    make
    ```
* Run the resulting binary

### Building only a commandline version
You could ignore all the Qt stuff and not have a gui, and just use this as a good old commandline app.  
Just one g++ command should be enough, e.g:
```
g++ -pedantic -Wall -std=c++17 mainprogram.cc mainwindow.cc datastructures.cc -o prg  
```

## Running
When running the program, if you want to be able to make use of the many testing files <sub><sup>(found inside the `testing-files` folder)</sup></sub>, be sure to copy them over to next to your executable, or in your IDE, set your run directory to be where the test files are.  
Otherwise file references like `./test-file.txt` will say there is no such file.

## Deployment

Deployment instructions are found from the Qt documentation  
https://doc.qt.io/qt-5/deployment.html
