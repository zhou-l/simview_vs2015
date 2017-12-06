#include "DataAnalyzer.h"
#include "volume\volblockanalyzer.h"
#include "volume\octree.h"
#include "global.h"

DataAnalyzer::DataAnalyzer()
{
	_volBlkAnalyzer = new VolBlockAnalyzer();
}


DataAnalyzer::~DataAnalyzer()
{
	SAFE_DELETE(_volBlkAnalyzer);
}

void DataAnalyzer::buildKDtree(const std::vector<FLOATVECTOR3>& mesh, KD<spatialDataPt*>& kdtree)
{
	kdtree = KD<spatialDataPt*>(3);
	vector<spatialDataPt*> dataMesh(mesh.size(), 0);
	for (vector<spatialDataPt*>::iterator IT = dataMesh.begin(); IT != dataMesh.end(); ++IT)
	{
		UINT64 id = IT - dataMesh.begin();
		*IT = new spatialDataPt(mesh[id], id);
	}
	kdtree.buildBalanceTree(dataMesh);
}

//void DataAnalyzer::buildRegGrid(const std::vector<FLOATVECTOR3>& mesh, RegularGrid<vector<UINT64>>& grid)
//{
//	// The regular grid stores only the id in the mesh vector
//	UINT64VECTOR3 gridDim = grid.dim();
//	// subdivide the mesh to build a grid.
//	FLOATVECTOR3 meshMin(FLT_MAX, FLT_MAX, FLT_MAX);
//	FLOATVECTOR3 meshMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
//	for (vector<FLOATVECTOR3>::const_iterator IT = mesh.begin(); IT != mesh.end(); ++IT) {
//		meshMin.x = MIN(IT->x, meshMin.x);
//		meshMin.y = MIN(IT->y, meshMin.y);
//		meshMin.z = MIN(IT->z, meshMin.z);
//
//		meshMax.x = MAX(IT->x, meshMax.x);
//		meshMax.y = MAX(IT->y, meshMax.y);
//		meshMax.z = MAX(IT->z, meshMax.z);
//	}
//	FLOATVECTOR3 meshSize = meshMax - meshMin;
//	cout << "Mesh size = " << meshSize << endl;
//	FLOATVECTOR3 cellSize = FLOATVECTOR3(meshSize.x / float(gridDim.x), meshSize.y/float(gridDim.y), meshSize.z/float(gridDim.z));
//	cout << " Cell size = " << cellSize << endl;
//	// traverse the grid and fill particles
//	for (vector<FLOATVECTOR3>::const_iterator IT = mesh.begin(); IT != mesh.end(); ++IT)
//	{
//		UINT64 pid = IT - mesh.begin();
//		FLOATVECTOR3 pos_offset_norm = (*IT - meshMin)/meshSize;
//		UINT64VECTOR3 voxInd = UINT64VECTOR3(pos_offset_norm.x * (cellSize.x - 1), 
//			pos_offset_norm.y * (cellSize.y - 1), 
//			pos_offset_norm.z * (cellSize.z - 1));
//		vector<UINT64> voxContent = grid.element(voxInd);
//		voxContent.push_back(pid);
//		grid.setElement(voxInd, voxContent);
//	}
//}

void DataAnalyzer::buildEnsembleVolBlockList(const std::vector<VolumeData>& ensembleVols, std::vector<EnsembleVolBlock*>& ebList, UINT64VECTOR3 & eblistDim)
{
}

