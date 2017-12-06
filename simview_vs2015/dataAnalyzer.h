#pragma once
#include "KDtree.h"
#include "spatialDataPt.h"
#include "volume\VolumeData.h"
//#include "RegularGrid.h"

class octree;
class EnsembleVolBlock;
class VolBlockAnalyzer;
class DataAnalyzer
{
public:
	DataAnalyzer();
	~DataAnalyzer();

	void buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree);
	//void buildRegGrid(const std::vector<FLOATVECTOR3>& mesh, RegularGrid<vector<UINT64>>& grid);
	// convert the ensemble volumes to a list of ensemble vol blocks
	void buildEnsembleVolBlockList(const std::vector<VolumeData>& ensembleVols, std::vector<EnsembleVolBlock*>& ebList, UINT64VECTOR3& eblistDim);

	// generate a lookup table that converts mesh point id to 3D coordinates in a volume with dimensions of "gridDim"
	// volPtCnt -- a volume recording number of points in each cell
	void convertMeshIdToGridPos(std::vector<UINT64VECTOR3>& volCoord_lookupTbl, VolumeData** volPtCnt, const std::vector<FLOATVECTOR3>& mesh, UINT64VECTOR3 gridDim);
	// convert ensemble point data to volumes
	void convertEnsembleRunsToVol(std::vector<VolumeData*>& vols, const std::vector<std::vector<float> >& ensembles,
		const UINT64VECTOR3& volDim, const std::vector<UINT64VECTOR3>& posLookupTbl, const VolumeData& volPtCnt);
	// Analyze ensembles
	// Analyze using KD-tree (not helpful though...)
	void analyzeEnsembleRuns(const KD<spatialDataPt*>& kdtree, vector<vector<float> >& ensemble);
	// Analyze with neighboring node similarity(?)
	//void analyzeEnsembleNodes(std::vector<EnsembleVolBlock*> ensVolBlockList, const UINT64VECTOR3& listDim);
	void createEnsembleOctree(std::vector<VolumeData*>& ensembleVols, octree* ensembleOctTree);
	

private:
	void kdtree_ensemble_analysis(BinNode<spatialDataPt*>* kdnode, vector<vector<float>>& ensemble); // recursive function

	VolBlockAnalyzer*         _volBlkAnalyzer;
};

