#ifndef PARAMS_H
#define PARAMS_H
#include "vertex.h"
#include "KDtree.h"
#include "spatialDataPt.h"
#include <string>

class VolumeData;
class EnsembleVolBlock;
class octree;
class MTRand;
class Params
{
public:
	Params();
	~Params();

	const std::vector<std::vector<float> >& pointData() const;
	void setPointData(const std::vector<std::vector<float> > &pointData);

	const std::vector<Vertex>& vertices() const;
	void setVertices(const std::vector<Vertex> &vertices);

	KD<spatialDataPt*>& meshKDTree_unsafe() { return _meshKDtree; }
	void meshKDTree(const KD<spatialDataPt*>& val) { _meshKDtree = val; }

	const std::vector<std::vector<float> >& ensembleData() const;
	void setEnsembleData(const std::vector<std::vector<float> > &val);

	std::vector<VolumeData*>& ensembleVols_unsafe() { return _ensembleVols; }
	const std::vector<VolumeData*>& ensembleVols() const { return _ensembleVols; }
	void setEnsembleVols(const std::vector<VolumeData*>& val) { _ensembleVols = val; }

	const int VolBlockSize() const { return _volBlockSize; }
	void VolBlockSize(int volBkSize) { _volBlockSize = MAX(volBkSize, 2); } // min block size is 2*2*2!

	const int ensStatNumOutliers() const { return _ensStatNumOutliers; }
	void ensStatNumOutliers(int val) { _ensStatNumOutliers = val; }


	const int ensStatNumNeighbors() const { return _ensStatNumNeighbors; }
	void ensStatNumNeighbors(int val) { _ensStatNumNeighbors = val; }

	const UINT64VECTOR3 ensVolBlkListDim() const { return _ensembleVolBlkListDim; }
	void ensVolBlkListDim(UINT64VECTOR3 val) { _ensembleVolBlkListDim = val; }

	const std::vector<EnsembleVolBlock*> ensVolBlocks() const { return _ensembleVolBlocks; }
	void ensVolBlocks(const std::vector<EnsembleVolBlock*>& val) { _ensembleVolBlocks = val; }

	void ensembleOctree(octree* tree) { _ensembleOctree = tree; }
	octree* ensembleOctree() { return _ensembleOctree; }

	MTRand* randGen() const { return _randGen; }

	void setOctreeLevel(int level) { _currOctreeLevel = level; }
	int octreeLevel() const { return _currOctreeLevel; }

	void currentRun(int run) { _currentRun = run; }
	int currentRun() const { return _currentRun; }

	void runFileFolder(const std::string& ff) { _runFileFolder = ff; }
	const std::string& runFileFolder() const { return _runFileFolder; }

	void meshFileName(const std::string& meshFile) { _meshFileName = meshFile; }
	const std::string& meshFileName() const { return _meshFileName; }

	void numRuns(int numRuns) { _numRuns = numRuns; }
	int numRuns() const { return _numRuns; }

	const std::vector< std::string >& runFileList() const { return _runFileList; }
	void setRunFileList(const std::vector<std::string>& fileList) { _runFileList = fileList; }
private:
    std::vector<std::vector<float>> _pointData;
    std::vector<std::vector<float>> _ensembleData;
    std::vector<VolumeData*>        _ensembleVols; // converted from the ensemble point data
	std::vector<EnsembleVolBlock*>  _ensembleVolBlocks; // a list of all ensemble blocks
	UINT64VECTOR3                   _ensembleVolBlkListDim; // number of ensemble blocks in each dimension in the list
	octree*                         _ensembleOctree; // the octree of ensemble blocks
	int                             _currOctreeLevel; // current octree level

    std::vector<Vertex>             _vertices;
    KD<spatialDataPt*>              _meshKDtree;

    int                             _volBlockSize; // the size of a basic volume block for analysis
    // params for per-node statistics
    int                             _ensStatNumOutliers; // number of outliers 
    int                             _ensStatNumNeighbors;

	MTRand*                          _randGen;
	//info about the ensemble data
	int                              _currentRun; // the current run id
	std::string                      _runFileFolder; // folder for ensemble files
	std::vector< std::string >       _runFileList; // list of run files
	int                              _numRuns; // number of runs in the folder
	std::string                      _meshFileName; // file name of the mesh 
	
};

#endif // PARAMS_H
