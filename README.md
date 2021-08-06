# CleanDBS

Real-time removal of stimulation artefacts in closed-loop deep brain stimulation.

The artifact peaks were detected by applying a threshold to the raw recordings, and the samples within the contaminated period of the stimulation pulses were excluded and replaced with the interpolation of the samples prior to and after the stimulation artifact duration.

## How to use

### Add source files to your c++ project

This method was implemented in the class named `CleanDBS`. 

You need to add both `cleandbs.h` and `cleandbs.cpp` to your c++ project. These files can be found in the `source` folder. 

### Include the header file

```C++
#include "cleandbs.h"
```

### Initialize the CleanDBS object

```C++
double fs = 44000; // Sampling rate of raw signal
double fsOut = 300; // Output sampling rate
double preArtifact = 0.0002; // Skip 'preArtifact' second ahead of each artifact peak
double postArtifact = 0.005; // Skip 'postArtifact' second after each artifact peak
double thr = 3000; // Threshold for artifact peak detection

CleanDBS cleanObj(fs, fsOut, preArtifact, postArtifact, thr);
```

### Real-time processing

Once the CleanDBS object was successfully initialized, the raw signal can be processed one sample by sample by calling the `recieve` function.

When the stimulation state is known, the timing of the upcoming artifact was predicted by the timing of last peak + stimulation interval.

```C++
// Process the raw signal in real time.
while(1){
    double sample = getraw(); // Stream the raw data 
    double sampleOut; // The output sample
    double stimInterval = xxx; // The stimulation interval
    
    if(cleanObj.recieve(sample, &sampleOut, stimInverval)){
        // The recieve funtion will returen true if there is output.
        // Add your code here.
        ...
    }
}
```

When the stimulation state is unknown, the timing of the upcoming artifact was predicted by the timing of last peak + the last peak interval.

```C++
// Process the raw signal in real time.
while(1){
    double sample = getraw(); // Stream the raw data 
    double sampleOut; // The output sample
    
    if(cleanObj.recieve(sample, &sampleOut)){
        // The recieve funtion will returen true if there is output.
        // Add your code here.
        ...
    }
}
```

## Demo project 

The demo project can be found in the demo folder, this project was created with Qt 5.

The demo program can load raw signal from a .txt file and process it one sample by one sample to simulate the real-time processing.

- cleandbs.pro: the project file of Qt
- main.cpp: the main file
- cleandbs.h & cleandbs.cpp: source file of CleanDBS class
- nsplfile.h & nsplfile.cpp: read and write .txt files
- progressbar.h & progressbar.cpp: show progress bar

You can compile the project in Qt, and run it in the terminal.

```shell
$ cleandbs [fileIn][fileOut][option][value]

Options:                                                            
    -f, output sampling rate, default 300Hz                         
    -a, skip 'a' second ahead of each artifact peak, default 0.0002 
    -p, skip 'p' second after each artifact peak, default 0.004     
    -t, threshold for artifact peak detection, default 3000         
```

Note: 

This raw data should be saved in a plain text file (.txt), each column is one channel.

The header information should be stored in the first line of the text file.

For example:
```
44000    2    2200000    0
```

- 44000, the sampling rate of the raw signal
- 2, the number of channels
- 2200000, the number of points
- 0, not used parameter

**Run with test data**

The test data can be found in `/Demo/testdata/` folder.

Copy the test data file to your build folder and run:

```
$ cleandbs input.txt output.txt -f 300 -a 0.0002 -p 0.004 -t 1000
```

Replace the `input.txt` & `output.txt` to the data file name accordingly.