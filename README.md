# WaterTraceability

The IDE used was Visual Studio Community 2022 (64 bits) version 17.4.4. It was programmed in C++, and Qt 6.4.2 was used for the user interface.

## Input files 

The input files needed to run the program are as follows:

 **DEM:** It is a file with .asc extension containing the terrain elevation in the form of a matrix, whose format is explained below:
- The first 5 lines contain a parameter and a value representing the number of columns in the matrix (ncols), the number of rows in the matrix (nrows), X coordinate of the lower left corner (xllcorner), Y coordinate of the lower corner (yllcorner), and the size of each cell (cellsize).
- From the sixth line onwards, there will be an array in which each cell will have the height of the plot.

**Land use:** It is an .asc file that contains in each cell the land use code. These codes must be integer values.

**Rain map:** It is an .asc file that contains the accumulated rain water data in each cell.

**RP:** This is an optional .csv file that contains a series of UTM coordinates of characteristic points for which information is to be obtained. Each of the lines in this file contains an identifier, the height of the RP, the UTM X coordinate, and the UTM Y coordinate.

**Threshold:** It is a value between 0 and 100 that limits the amount of PRs that will be obtained after running the algorithm, depending on the amount of water accumulated in each cell. This limit is defined by the percentage of accumulated water with respect to the maximum accumulated water found. Therefore, all the PRs whose amount of accumulated water exceeds this limit will be obtained.

**Absorption parameters:** This is a .csv file containing a water absorption value according to the type of land use. Each line contains the use identifier, the type of land use, and the percentage of water absorption.

**Number of iterations:** This is the number of iterations in which the algorithm will run. If the number entered is 0, it will run to complete.

## Output files 

The output of the algorithm is as follows:

- If the **Generate RP** files checkbox is checked, an individual .csv file will be generated for each RP obtained. These files store in each line the amount of water accumulated at the current instant, the land use identifier from which the water comes with the amount of water in that land use.

- **File name:** Name of the .csv file containing one line for each PR obtained. Each line contains the UTM coordinates of the RP, the average amount of accumulated water, the number of times that water has arrived or discharged at this point, maximum amount of accumulated water, number of land uses that have brought water to this point, the different land uses separated by ':', the amount of water coming from each land use separated by ':', and the percentage with respect to the total of each land use separated by ':'.

- **Image name:** Name of the image that will have the calculated drainage network.

