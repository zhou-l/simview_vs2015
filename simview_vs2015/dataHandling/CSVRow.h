#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Read a row in a CSV file and seperate the line into tokens!
class CSVRow
{
public:
	CSVRow()
	{
		m_delimiter = ',';
	}

	CSVRow(char delimiter) :
		m_delimiter(delimiter)
	{}


	std::string const& operator[](std::size_t index) const
	{
		return m_data[index];
	}
	std::size_t size() const
	{
		return m_data.size();
	}
	void readNextRow(std::istream& str)
	{
		std::string         line;
		std::getline(str, line);

		std::stringstream   lineStream(line);
		std::string         cell;

		m_data.clear();
		while (std::getline(lineStream, cell, m_delimiter))
		{
			m_data.push_back(cell);
		}
	}

	friend std::istream& operator>>(std::istream& str, CSVRow& data)
	{
		data.readNextRow(str);
		return str;
	}
private:
	std::vector<std::string>    m_data;
	char                        m_delimiter;
};

