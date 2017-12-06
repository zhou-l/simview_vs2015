#ifndef DATALOADER_H
#define DATALOADER_H

#include <fstream>
#include <vector>
#include <QString>
class Volume;
class DataLoader
{
public:
    DataLoader();
	virtual ~DataLoader();

    bool loadCSVtoPointCloud(const QString& fileName);
	bool loadEnsembleRunsTxt(const QString& folderName);

	void cleanData();

    std::vector<std::vector<float> > pointData() const;
    void setPointData(const std::vector<std::vector<float> > &pointData);

	std::vector<std::vector<float> > ensembleData() const;

    std::vector<std::string> attrib_names() const;
    void setAttrib_names(const std::vector<std::string> &attrib_names);

private:
    std::ifstream _ifData;
    std::vector<std::vector<float> >  _pointData;
	std::vector<std::vector<float> >  _ensembleData;
    // attribute information
    std::vector<std::string>   _attrib_names;


};

#endif // DATALOADER_H
