#include "nsplfile.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

using namespace std;

int NSPLFile::readnsplhdr(QString filename, int *nChan, int *nPoint, int *fs){
    // Open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {
        cout << "Cannot open file." << endl;
        return -1;
    }

    QTextStream txtStream(&file);
    QString lineStr;

    // Read header
    lineStr = txtStream.readLine();
    QStringList spl = lineStr.split('\t', Qt::SkipEmptyParts);
    *fs = spl.at(0).toInt();
    *nChan = spl.at(1).toInt();
    *nPoint = spl.at(2).toInt();

    file.close();

    return 0;
}

int NSPLFile::readnspldata(QString filename, double **pData, int m, int n){
    // Reader header
    int nChan=0, nPoint=0, fs=0;
    NSPLFile::readnsplhdr(filename, &nChan, &nPoint, &fs);

    if(m<nChan||n<nPoint){
        cout << "Allocated space not enough." << endl;
        return -1;
    }

    // Open file
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly  | QIODevice::Text))
    {
        cout << "Cannot open file." << endl;
        return -1;
    }

    QTextStream txtStream(&file);
    QString lineStr;

    // Skip the header line
    lineStr = txtStream.readLine();

    cout << "Loading data ... ";

    // Read data
    for(int i=0; i<nChan; i++){
        pData[i] = new double[nPoint];
    }

    QStringList spl;
    for(int i=0; i<nPoint;i++){
        lineStr = txtStream.readLine();
        spl = lineStr.split('\t', Qt::SkipEmptyParts);
        for(int j=0; j<nChan; j++){
            pData[j][i] = spl.at(j).toDouble();
        }
    }

    cout << "Finished." << endl;

    return 0;
}

int NSPLFile::savenspldata(QString filename, double **pData, int nChan, int nPoint, int fs){
    if(nChan<1||nPoint<1){
        return -1;
    }

    // Open file
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        cout << "Cannot open file." << endl;
        return -1;
    }

    QTextStream txtStream(&file);

    cout << "Saving data ... ";

    // Write header
    txtStream << fs << "\t" << nChan << "\t" << nPoint << "\t" << 0 << Qt::endl;

    // Write data
    for(int i=0; i<nPoint;i++){
        for(int j=0; j<nChan; j++){
            txtStream << pData[j][i] << "\t";
        }
        txtStream << Qt::endl;
    }

    file.close();

    cout << "Finished." << endl;

    return 0;
}
