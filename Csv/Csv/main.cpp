#include <iostream>
#include <vector>

using namespace std;

#include <csv/CsvFile.hpp>




int main()
{
	CsvFile testFile;

	testFile.firstRowIsHeader(false);

	testFile.loadFile("test.csv");
	

	for (int i = 0; i < testFile.getRowSize(); i++)
	{
		for (int col = 0; col < testFile.getColSize(); col++)
		{
			cout << testFile.getValue(i, col) << ",";
		}

		cout << endl;
	}

	testFile.setValue(0, 0, "1");
	testFile.setValue(0, 1, "2");
	testFile.setValue(0, 2, "3");

	testFile.saveNewFile("copy.csv");

	int a;
	cin >> a;

	return 0;
}