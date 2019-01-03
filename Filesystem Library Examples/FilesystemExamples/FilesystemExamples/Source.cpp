#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std::experimental::filesystem;
using std::cout;
using std::endl;
using std::error_code;

uintmax_t ComputeFileSize(const path& pathToCheck)
{
	if (exists(pathToCheck) &&
		is_regular_file(pathToCheck))
	{
		auto err = error_code{};
		auto filesize = file_size(pathToCheck, err);
		if (filesize != static_cast<uintmax_t>(-1))
			return filesize;
	}

	return static_cast<uintmax_t>(-1);
}

void DisplayFileInfo(const v1::directory_entry & entry, std::string &lead, v1::path &filename)
{
	time_t cftime = std::chrono::system_clock::to_time_t(last_write_time(entry));
	cout << lead << " " << filename << ", "
		<< ComputeFileSize(entry);
}

void DisplayDirectoryTree(const path& pathToShow, int level)
{
	if (exists(pathToShow) && is_directory(pathToShow))
	{
		auto lead = std::string(level * 3, ' ');
		for (const auto& entry : directory_iterator(pathToShow))
		{
			auto filename = entry.path().filename();
			if (is_directory(entry.status()))
			{
				cout << lead << "[+] " << filename << "\n";
				DisplayDirectoryTree(entry, level + 1);
				cout << "\n";
			}
			else if (is_regular_file(entry.status()))
			{
				DisplayFileInfo(entry, lead, filename);	
			}
			else
			{
				cout << lead << " [?]" << filename << "\n";
			}
		}
	}
}

int main(int argc, char* argv[])
{
	path pathToShow("C:\\Users\\terzi\\Desktop");
	cout << "exists() = " << exists(pathToShow) << "\n"
		<< "root_name() = " << pathToShow.root_name() << "\n"
		<< "root_path() = " << pathToShow.root_path() << "\n"
		<< "relative_path() = " << pathToShow.relative_path() << "\n"
		<< "parent_path() = " << pathToShow.parent_path() << "\n"
		<< "filename() = " << pathToShow.filename() << "\n"
		<< "stem() = " << pathToShow.stem() << "\n"
		<< "extension() = " << pathToShow.extension() << "\n";
	
	size_t j = 0;
	for (auto& i : pathToShow)
	{
		cout << "Path part " << j++ << ": " << i << endl;
	}

	uintmax_t fileSize = ComputeFileSize("C:\\data\\Protocol1.txt");
	cout << "The size .txt is " << fileSize  << " bytes" << endl;
			
	
	return 0;
}