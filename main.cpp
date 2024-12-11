#include "BlockChain.h"
#include "Utilities.h"
#include <iostream>

using std::cerr;
using std::string;
using std::cout;

#define OPERATION 1
#define SOURCE 2
#define TARGET 3
#define EXPECTED_NUM_ARGS 4

int main(int argc, char** argv) {
    if (argc != EXPECTED_NUM_ARGS) {
        cerr << getErrorMessage() << argv[0];
    }
    string op = argc[OPERATION];
    string source = argc[SOURCE];
    string target = argc[TARGET];

    ifstream source_file(source);

    if (op == "format"){ //operation 1
        ofstream target_file(target);
        BlockChain blockChain = BlockChainLoad(source_file);
        BlockChainDump(blockChain, target_file);
    } else if (op == "hash"){ //operation 2
        ofstream target_file(target);
        BlockChain blockChain = BlockChainLoad(source_file);
        BlockChainDumpHashed(blockChain, target_file);
    } else if (op == "compress"){ //operation 3
        ofstream target_file(target);
        BlockChain blockChain = BlockChainLoad(source_file);
        BlockChainCompress(blockChain);
        BlockChainDump(blockChain, target_file);
    } else if (op == "verify"){ //operation 4
        cout << "Verification ";
        ifstream target_file(target);
        BlockChain blockChain = BlockChainLoad(source_file);
        if(BlockChainVerifyFile(blockChain, target_file)){
            cout << "passed" << std::endl;
        } else {
            cout << "failed" << std::endl;
        }
    } else { //oi vei
        std::cerr << getErrorMessage() << std::endl;
        return 0;
    }

}