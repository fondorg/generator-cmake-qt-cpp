# CMake Qt Yeoman Generator

Generate CMake project leveraging Qt UI framework.  
This a simple scaffolding tool that helps to get started with CMake based Qt project.

## Installation

Install [Node.js](https://nodejs.org/en/)  
Install [Yeoman](http://yeoman.io/) and generator-cmake-qt-cpp:
```bash
npm install -g yo
npm install -g generator-cmake-qt-cpp
```

## Usage
Create empty directory and navigate into it.  
Run Yeoman generator:  
```bash
yo cmake-qt-cpp
```
Follow instruction to configure the application.

## Features
Generates Qt application with the following features:  
* CMake build system
* Simple GUI class
* Background tasks execution with status reporting
* Saving fields in settings on change
* Optional OpenCV library inclusion
* Minor useful functions (modal message, file and directory opening)

