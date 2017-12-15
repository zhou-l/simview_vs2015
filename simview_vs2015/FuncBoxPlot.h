#pragma once
#include <vector>

typedef std::vector< std::vector<float> > funcEnsemble;

class FuncBoxPlot
{
public:
	FuncBoxPlot();
	virtual ~FuncBoxPlot();
private:
	int bandDepth(const funcEnsemble& ensemble);
	float MBD(const funcEnsemble& ensemble);
	float BD3(const funcEnsemble& ensemble);
	float BD2(const funcEnsemble& ensemble);
};

