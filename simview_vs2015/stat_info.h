#pragma once
// the statistics information of a node

#include <vector>
#include "global.h"

class StatInfo
{
public:
	StatInfo():
		_mu(0.0f),
		_var(0.0f),
		_median(0.0f),
		_min(0.0f),
		_max(0.0f)
	{
		_outliers.resize(g_params.ensStatNumOutliers(), 0.0f);
		_statDists.resize(g_params.ensStatNumNeighbors());
	}

	~StatInfo() {}

	friend std::ostream& operator << (std::ostream& os, const StatInfo& v)
	{
		os << v._mu << endl
			<< v._var << endl
			<< v._median << endl
			<< v._min << endl << v._max << endl;// << _statDists;
		return os;
	}

public:
	float _mu;
	float _var;
	float _median;
	float _min;
	float _max;

	std::vector<float> _outliers;
	// statistical differences to the node's neighbors
	// The difference to each neighor is a list of float 
	std::vector<std::vector<float> > _statDists;
};