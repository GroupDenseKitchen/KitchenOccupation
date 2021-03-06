Project Kitchen Occupation 
==========================

This project is part of the course TSBB11: Images and Graphics, Project Course CDIO at Linköping University 2013. The goal of this project is to produce a system that, using computer vision, can monitor the occupancy of student kitchens at Linköping University.

***

Our project [wiki](https://github.com/GroupDenseKitchen/project/wiki) contains some information about how we develop. 

***
##Overview of system
The system recieve video feeds or files (one for each camera) and estimates the number of people in one or several rooms.
This is archieved by sending the video frames one at a time through a image processing and analytics pipeline, with each algorithm in the pipeline building on the results of the previous.
It is configurable and debuggable via a GUI which increment the pipeline, apart from this the GUI is not at all required for the execution of the system.
Configurations are read from file aswell.

![Overview](readme_images/DenseKitchen%20Overview.jpg)

### DenseKitchen
This is the core class for the person density estimation. It is owned and run byt the debug gui. It is resposible for administring and syncronizing the processing pipeline, and passing around the process data. 

### Frame
The Frame class is the main data container for any data needed by the processing pipeline. Each object is responsible for data pertaining to one time step.

### Network
The Network module is responsible for reading images from the involved cameras and comunicating with external systems.

### ImageProcessor
The ImageProcessor class is derived from Algorithm and is responsible for the image processing part of the pipeline. It holds an ordered collection of the objects responsible for each image processing step (also derived from Algorithm).

### Statistics
The statistics class has the same structure as the ImageProcessor but is responsible for aggregation and analysis of the output from the image processing part of the pipeline.

### Evaluator
The Evaluator evaluates the whole system by looking at the MOTA and MOTP metrics of tracking performance.

***
## Algorithm pipeline architecture
Algorithm is a base class that can own other algorithms. 
When Algorithm::initialize and Algorithm::process is called all its sub-Algorithms are called in order as well.

### Adding a new sub-Algorithm
Algorithms can be added to other Algorithms using the config file, this however requires that an AlgorithmFactory is instanciated and that the desired algorithm to be added is added to the factory first.
This can look like the following in the config file:
```yml
ImageProcessor:
   - BackgroundModelAlgorithm
   - ForegroundSegmentationAlgorithm
   - TrackingAlgorithm
```
where ImageProcessor is an Algorithm as well as the three listed under it which are to be added in that order to ImageProcessor. is called all its algorithms are called in order as well.

This requires the Algorithm to be registered in a AlgorithmFactory, a one liner:
```c++
algorithmFactory.add("BackgroundModelAlgorithm", new BackgroundModelAlgorithm()); 
...
```
### Setting up algorithms to register sub-Algorithms
The required code to register, add and initialize BackgroundModelALgorithm to ImageProcessor is as follows.

First we add the BackgroundModelAlgorithm algorithm to the algorithmFactory. The first argument is the string used to address that specific Algorithm in the config file.
```c++
algorithmFactory.add("BackgroundModelAlgorithm", new BackgroundModelAlgorithm()); 
...
```
Second we populate ImageProcessor using the config file (add all Algorithms in order as stated in the config file). The ConfigurationManager instanciation is here called settings.
```c++
imageProcessor.populateSubAlgorithms(settings, "ImageProcessor", algorithmFactory);
```
Lastly we initialize ImageProcessor (and by doing so we also initialize BackgroundModelAlgorithm et.al.)
```c++
imageProcessor.initialize(settings);
```
Both ImageProcessor::populate and ImageProcessor::initialize return false if something is not as it should, they also log the problem.


### Compilation instructions
The following libraries are needed to compile the program
* Opencv 2.4.6+
* QT5.1 (for the gui parts)
* libcurl (for communication with a REST API)
* rapidxml (used in the evaluation parts, included in the project)
* For windows systems OpenNI2 is also required to communicate with the kinect
* For unix likes libfreenect is needed instead.

For the acual build process you need:
* A c++11 capable compiler (clang/gcc4.8+/visual studio 2012)
* cmake 2.8.7
* doxygen (only to generate documentation)
* latex (only to build the manual/technical docs)

In order to build the program, run cmake on the src/ folder and then build the project using make.
