Project Kitchen Occupation 
==========================

This project is part of the course TSBB11: Images and Graphics, Project Course CDIO at Linköping University 2013. The goal of this project is to produce a system that, using computer vision, can monitor the occupancy of student kitchens at Linköping University.

***

Our project [wiki](https://github.com/GroupDenseKitchen/project/wiki) contains some information about how we develop. 

***
##Overview of system


### DenseKitchen
This is the core class for the person density estimation. It is owned and run byt the debug gui. It is resposible for administring and syncronizing the processing pipeline, and passing around the process data. 

### Frame
The Frame class is the main data container for any data needed by the processing pipeline. Each object is responsible for data pertaining to one time step.

### Network
The Network module is responsible for reading images the involved cameras and comunicating with external systems.

### ImageProcessor
The ImageProcessor class is derived from Algorithm and is responsible for the image processing part of the pipeline. It holds an ordered collection of the objects responsible for each image processing step (also derived from Algorithm).

### Statistics

### Evaluator

![Overview](https://doc-0s-8o-docs.googleusercontent.com/docs/securesc/5btq9f7huc8bqtncm8qh0s5eefd7hgmg/eoaigplsae5m486ulbr76qr2ok2d7b6h/1383652800000/06151035857182574422/06151035857182574422/0B53Hlj6c__71RjhrMzktdG1aWm8?h=12494863305412751534&e=view)
