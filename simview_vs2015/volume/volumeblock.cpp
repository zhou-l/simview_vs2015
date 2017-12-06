#include "volumeblock.h"
#include "VolumeData.h"

VolumeBlock::VolumeBlock():
	_dim(0,0,0),
	_data(NULL)
{}

VolumeBlock::VolumeBlock(const VolumeData* vol, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim):
    _dim(blockDim)
{
   _data = new float[blockDim.volume()];
   UINT64VECTOR3 volDim = vol->getDim();
   for(UINT64 z = blockStart.z; z < blockStart.z + blockDim.z; z++)
   {
       for(UINT64 y = blockStart.y; y < blockStart.y + blockDim.y; y++)
       {
           for(UINT64 x = blockStart.x; x < blockStart.x + blockDim.x; x++)
           {
               UINT64 blockInd = ((z - blockStart.z) * blockDim.y + (y - blockStart.y)) * blockDim.x + (x - blockStart.x);
               float val = vol->getVoxel(UINT64VECTOR3(x,y,z));

               _data[blockInd] = val;
           }
       }
   }
}

EnsembleVolBlock::EnsembleVolBlock():VolumeBlock()
{}

EnsembleVolBlock::EnsembleVolBlock(const std::vector<VolumeData*>& ensembles, UINT64VECTOR3 blockStart, UINT64VECTOR3 blockDim):
	VolumeBlock()
{
	_dim = blockDim;
	_num_ensembles = int(ensembles.size());
	_data = new float[blockDim.volume() * _num_ensembles];

	// set data
	for (int r = 0; r < _num_ensembles; r++)
	{
		
		for (UINT64 z = blockStart.z; z < blockStart.z + blockDim.z; z++)
		{
			for (UINT64 y = blockStart.y; y < blockStart.y + blockDim.y; y++)
			{
				for (UINT64 x = blockStart.x; x < blockStart.x + blockDim.x; x++)
				{
					UINT64 blockInd = ((z - blockStart.z) * blockDim.y + (y - blockStart.y)) * blockDim.x + (x - blockStart.x);

					float val = ensembles[r]->getVoxel(UINT64VECTOR3(x, y, z));
					_data[blockInd * _num_ensembles + r] = val;

				}
			}
		}

	}

}
