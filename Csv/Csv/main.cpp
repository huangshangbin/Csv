#include <iostream>
#include <vector>

using namespace std;

#include <csv/dependent/csv2.hpp>
using namespace csv2;

void readTest()
{
	csv2::Reader<trim_policy::trim_whitespace> csv;

	csv.firstRowIsHeader(false);

	string filePath = "test.csv";
	if (csv.mmap("test.csv")) {
		const auto header = csv.header();
		for (const auto row : csv) {
			for (const auto cell : row) {
				// Do something with cell value
				std::string value;
				cell.read_value(value);
				cout << value << endl;
			}
		}
	}
}

void writeTest()
{
	char ch = ',';
	std::ofstream stream("test.csv");
	Writer writer(stream);

	writer.setQuoteCharacter('[');

	std::vector<std::vector<std::string>> rows =
	{
		{ "a", "b", "c" },
		{ "1", "2", "3" },
		{ "4", "5", "6" }
	};

	writer.write_rows(rows);
}

int main()
{
	writeTest();

	int a;
	cin >> a;

	return 0;
}