void DataAnalyzer::convertMeshIdToGridPos(std::vector<UINT64VECTOR3>& volCoord_lookupTbl, VolumeData** volPtCnt, const std::vector<FLOATVECTOR3>& mesh, UINT64VECTOR3 gridDim)
{
	cout << "Generating mesh id to vol pos table...";
	if (*volPtCnt != NULL)
		SAFE_DELETE(*volPtCnt);
	(*volPtCnt) = new VolumeData(gridDim, 4, 0, true, true); // create a float volume
	// subdivide the mesh to build a grid.
	FLOATVECTOR3 meshMin(FLT_MAX, FLT_MAX, FLT_MAX);
	FLOATVECTOR3 meshMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (vector<FLOATVECTOR3>::const_iterator IT = mesh.begin(); IT != mesh.end(); ++IT) {
		meshMin.x = MIN(IT->x, meshMin.x);
		meshMin.y = MIN(IT->y, meshMin.y);
		meshMin.z = MIN(IT->z, meshMin.z);

		meshMax.x = MAX(IT->x, meshMax.x);
		meshMax.y = MAX(IT->y, meshMax.y);
		meshMax.z = MAX(IT->z, meshMax.z);
	}
	FLOATVECTOR3 meshSize = meshMax - meshMin;
	cout << "Mesh size = " << meshSize << endl;
	FLOATVECTOR3 cellSize = FLOATVECTOR3(meshSize.x / float(gridDim.x), meshSize.y / float(gridDim.y), meshSize.z / float(gridDim.z));
	cout << " Cell size = " << cellSize << endl;
	// traverse the grid and fill particles
	volCoord_lookupTbl.resize(mesh.size());
	for (vector<FLOATVECTOR3>::const_iterator IT = mesh.begin(); IT != mesh.end(); ++IT)
	{
		UINT64 pid = IT - mesh.begin();
		FLOATVECTOR3 pos_offset_norm = (*IT - meshMin) / meshSize;
		UINT64VECTOR3 voxInd = UINT64VECTOR3(pos_offset_norm.x * (gridDim.x - 1),
			pos_offset_norm.y * (gridDim.y - 1),
			pos_offset_norm.z * (gridDim.z - 1));
		volCoord_lookupTbl[pid] = voxInd;
		// set point count volume
		float cnt = (*volPtCnt)->getVoxel(voxInd);
		cnt++;
		(*volPtCnt)->setVoxel(voxInd, cnt);
	}

	bool verifyResults = true;
	if (verifyResults)
	{
		// print out the look up table
		string fname = "volposTable_" + number2String(gridDim.x) + "_" + number2String(gridDim.y) + "_" + number2String(gridDim.z) + ".txt";
		ofstream ofPosTbl(fname.c_str());
		for (size_t i = 0; i < volCoord_lookupTbl.size(); i++)
			ofPosTbl << volCoord_lookupTbl[i] << endl;
		ofPosTbl.close();


		float colCnt = 0;
		float totalCnt = 0;
		for (UINT64 z = 0; z < gridDim.z; z++)
		{
			for (UINT64 y = 0; y < gridDim.y; y++)
			{
				for (UINT64 x = 0; x < gridDim.x; x++)
				{
					float val = (*volPtCnt)->getVoxel(UINT64VECTOR3(x, y, z));
					if (val > 1)
						colCnt += val - 1;
					totalCnt += val;
				}
			}
		}
		cout << endl<<"Total number of data points ="<<totalCnt<<", with collisions = " << colCnt << endl;
	}
	cout << "finished!" << endl;;
}

void DataAnalyzer::convertEnsembleRunsToVol(std::vector<VolumeData*>& vols, const std::vector<std::vector<float>>& ensembles, const UINT64VECTOR3 & volDim, const std::vector<UINT64VECTOR3>& posLookupTbl, const VolumeData& volPtCnt)
{
	cout << "Converting ensemble runs to volumes..";
	// release vols
	for (size_t i = 0; i < vols.size(); i++)
		SAFE_DELETE(vols[i]);
	
	// Generate volumes for the list
	vols.resize(ensembles.size());
	for (size_t i = 0; i < vols.size(); i++)
	{
		float* fData = new float[volDim.volume()];
		// clear memory
		memset(fData, 0, sizeof(float) * volDim.volume());
		vols[i] = new VolumeData(volDim, 4, fData, true, true);
	}

	// fill in data

	for (size_t item = 0; item < ensembles[0].size(); item++)
	{

		UINT64VECTOR3 pos = posLookupTbl[item];
		float oneOverNumPtsAtPos = 1.0f/ volPtCnt.getVoxel(pos); // cannot be zero!
		//UINT64 iPos = (pos.z * volDim.y + pos.y) * volDim.x + pos.x;
		for (size_t r = 0; r < ensembles.size(); r++)
		{
			float val = ensembles[r][item];
			float prevAvg = vols[r]->getVoxel(pos);
			// online average
			float newAvg = prevAvg + (val - prevAvg)*oneOverNumPtsAtPos;
			vols[r]->setVoxel(pos, newAvg);
		}
	}
	cout << "done!" << endl;
}

