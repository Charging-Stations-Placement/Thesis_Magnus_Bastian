# Thesis_Magnus_Bastian

## Introduction

This repository contains the code and data used for the thesis of Magnus and Bastian. In this repository, you can find the following folders:

- `AllData/`: This is the test and code for creating the gravity model for the case where all charging stations are included in the model.
- `Urban/`: This is the test and code for creating the gravity model for the case where charging stations in the city are included in the model.
- `Motorway/`: This is the test and code for creating the gravity model for the case where charging stations on the motorway are included in the model. 

Then there is the folder `MapApp`/` this is the code for the map application. Users can use the gravity model to get heat maps interactively and use it to get an understanding of how to place new stations or where the station is mostly to be used.

## Genectic Algorithm Weight Optimization
The first three folders contain the same code but with different data. The structure is as follows:
- `Datasets/`: Contains the data used for the gravity model. The data is in the form of a CSV file.
- `gravityscore.cpp`: This is the code for the gravity model. It executes all the mathematical calculations and returns the gravity scores for each station input to it.
- `GAModels.ipynb`: This is the code for the genetic algorithm. It uses the gravity model to calculate the fitness of the population and then uses the fitness to evolve the weighting of the gravity model.
- `Run_Gravity.ipynb`: This is a test file where different weights can be tested on the gravity model. It is used to see how the gravity model works and how the weights affect the gravity model. (In the `ManuelWeights` folder there is a file called `ManuelWeights.ipynb` that can be used to test the gravity model with manual weights.)

In Urban and Motorway there is a file called `ModelPredictUse.ipynb` this file contains code that tries to predict the use of the charging stations based on the data that we have gathered and the gravity model that we have created. This is done with linear regression and the code is used to see how well the gravity model can predict the use of the charging stations.


## Map Application
The Map Application is a Visual Studio project that uses the gravity model to create heat maps. The user can interact with the map and see how the heat map is at different locations and zooms. To run this code you need to have ...



