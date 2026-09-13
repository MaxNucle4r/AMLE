#pragma once
#include <string>
#include <vector>





struct MemoryEntry
{
    std::string input;
    std::string output;
};



class Memory
{

private:
    std::vector<MemoryEntry> entries;


public:

    void add(const std::string& input,
        const std::string& output);


    const std::vector<MemoryEntry>& getAll() const;
    std::vector<MemoryEntry>& getAll();


    std::vector<MemoryEntry>
        findByWord(const std::string& word) const;


    std::vector<MemoryEntry>
        findByMaxInputWords(size_t maxWords) const;


    std::vector<MemoryEntry> getRange(size_t from, size_t to) const;

    std::vector<MemoryEntry> getFirst(size_t amount) const;

    std::vector<MemoryEntry> getLast(size_t amount) const;



    void append(const Memory& other);



    void clear();


    size_t size() const;



    Memory& operator+= (const Memory& other);
};