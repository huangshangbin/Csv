#pragma once

#include "dependent/csv2.hpp"

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;


class CsvFile
{
private:
	string m_filePath;
	csv2::Reader<csv2::trim_policy::trim_whitespace> m_csvReader;

	vector<string> m_headerList;
	map<string, string> m_indexValueMap;// "1,2" = "huang"
	unsigned long m_rowSize;
	unsigned long m_colSize;

public:
	CsvFile() 
	{
		m_rowSize = -1;
		m_colSize = 0;
	}
	~CsvFile() {}

public:
	void setDelimiter(char delemiter) { m_csvReader.setDelimiter(delemiter); }
	void setQuoteCharacter(char quoteCharacter) { m_csvReader.setQuoteCharacter(quoteCharacter); }
	void firstRowIsHeader(bool value)
	{
		m_csvReader.firstRowIsHeader(value); 
		m_rowSize = value ? -1 : 0;
	}

public:
	void loadFile(string filePath)
	{
		m_headerList.clear();
		m_filePath = filePath;

		if (m_csvReader.mmap(filePath)) {
			const auto header = m_csvReader.header();
			for (const auto headerNode : header) {
				std::string value;
				headerNode.read_value(value);

				m_headerList.push_back(value);
			}

			m_rowSize = m_rowSize + m_csvReader.rows();//m_csvReader.rows include header row
			m_colSize = m_csvReader.cols();

			int rowIndex = 0;
			int colIndex = 0;
			for (const auto row : m_csvReader) {
				for (const auto cell : row) {
					std::string value;
					cell.read_value(value);
					
					m_indexValueMap[std::to_string(rowIndex) + "," + std::to_string(colIndex)] = value;

					colIndex++;
				}

				rowIndex++;
				colIndex = 0;
			}
		}
	}

public:
	void setHeader(vector<string> headerList)
	{
		m_headerList = std::move(headerList);
	}

	void setValue(int row, int col, string value)
	{
		if (col >= m_colSize)
		{
			m_colSize = col + 1;
		}
		if (row >= m_rowSize)
		{
			m_rowSize = row + 1;
		}

		m_indexValueMap[std::to_string(row) + "," + std::to_string(col)] = value;
	}

	string getValue(int row, int col)
	{
		return m_indexValueMap[std::to_string(row) + "," + std::to_string(col)];
	}

public:
	unsigned long getRowSize()
	{
		return m_rowSize;
	}

	unsigned long getColSize()
	{
		return m_colSize;
	}


public:
	void saveFile()
	{
		saveNewFile(m_filePath);
	}

	void saveNewFile(string filePath)
	{
		std::ofstream stream(filePath);
		csv2::Writer writer(stream);
		writer.setDelimiter(m_csvReader.getDelimiter());
		writer.setQuoteCharacter(m_csvReader.getQuoteCharacter());

		vector<vector<string>> fileData;
		if (m_csvReader.getFirstRowIsHeader())
		{
			fileData.push_back(m_headerList);
		}

		for (int rowIndex = 0; rowIndex < m_rowSize; rowIndex++)
		{
			vector<string> rowData;
			for (int colIndex = 0; colIndex < m_colSize; colIndex++)
			{
				rowData.push_back(getValue(rowIndex, colIndex));
			}

			fileData.push_back(rowData);
		}

		writer.write_rows(fileData);
	}
};

