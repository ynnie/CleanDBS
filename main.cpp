#include <iostream>
#include <QString>
#include <QFile>
#include <QTextStream>

#include "nsplfile.h"
#include "progressbar.h"
#include "cleandbs.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Input and output filename
    QString filenameIn("test.txt");
    QString filenameOut("test-out.txt");

    // Default parameters
    double fsOut = 300; // Output sampling rate
    double preArtifact = 0.0002;
    double postArtifact = 0.005;
    double thr = 3000;

    // Read header info
    int nChan;
    int nPoint;
    int fs;

    int retn = NSPLFile::readnsplhdr(filenameIn, &nChan, &nPoint, &fs);
    if(retn!=0){
        cout << "Can not open file !" << endl;
        return -1;
    }

    // Allocate space for loading data
    double **pData = new double*[nChan];
    for(int i=0; i<nChan; i++)
        pData[i] = new double[nPoint];

    // Read data
    if(NSPLFile::readnspldata(filenameIn, pData, nChan, nPoint)!=0){
        return -1;
    }

    // Open file for saving output data
    QFile file(filenameOut);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        cout << "Cannot create output file." << endl;
        return -1;
    }

    QTextStream txtStream(&file);

    txtStream << fsOut << "\t" << 1 << "\t" << 0 << "\t" << 0 << Qt::endl; // Write header


    // Init CleanDBS
    CleanDBS clean(fs, fsOut, preArtifact, postArtifact, thr);

    ProgressBar *pb = new ProgressBar(nPoint, "Processing");
    pb->SetFrequencyUpdate(fs/2);

    // Main loop
    for(int i=0; i<nPoint; i++){
        double sample = pData[0][i];

        double sampleOut = 0;

        if(clean.recieve(sample, &sampleOut)){
            txtStream << sampleOut << Qt::endl;
        }

        pb->Progressed(i+1);
    }

    // Clean up
    file.close();

    for(int i=0; i<nChan; i++){
        delete [] pData[i];
    }

    delete [] pData;

    cout << endl;
    cout << "Finished!" << endl;

    return 0;
}
