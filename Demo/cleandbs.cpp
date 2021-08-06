#include "cleandbs.h"
#include <math.h>

CleanDBS::CleanDBS(double fsOrg, double fsOut, double preArtifact, double postArtifact, double thr)
{
    // Init parameters
    _fsOrg = fsOrg;
    _fsOut = fsOut;

    _deltaOrg = 1/fsOrg;
    _deltaOut = 1/fsOut;

    _preArtifact = preArtifact;
    _postArtifact = postArtifact;
    _thr = thr;

    _tLastPeak = -1;

    _tCurrent = 0;

    _lastSample = 0;
    _tLastSample = 0;

    _time = 0;

    _nBuffer = ceil(_preArtifact*_fsOrg);
    _pBuffer = new double[_nBuffer];

}

CleanDBS::~CleanDBS(){
    delete [] _pBuffer;
}

bool CleanDBS::recieve(double sampleIn, double *sampleOut){
    _tCurrent += _deltaOrg;
    _time += _deltaOrg;

    bool artifact = isArtifact(sampleIn);


    if(!artifact){
        if(_tCurrent>=_deltaOut){
            // Time to resample
            *sampleOut = interpolate(_lastSample, _tLastSample, sampleIn, _tCurrent, _deltaOut);

            _tCurrent = _tCurrent-_deltaOut;
            _tLastPeak = _tLastPeak-_deltaOut;
            _tLastSample = -_deltaOut;
            _lastSample = *sampleOut;


            return true;
        }else {
            // Continue buffering
            _tLastSample = _tCurrent;
            _lastSample = sampleIn;
            bufferAppend(sampleIn);

            return false;
        }

    }
    return false;
}

bool CleanDBS::recieve(double sampleIn, double *sampleOut, double stimInterval){
    _tCurrent += _deltaOrg;
    _time += _deltaOrg;

    bool artifact = isArtifact(sampleIn, stimInterval);

    if(!artifact){
        if(_tCurrent>=_deltaOut){
            // Time to resample
            *sampleOut = interpolate(_lastSample, _tLastSample, sampleIn, _tCurrent, _deltaOut);

            _tCurrent = _tCurrent-_deltaOut;
            _tLastPeak = _tLastPeak-_deltaOut;
            _tLastSample = -_deltaOut;
            _lastSample = *sampleOut;

            return true;
        }else {
            // Continue buffering
            _tLastSample = _tCurrent;
            _lastSample = sampleIn;
            bufferAppend(sampleIn);

            return false;
        }

    }
    return false;
}

double CleanDBS::interpolate(double sample1, double time1, double sample2, double time2, double timeOut){
    double sampleOut = sample1 + (sample2-sample1)/(time2-time1)*(timeOut-time1);
    return sampleOut;
}

bool CleanDBS::isArtifact(double sample){
    double deltaLastPeak = _tCurrent-_tLastPeak;

    if((deltaLastPeak>=0.001)&&fabs(sample)>_thr){
        _lastPeakInterval = _tCurrent-_tLastPeak;

        _tLastPeak = _tCurrent;
        deltaLastPeak = _tCurrent-_tLastPeak;

        _lastSample = getBufferedSample(0);
        _tLastSample = _tLastSample-_preArtifact;
    }

    bool result = (deltaLastPeak<_postArtifact)||((deltaLastPeak>(_lastPeakInterval-_preArtifact)&&(deltaLastPeak<_lastPeakInterval+_postArtifact)));

    return result;
}

bool CleanDBS::isArtifact(double sample, double stimInterval){
    double deltaLastPeak = _tCurrent-_tLastPeak;

    bool result;

    if(stimInterval==0){
        // Stimulation OFF
        result = false;
    }else{
        // Stimulation ON
        if((deltaLastPeak>=0.001)&&fabs(sample)>_thr){
            _lastPeakInterval = stimInterval;

            _tLastPeak = _tCurrent;
            deltaLastPeak = _tCurrent-_tLastPeak;

            _lastSample = getBufferedSample(0);
            _tLastSample = _tLastSample-_preArtifact;
        }

        result = (deltaLastPeak<_postArtifact)||((deltaLastPeak>(stimInterval-_preArtifact)&&(deltaLastPeak<stimInterval+_postArtifact)));
    }

    return result;
}

void CleanDBS::bufferAppend(double sample){
    for(int i=0; i<_nBuffer-1; i++){
        _pBuffer[i] = _pBuffer[i+1];
    }

    _pBuffer[_nBuffer-1] = sample;
}

double CleanDBS::getBufferedSample(int i){
    if(i<0){
        return _pBuffer[_nBuffer+i];
    }else {
        return _pBuffer[i];
    }
}
