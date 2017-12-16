#include "params.h"
#include "SysTools.h"
#include "MersenneTwister.h"

using namespace std;

Params::Params():
    _meshKDtree(3)
{
	VolBlockSize(32);

	ensStatNumOutliers(4);
	ensStatNumNeighbors(8);

	_randGen = new MTRand();

	currentRun(0); // should set to the mean run
	
	string folder = "C:/MyData/Utah_heart_ischemia/201701_Conductivity/simRuns/dataset_test";
	runFileFolder(folder);

	string meshFile = "C:\\MyData\\Utah_heart_ischemia\\201701_Conductivity\\mesh\\heartPts.csv";
	meshFileName(meshFile);
	// Get number of runs 
	vector<string> folderContent = SysTools::GetDirContents(folder, "*", "txt");
	vector<string> runFileList;
	for (size_t i = 0; i < folderContent.size(); i++)
	{
		if (folderContent[i].find("run") != std::string::npos)
			runFileList.push_back(folderContent[i]);
	}
	setRunFileList(runFileList);
	numRuns(int(runFileList.size()));
}

Params::~Params()
{
    _meshKDtree.clear();
	SAFE_DELETE(_randGen);
}

const std::vector<std::vector<float> >& Params::pointData() const
{
    return _pointData;
}

void Params::setPointData(const std::vector<std::vector<float> > &pointData)
{
    _pointData = pointData;
}

const std::vector<Vertex>& Params::vertices() const
{
    return _vertices;
}

void Params::setVertices(const std::vector<Vertex> &vertices)
{
    _vertices = vertices;
}

const std::vector<std::vector<float> >& Params::ensembleData() const
{
    return _ensembleData;
}

void Params::setEnsembleData(const std::vector<std::vector<float> > &val)
{
    _ensembleData = val;
}