void DataAnalyzer::analyzeEnsembleRuns(const KD<spatialDataPt*>& kdtree, vector<vector<float>>& ensemble)
{
	BSTNode<int, spatialDataPt*>* root = dynamic_cast<BSTNode<int,spatialDataPt*>*>(kdtree.rootNode());
	BinNode<spatialDataPt*>* node = root;
	// Traverse the tree
	while (node != NULL)
	{
		kdtree_ensemble_analysis(node->left(), ensemble);
		kdtree_ensemble_analysis(node->right(), ensemble);
	}

}

void DataAnalyzer::createEnsembleOctree(std::vector<VolumeData*>& ensembleVols, octree* ensembleOctTree)
{
	if (ensembleVols.empty())
	{
		cout << "No ensemble volumes! Octree not created!" << endl;
		return;
	}
	if (ensembleOctTree)
		SAFE_DELETE(ensembleOctTree);
	ensembleOctTree = new octree(OCT_ENSEMBLE);
	// determine how many levels the octree has
	UINT64VECTOR3 volDim = ensembleVols[0]->getDim();
	float numBlocks = volDim.maxVal() / g_params.VolBlockSize();
	// get nearest power of 2
	UINT64VECTOR3 paddedVolDim;
	int tree_levels = int(ceil(log(numBlocks) / log(2.0f)) + 1);
	int paddedNumBlocks = int(pow(2, ceil(log(numBlocks) / log(2.0f))));
	int blkSize = g_params.VolBlockSize();
	if (numBlocks == float(paddedNumBlocks))
		paddedVolDim = volDim;
	else
		paddedVolDim = UINT64VECTOR3(paddedNumBlocks * blkSize, 
			paddedNumBlocks * blkSize, paddedNumBlocks * blkSize);


	// pad volume
	for(size_t i = 0; i < ensembleVols.size(); i++)
		ensembleVols[i]->pad(paddedVolDim);
	
	// Build a skeleton octree
	ensembleOctTree->build(tree_levels);
	// Fill in data
	ensembleOctTree->fillInEnsembleData(ensembleVols, blkSize);

	bool dumpNodeInfo = true;
	if (dumpNodeInfo)
		ensembleOctTree->beginOutputContent(std::string("ensembleOctree.txt"));
	// Analyze the volume!
	ensembleOctTree->analyzeEnsembleData(g_params.ensVolBlocks(), g_params.ensVolBlkListDim(), _volBlkAnalyzer);
	if (dumpNodeInfo)
		ensembleOctTree->endOutputContent();
}

//void DataAnalyzer::analyzeEnsembleNodes(std::vector<EnsembleVolBlock*> ensVolBlockList, const UINT64VECTOR3 & listDim)
//{
//	// analyze the list of ensemble blocks
//	for (UINT64 bz = 0; bz < listDim.z; bz++)
//	{
//		for (UINT64 by = 0; by < listDim.y; by++)
//		{
//			for (UINT64 bx = 0; bx < listDim.x; bx++)
//			{
//				UINT64 blkId = (bz * listDim.y + by) * listDim.x + bx;
//
//			}
//		}
//	}
//}

void DataAnalyzer::kdtree_ensemble_analysis(BinNode<spatialDataPt*>* kdnode, vector<vector<float>>& ensemble)
{
	if (!kdnode->isLeaf())
	{
		kdtree_ensemble_analysis(kdnode->left(), ensemble);
		kdtree_ensemble_analysis(kdnode->right(), ensemble);
	}
	else
	{
		if (kdnode == NULL)
			return; 
		// compute ensemble statistics

	}
}
