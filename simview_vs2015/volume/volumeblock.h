#ifndef VOLUMEBLOCK_H
#define VOLUMEBLOCK_H

#include "MyVectors.h"

class VolumeData;
class VolumeBlock
{
public:
    VolumeBlock();
    VolumeBlock(const VolumeData* vol, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim);
    float* Data() const {return _data;}
    UINT64VECTOR3 Dim() const { return _dim;}
protected:
    float*         _data;		   // data array
    UINT64VECTOR3  _dim;           // spatial dimension of the block
};

class EnsembleVolBlock : public VolumeBlock
{
public:
	EnsembleVolBlock();
    EnsembleVolBlock(const std::vector<VolumeData*>& ensembles, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim);

    int numEnsembles() const { return _num_ensembles; }

    // The data layout is:

    // each row stores all runs
    // items are stored sequentially, i.e., an n*r array
private:
    int              _num_ensembles;
};
#endif // VOLUMEBLOCK_H
