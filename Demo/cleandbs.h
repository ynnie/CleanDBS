#ifndef CLEANDBS_H
#define CLEANDBS_H

class CleanDBS
{
public:
    CleanDBS(double fsOrg, double fsOut, double preArtifact, double postArtifact, double thr);
    ~CleanDBS();

    bool recieve(double sampleIn, double *sampleOut);
    bool recieve(double sampleIn, double *sampleOut, double stimInterval);

private:
    double interpolate(double sample1, double time1, double sample2, double time2, double timeOut);
    bool isArtifact(double sample);
    bool isArtifact(double sample, double stimInterval);

    void bufferAppend(double sample);
    double getBufferedSample(int i);

    double _fsOrg;
    double _fsOut;
    double _deltaOrg;
    double _deltaOut;

    double _thr;

    double _preArtifact;
    double _postArtifact;

    double _tLastPeak;
    double _lastPeakInterval;

    double _tCurrent;
    double _lastSample;
    double _tLastSample;

    double _time;

    int _nBuffer;
    double *_pBuffer;
};

#endif // CLEANDBS_H
