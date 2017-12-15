#include "FuncBoxPlot.h"

//function contg = MBD(obj)
//% calculates the generalized band depth of a set of data
//n = size(obj.y, 1); % size of the data matrix
//cont = zeros(n, 1, obj.outdims(3));
//for p = 1:obj.outdims(3)
//for i = 1 : (n - 1)
//for j = (i + 1) : (n) % consider all possible pairs of functions
//cont(:, : , p) = cont(:, : , p) + obj.a(obj.y(:, : , p), [i j]);
//end
//end
//end
//contg = cont / obj.combinat(n, 2);
//end
//
//function contg = BD3(obj)
//% calculate the band depth with J = 3.
//n = size(obj.y, 1);
//cont = zeros(n, 1, obj.outdims(3));
//% Select three observations from the sample in all the possible ways.
//for p = 1:obj.outdims(3)
//for i = 1 : (n - 2)
//for j = (i + 1) : (n - 1)
//for k = (j + 1) : n
//cont(:, : , p) = cont(:, : , p) + obj.estaEntre(obj.y(:, : , p), [i j k])';   % In this subfunction we check which observations from the sample is inside the band delimeted by observations i,j and k.           
//
//end
//end
//end
//end
//contg = cont / obj.combinat(n, 3);
//end
//
//function contg = BD2(obj)
//% calculate the band depth of every observation in the matrix
//n = size(obj.y, 1);
//cont = zeros(n, 1, obj.outdims(3));
//for p = 1:obj.outdims(3)
//for i = 1 : ((n + 2) / 3)
//for j = (i + 1) : (n) % choose pairs of indexes in all the possible ways.
//cont(:, : , p) = cont(:, : , p) + obj.estaEntre(obj.y(:, : , p), [i j])';
//end
//end
//end
//contg = cont / obj.combinat(n, 2);
//end
//
//end

FuncBoxPlot::FuncBoxPlot()
{
}


FuncBoxPlot::~FuncBoxPlot()
{
}

int FuncBoxPlot::bandDepth(const funcEnsemble & ensemble)
{
	return 0;
}

float FuncBoxPlot::MBD(const funcEnsemble & ensemble)
{
	return 0.0f;
}

float FuncBoxPlot::BD3(const funcEnsemble & ensemble)
{
	return 0.0f;
}


float FuncBoxPlot::BD2(const funcEnsemble & ensemble)
{
	return 0.0f;
}


