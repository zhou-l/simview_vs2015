#pragma once
class Boxplot
{
public:
	Boxplot();
	~Boxplot();

protected:
	float _mu;
	float _std;
	float _median;
	float _outliers;
};

