#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
#include <chrono>

size_t comparisons = 0;
size_t swaps = 0;

struct Node
{
    int adress;
    char value;
    int pointer;
};

void readNode(std::fstream& file, Node& node)
{
    //today I learned that sizeof(Node) != 9, because there's padding
    char c[sizeof(int) + sizeof(char) + sizeof(int)];
    char* buffer = c;
    file.read(buffer, (sizeof(int) + sizeof(char) + sizeof(int)));
    unsigned char* ubuff = (unsigned char*)c;
    //works for big-endian or little-endian architectures
    node.adress = ubuff[0] | (ubuff[1] << 8) | (ubuff[2] << 16) | (ubuff[3] << 24);
    ubuff += sizeof(int);
    node.value = (char)*ubuff;
    ubuff += sizeof(char);
    node.pointer = ubuff[0] | (ubuff[1] << 8) | (ubuff[2] << 16) | (ubuff[3] << 24);
}

void readAndWriteFullDebug(std::fstream& file, int n)
{
    file.seekg(0, std::ios::beg);

    for (int i = 0; i < n; i++)
    {
        char c;
        file.get(c);//must use get because >> skips \n
        if (file.fail())
            assert(false);
        printf("%d ", c);
    }
}


void readAndWriteDebug(std::fstream& file, int n)
{
    file.seekg(0, std::ios::beg);
    Node node;

    for (int i = 0; i < n; i += sizeof(int) + sizeof(char) + sizeof(int))
    {
        readNode(file, node);
        if (file.fail())
            assert(false);
        printf("%i %d %d\n", node.adress, node.value, node.pointer);
    }
}

void setSeekg(std::fstream& file, int index)
{
    file.seekg(0);
    if (index == 0)
        return;

    Node node;
    int i = 0;
    while (true)
    {
        readNode(file, node);
        if (file.fail())
            assert(false);
        if (i == index)
        {//seekg was already offset forward because we read, need to go back by 9
            int pos = file.tellg();
            pos -= 9;
            file.seekg(pos);
            return;
        }
        file.seekg((node.pointer - 1) * (sizeof(int) + sizeof(char) + sizeof(int)));
        i++;
    }
}

void setSeekp(std::fstream& file, int index)
{
    if (index == 0)
    {
        file.seekp(4);
        return;
    }

    Node node;
    int i = 0;
    file.seekg(0);
    while (true)
    {
        readNode(file, node);
        if (file.fail())
            assert(false);
        if (i == index)
        {//seekg was already offset forward because we read, need to go back by 9
         //and because we want to write the value we need to go forward by 4
            int pos = file.tellp();
            pos -= 5;
            file.seekp(pos);
            return;
        }
        file.seekg((node.pointer - 1) * (sizeof(int) + sizeof(char) + sizeof(int)));
        i++;
    }
}

void print(std::vector<char>* vec)
{
    for (auto& i : *vec)
    {
        printf("%d ", i);
    }
    printf("\n");
}

void swap(std::fstream& file, int lhs, int rhs)
{
    swaps++;
    Node pl, pr;
    setSeekg(file, lhs);
    readNode(file, pl);
    setSeekg(file, rhs);
    readNode(file, pr);

    setSeekp(file, lhs);
    file.put(pr.value);
    setSeekp(file, rhs);
    file.put(pl.value);
}

void heapify(std::fstream& file, int n, int i)
{

    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
    int sizeo = sizeof(int) + sizeof(char) + sizeof(int);

    // If left child is larger than root
    if (l < n)
    {
        Node cl, clargest;
        setSeekg(file, l);
        readNode(file, cl);

        setSeekg(file, largest);
        readNode(file, clargest);

        if (cl.value > clargest.value)
            largest = l;
        comparisons++;
    }

    if (r < n)
    {
        Node ccl, cclargest;
        setSeekg(file, r);
        readNode(file, ccl);

        setSeekg(file, largest);
        readNode(file, cclargest);
        // If right child is larger than largest so far
        if (ccl.value > cclargest.value)
            largest = r;
        comparisons++;
    }

    // If largest is not root
    if (largest != i) {
        if (i > n || largest > n)
        {
            printf("Warning: out of bounds!");
        }
        swap(file, i, largest);

        // Recursively heapify the affected sub-tree
        heapify(file, n, largest);
    }
}

// main function to do heap sort
void heapSort(std::fstream& file, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(file, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        swap(file, 0, i);

        // call max heapify on the reduced heap
        heapify(file, i, 0);
    }
}

int readAndWrite(std::fstream& file, int n)
{
    file.seekg(0, std::ios::beg);

    Node node;
    int count = 0;
    node.pointer = -1;

    while (true)
    {
        readNode(file, node);
        if (file.fail())
            assert(false);
        printf("%d ", node.value);
        if (node.pointer == 0)
            return ++count;
        file.seekg((node.pointer - 1) * 9);
        count++;
    }
    return count;
}

int main()
{
    const char* filename = "../Inputs/InputList5.bin";

    // open the file:
    std::fstream file;

    std::streampos fileSize;

    //interesting find, must set more flags when opening a binary file with fstream
    file.open(filename, std::ios::in | std::ios::binary | std::ios::out);

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (!file.is_open() || file.fail())
    {
        std::cout << "Couldnt open file " << filename << "\n";
    }

    //When linked list is imitated in binary memory we must know it's memory adress. I'm going to assume it's
    //always going to be the first element with adress 0x01 
    //we als must know how many elements it contains
    //readAndWriteDebug(file, fileSize);
    int elements = readAndWrite(file, fileSize);
    std::cout << "\nSorted ->\n";
    file.seekg(0, std::ios::beg);

    auto start = std::chrono::high_resolution_clock::now();

    heapSort(file,elements);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    readAndWrite(file, fileSize);
    std::cout << "\nIt took me " << time_span.count() << " seconds.";
    std::cout << "\nswaps " << swaps;
    std::cout << "\ncomparisons " << comparisons;
    std::cout << "\nelements " << elements;
    //  print(input);
    return 0;
}