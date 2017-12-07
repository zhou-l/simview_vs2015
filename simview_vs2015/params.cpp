#include "params.h"
#include "MersenneTwister.h"


Params::Params():
    _meshKDtree(3)
{
	VolBlockSize(32);

	ensStatNumOutliers(4);
	ensStatNumNeighbors(8);

	_randGen = new MTRand();
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

