#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <sstream>
#include <fstream>

void ReadFile(std::string file_name, RBTree* tree);

int main(int argc, char* argv[]) {
	RBTree tree;
	ReadFile(argv[1], &tree);
	/*tree.insert("a");
	tree.insert("b");
	tree.insert("c");
	tree.insert("d");
	tree.insert("e");
	tree.inorder();*/
	std::cout << tree.height(tree.top()) << std::endl;
    
}

void ReadFile(std::string file_name, RBTree* tree){
    // Create the input filestream - opens the file & prepares it for reading
    std::ifstream file(file_name);

    // Temporary string to hold a single line of the file
    std::string str;

	std::string const delimiters = " ,'\"-?!%&*^()@_;:./\\";

    // Reads all lines in the file, 1 at at time
    while (std::getline(file, str)) {

    size_t beg, pos = 0;
    while ((beg = str.find_first_not_of(delimiters, pos)) != std::string::npos)
    {
        str[pos] = tolower(str[pos]);
        pos = str.find_first_of(delimiters, beg + 1);
        (*tree).insert(str.substr(beg, pos - beg));
        tree->top()->red = false; 
    }
      /* // Converts our string into a stringstream
      std::istringstream ss(str);
       // Temp double to store a converted value from a line
      std::string token;

        // Reads all values from the stringstream (current row), converts to double
        while(getline(ss, token)){
			for (int i = 0; i < token; i++){
				
			}
            // Adds the converted value to the row
            (*tree).insert(token);
        }
    }*/
	}
}