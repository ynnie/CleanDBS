#ifndef NSPLFILE_H
#define NSPLFILE_H
#include <QString>

class NSPLFile
{
public:
    static int readnsplhdr(QString filename, int *nChan, int *nPoint, int *fs);
    static int readnspldata(QString filename, double **pData, int m, int n);
    static int savenspldata(QString filename, double **pData, int nChan, int nPoint, int fs);
};

#endif // NSPLFILE_H
