#include <algorithm>
#include <sstream>

#include "Memory.h"





void Memory::add(const std::string& input,
    const std::string& output)
{
    entries.push_back({ input, output });
}



const std::vector<MemoryEntry>& Memory::getAll() const
{
    return entries;
}


std::vector<MemoryEntry>& Memory::getAll()
{
    return entries;
}



std::vector<MemoryEntry> Memory::findByWord(const std::string& word) const
{
    std::vector<MemoryEntry> found;

    for (auto it = entries.rbegin(); it != entries.rend(); ++it)
    {
        if (it->input.find(word) != std::string::npos ||
            it->output.find(word) != std::string::npos)
        {
            found.push_back(*it);
        }
    }

    return found;
}



std::vector <MemoryEntry> Memory::findByMaxInputWords(size_t maxWords) const
{
    std::vector<MemoryEntry> found;

    for (const auto& e : entries)
    {
        std::stringstream ss(e.input);

        size_t count = 0;

        std::string word;

        while (ss >> word) count++;

        if (count <= maxWords)
            found.push_back(e);

    }

    return found;

}



void Memory::clear()
{
    entries.clear();
}


size_t Memory::size() const
{
    return entries.size();
}



std::vector<MemoryEntry> Memory::getRange(size_t from, size_t to) const
{
    std::vector<MemoryEntry> range;

    for (size_t i = from; i < to; ++i)
    {
        range.push_back(entries[i]);
    }

    return range;

}


std::vector<MemoryEntry> Memory::getFirst(size_t amount) const
{
    std::vector<MemoryEntry> range;

    for (size_t i = 0; i < amount; ++i)
    {
        range.push_back(entries[i]);
    }

    return range;
}


std::vector<MemoryEntry> Memory::getLast(size_t amount) const
{
    std::vector<MemoryEntry> range;

    size_t start = entries.size() - amount;

    for (size_t i = start; i < entries.size(); ++i)
    {
        range.push_back(entries[i]);
    }

    return range;
}



void Memory::append(const Memory& other)
{
    // TO DO
    // make a workaround for cases like memory += memory

    entries.insert(
        entries.end(),
        other.getAll().begin(),
        other.getAll().end()
    );
}





Memory& Memory::operator+= (const Memory& other)
{
    append(other);

    return *this;
}

