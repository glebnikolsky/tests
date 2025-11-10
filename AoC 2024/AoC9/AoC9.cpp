// AoC9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;

uint64_t solve1(string &line) 
{
    uint64_t crc{ 0 };
    vector<int> disk_layout;
    int file_id{ 0 };
    for (int i = 0; i < line.length(); i+=2,++file_id) {
        int file = line[i] - '0';
        int free = line[i+1] - '0';
        while (file--)disk_layout.push_back(file_id);
        while (free--)disk_layout.push_back(-1);
    }
    //for (auto& i : disk_layout) cout << (char)(i == -1 ? '.' : '0' + i);
    //cout << endl;
    for (size_t b = 0, e = disk_layout.size()-1; b < e; ) {
        while (disk_layout[b] != -1) ++b;
        while (disk_layout[e] == -1) --e;
        if (e <= b) break;
        disk_layout[b] = disk_layout[e];
        disk_layout[e] = -1;
    }
    //for (auto& i : disk_layout) cout << (char)(i == -1 ? '.' : '0' + i);
    //cout << endl;
    for (int i = 0; i < disk_layout.size() && disk_layout[i] != -1; ++i) crc += i * disk_layout[i];
    return crc;
}

struct disk_block {
    int addr_;
    int size_;
    int file_id_;
    disk_block(int adr, int sz, int fid) : addr_{ adr }, size_{ sz }, file_id_{ fid } {}
    disk_block( int adr, int sz) : addr_{ adr }, size_{ sz }, file_id_{ -1 } {}
    bool operator <(const disk_block& rgt) const {
        return make_tuple(size_,addr_)< make_tuple(rgt.size_, rgt.addr_);
	}
    uint64_t crc() {
        uint64_t crc{ 0 };
        for (; size_; ++addr_, --size_) crc += file_id_ * addr_;
        return crc;
    }
    template <class Ostream> friend Ostream& operator << (Ostream& os, const disk_block& b)
    {
        os << '(' << (b.file_id_ == -1 ? "free" : to_string(b.file_id_)) << ',' << b.addr_ << ',' << b.size_ << ')';
        return os;
    }

};

uint64_t solve2(string &line)
{
    uint64_t crc{ 0 };
    vector<disk_block> file_layout;
    int file_id{ 0 };
    int addr{ 0 };
    for (int i = 0; i < line.length(); i += 2, ++file_id) {
        int file_size = line[i] - '0';
        file_layout.emplace_back(addr, file_size,file_id);
		addr += file_size;
        int free_size = line[i + 1] - '0';
//        if (free_size) {
            file_layout.emplace_back(addr, free_size);
            addr += free_size;
//        }
    }
    {
        ofstream ofs("res.txt");
        for (auto& i : file_layout) ofs << i << endl;
    }
    ofstream ofs("proc.txt");
    for (int fid = file_layout.size() - 2; fid; fid -= 2) {
		if (file_layout[fid].file_id_ == -1) continue;
        int free_space{ 0 };
        for (int i = 1; i < fid; i+=2) {
            if (file_layout[i].size_ < file_layout[fid].size_) continue;
            free_space = i;
            break;
        }
        if (!free_space) continue;
        ofs << file_layout[fid] << "=>" << file_layout[free_space] << endl;
        file_layout[fid].addr_ = file_layout[free_space].addr_;
        file_layout[free_space].addr_ += file_layout[fid].size_;
        file_layout[free_space].size_ -= file_layout[fid].size_;
    }
    for (auto& i : file_layout)
        crc += i.crc();
    return crc;
}

int main()
{
    ifstream ifs("AoC9.txt");
    string line;
    getline(ifs, line);
    if (line.length() % 2) line += '0';
    cout << solve1(line) << endl;
    cout << solve2(line) << endl;
}


