#ifndef OCTREE_H
#define OCTREE_H
#include "MyVectors.h"
#include "octreenode.h"
#include <fstream>
#include "Eigen/Core"
class VolBlockAnalyzer;
class VolumeData;
class MassSpringSystem;
enum OCT_TYPE
{
	OCT_ENSEMBLE = 0,
	OCT_SINGLE
};

class octree
{
public:
    octree(OCT_TYPE type);
	~octree();

	// top level functions
    bool build(int levels);
	void fillInEnsembleData(std::vector<VolumeData*>& ensVols, int blockSize);
	void analyzeEnsembleData(const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3& ebListDim, VolBlockAnalyzer* analyzer);
	void setupMassSpringSys(MassSpringSystem** ppMassSpring);

	void beginOutputContent(std::string& filename);
	void endOutputContent();
	// breadth first assignment of node info to a list of matrices
	void bfsSetLevelNodesInfo(std::vector<Eigen::MatrixXf>& M);

	int  levels() const { return _tree_levels; }
	
	friend UINT64 volPos2SID(FLOATVECTOR3 fVolPos, int level); // convert floating point 3D volume pos to uint64 serial id

protected:
	// recursive functions
	void octreeBuild(octreeNode* node);
	void octreeDestroy(octreeNode* node);
	void octreeFillEnsembleData(octreeNode* node, std::vector<VolumeData*>& ensVols, int blockSize);
	void octreeAnalyzeEnsData(octreeNode* node, const std::vector<EnsembleVolBlock*>& ebList,
		const UINT64VECTOR3& ebListDim, VolBlockAnalyzer* analyzer);
	void octreeSetMassSpringSys(octreeNode* node, MassSpringSystem** ppMassSpring);

	void octreeWriteContent(octreeNode* node, std::ofstream& file);
protected:
    octreeNode* _root;     // the root node

	int      _tree_levels; // number of tree levels
	OCT_TYPE _type;        // single data or ensembles?

	bool     _isWrite; // shall we write out content?
	ofstream _ofs; // file writer
};



#endif // OCTREE_H
