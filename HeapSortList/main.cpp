//Heap Sort, char, linked list

#include <iostream>
#include <vector>
#include <list>
#include <fstream>

struct node
{
    char val;
    node* parent;
    node* left;
    node* right;
};


void print(std::list<char>* vec)
{
    for (auto& i : *vec)
    {
        printf("%d ", i);
    }
    printf("\n");
}

void heapify(std::list<char>& lst, int n, int i)
{

    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && *std::next(lst.begin(), l) > *std::next(lst.begin(), largest))
    {
        largest = l;
    }

    // If right child is larger than largest so far
    if (r < n && *std::next(lst.begin(), r) > *std::next(lst.begin(), largest))
    {
        largest = r;
    }

    // If largest is not root
    if (largest != i) {
        std::swap(*std::next(lst.begin(), i), *std::next(lst.begin(), largest));
        // Recursively heapify the affected sub-tree
        heapify(lst, n, largest);
    }
}

// main function to do heap sort
void heapSort(std::list<char>& arr, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--)
    {
        // Move current root to end
        std::swap(*arr.begin(), *std::next(arr.begin(), i));

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

node* fillTree(node* root, std::vector<char>& data, int i, int n)
{
    if (i < n)
    {
        node* temp = new node{ data[i], root, nullptr, nullptr };
        root = temp;

        //insert left child
        root->left = fillTree(root->left, data, 2 * i + 1, n);

        //insert right child
        root->right = fillTree(root->right, data, 2 * i + 2, n);
    }
    return root;
}

void read(std::list<char>* data, const char* filename)
{

    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    data->insert(data->begin(),
        std::istream_iterator<char>(file),
        std::istream_iterator<char>());

    file.close();
}

int main()
{
    std::list<char>* input = new std::list<char>();
    read(input, "../Inputs/Input4.bin");
    heapSort(*input, input->size());
    print(input);
    return 0;
}