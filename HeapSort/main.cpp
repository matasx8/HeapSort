#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

size_t comparisons = 0;
size_t swaps = 0;

void print(std::vector<char>* vec)
{
    for (auto& i : *vec)
    {
        printf("%d ", i);
    }
    printf("\n");
}

void heapify(std::vector<char>& vec, int n, int i)
{

    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && vec[l] > vec[largest])
    {
        largest = l;
    }
    comparisons++;

    // If right child is larger than largest so far
    if (r < n && vec[r] >vec[largest])
    {
        largest = r;
    }
    comparisons++;

    // If largest is not root
    if (largest != i) {
        std::swap(vec[i], vec[largest]);
        swaps++;

        // Recursively heapify the affected sub-tree
        heapify(vec, n, largest);
    }
}

// main function to do heap sort
void heapSort(std::vector<char>& arr, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) 
    {
        // Move current root to end
        std::swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

void read(std::vector<char>* data, const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    data->reserve(fileSize);

    // read the data:
    data->insert(data->begin(),
        std::istream_iterator<char>(file),
        std::istream_iterator<char>());
    file.close();
}

int main()
{
    std::vector<char>* input = new std::vector<char>();
    read(input, "../Inputs/Input5.bin");

    auto start = std::chrono::high_resolution_clock::now();

    heapSort(*input, input->size());

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
   // print(input);
    std::cout << "\nIt took me " << time_span.count() << " seconds.";
    std::cout << "\nswaps " << swaps;
    std::cout << "\ncomparisons " << comparisons << " seconds.";
	return 0;
}