#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>


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
    char pl, pr;//something wrong
    file.seekg(lhs).get(pl);
    file.seekg(rhs).get(pr);

    file.seekp(lhs).put(pr);
    file.seekp(rhs).put(pl);
}

void heapify(std::fstream& file, int n, int i)
{

    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n)
    {
        char cl, clargest;
        file.seekg(l).get(cl);
        file.seekg(largest).get(clargest);
           
        if (cl > clargest)
            largest = l;
    }

    if (r < n)
    {
        char ccl, cclargest;
        file.seekg(r).get(ccl);
        file.seekg(largest).get(cclargest);
        // If right child is larger than largest so far
        if (ccl > cclargest)
            largest = r;
    }

    // If largest is not root
    if (largest != i) {
        if (i > n || largest > n)
        {
            printf("Warning: ut of bounds!");
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

void readAndWrite(std::fstream& file, int n)
{
    file.seekg(0, std::ios::beg);

    for(int i = 0; i < n; i++)
    {
        char c;
        file.get(c);//must use get because >> skips \n
        if (file.fail())
            assert(false);
        printf("%d ", c);
    }
}

int main()
{
    const char* filename = "../Inputs/Input7.bin";

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

    readAndWrite(file, fileSize);
    std::cout << "\nSorted ->\n";
    file.seekg(0, std::ios::beg);
    heapSort(file, fileSize);
    readAndWrite(file, fileSize);
  //  print(input);
    return 0;
}