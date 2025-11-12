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

void Show(vector<disk_block>& file_layout, vector<disk_block>& space_layout)
{
    size_t len{ 42 };
    //for (auto& i : file_layout) len += i.size_;
    //for (auto& i : space_layout) len += i.size_;
    string out(len, '.');
    for (auto& i : file_layout) 
        for (int j = i.addr_; j < i.addr_+i.size_; ++j) out[j] = (char)(i.file_id_ + '0');
    
    cout << out<<endl;
}

uint64_t solve2(string &line)
{
    uint64_t crc{ 0 };
    vector<disk_block> file_layout;
    vector<disk_block> space_layout;
    int file_id{ 0 };
    int addr{ 0 };
    for (int i = 0; i < line.length(); i += 2, ++file_id) {
        int file_size = line[i] - '0';
        file_layout.emplace_back(addr, file_size,file_id);
		addr += file_size;
        int free_size = line[i + 1] - '0';
        space_layout.emplace_back(addr, free_size, file_id);
        addr += free_size;
    }
    //Show(file_layout, space_layout);
    for (int fid = file_layout.size() - 1; fid; --fid) {
        for (int sps{ 0 }; sps < fid; ++sps)
            if (space_layout[sps].size_ >= file_layout[fid].size_) {
                file_layout[fid].addr_ = space_layout[sps].addr_;
                space_layout[sps].addr_ += file_layout[fid].size_;
                space_layout[sps].size_ -= file_layout[fid].size_;
                break;
        }
//        Show(file_layout, space_layout);
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


