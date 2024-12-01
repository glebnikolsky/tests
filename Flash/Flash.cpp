#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
const size_t cChunk{ 4096 };
const long long cTestSize{ 1024 * 1024 * 128};

map<string, unsigned long> crc;

bool Check(long long n)
{
	double total{ 0.0 };
	for (int i = 1; i <= n; i++) {
		vector<unsigned char> test(cChunk,i);
		string name = "e:" + to_string(i) + ".txt";
		vector<unsigned char> buff(cChunk);
		ifstream ifs;
		ifs.open(name.c_str(), ios_base::binary);
		const auto start = std::chrono::high_resolution_clock::now();
		for (int k = 0; k < cTestSize && !ifs.eof(); k += cChunk) {
			ifs.read((char*)&buff[0], cChunk);
			if (buff != test) {
				cout << i << " error!!!\n";
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char **argv)
{
	long long vol;
	if (argc == 1) vol = 128 * 1024 * 1024;
	try {
		for (int n = 1; vol / cTestSize; n++)
		{
			cout << "\rwrite... " << n<< '\n';
			vector<unsigned char> v(cChunk, n);
			string name = "e:" + to_string(n) + ".txt";
			ofstream ofs;
			ofs.open(name.c_str(), ios_base::binary);
			for (size_t i = 0; !ofs.bad() && i < cTestSize; i += cChunk) {
				ofs.write((const char*)&v[0], cChunk);
				if (ofs.bad()) {
					cout << "\t io error " << n << '\n';
					break;
				}
			}
			ofs.close();
			if (!Check(n)) break;
		}
	}
	catch (ios_base::failure f)
	{
		cout << "Caught an exception: " << f.what() << endl;
	}
}

