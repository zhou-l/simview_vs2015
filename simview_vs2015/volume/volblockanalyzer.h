#ifndef VOLBLOCKANALYZER_H
#define VOLBLOCKANALYZER_H

#include "MyVectors.h"
#include "volumeblock.h"
#include "stat_info.h"

class VolBlockAnalyzer
{
public:
    VolBlockAnalyzer();

    float blockCorr(const VolumeBlock* b1, const VolumeBlock* b2);
    void  InblockAnalysis(const VolumeBlock* b, StatInfo& stats);

	// analyze blocks of ensembles
	void ensemble_inBlockAnalysis(const EnsembleVolBlock* eb, StatInfo& propagated_block_stats);
	// compute propagated block stats from a list of stats
	void ensemble_propStats(const std::vector<StatInfo>& childrenStats, StatInfo& propagated_block_stats);
	void ensemble_neighborBlocksAnalysis(const std::vector<EnsembleVolBlock*>& ebList, const UINT64VECTOR3& ebListDim, FLOATVECTOR3 blockPos, StatInfo& blockStats);

	// Compute a set of KL differences???
	std::vector<float> ensemble_betweenBlockAnalysis(const EnsembleVolBlock* eb1, const EnsembleVolBlock* eb2);
private:


    UINT64VECTOR3 blockSize;
};

#endif // VOLBLOCKANALYZER_H
