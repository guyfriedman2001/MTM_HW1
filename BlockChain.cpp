
#pragma once

#include <string>
#include <fstream>
#include "BlockChain.h"


using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::endl;


typedef unsigned int (*updateFunction)(unsigned int);



int BlockGetSize(const Block& block);
int BlockPersonalBalance(const Block& block, const string& name);
void BlockDump(const Block& block, ofstream& file);
void BlockDump(const Block& block, ofstream& file, int num);
void BlockDumpHashed(const Block& block, ofstream& file);
bool BlockVerifyFile(const Block& block, std::ifstream& file);
void BlockCompress(Block& block);
void BlockTransform(Block& block, updateFunction function);
void DestroyBlock(Block& block);
void DestroyBlocks(Block& block);



/**
 * BlockChainGetSize - returns the number of Blocks in the BlockChain
 *
 * @param blockChain - BlockChain to measure
 *
 * @return Number of Blocks in the BlockChain
*/
int BlockChainGetSize(const BlockChain& blockChain){
        Block* head = blockChain.block;
        if(head == nullptr){
                return 0;
        }
        return BlockGetSize(*head);
}


/**
 * BlockChainPersonalBalance - returns the balance of a given person, relative to a given BlockChain
 *
 * @param blockChain - BlockChain to calculate the balance from
 * @param name - Name of the person to calculate the balance for
 *
 * @return Balance of the person
*/
int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name){
        Block* head = blockChain.block;
        if(head == nullptr){
                return 0;
        }
        return BlockPersonalBalance(*head, name);
}


/**
 * BlockChainAppendTransaction - creates and appends a new transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param value Value of the transaction
 * @param sender Name of the sender
 * @param receiver Name of the receiver
 * @param timestamp String that holds the time the transaction was made
*/
void BlockChainAppendTransaction(BlockChain& blockChain, unsigned int value, const string& sender,
        const string& receiver, const string& timestamp){
        Transaction trans{value,sender,receiver};
        BlockChainAppendTransaction(blockChain, trans, timestamp);
}


/**
 * BlockChainAppendTransaction - appends a copy of a given transaction to the BlockChain
 *
 * @param blockChain BlockChain to append the transaction to
 * @param transaction Transaction we want to append
 * @param timestamp String that holds the time the transaction was made
*/
void BlockChainAppendTransaction(BlockChain& blockChain, const Transaction& transaction, const string& timestamp){
        Block* newBlock = new Block(transaction, timestamp, blockChain.block);
        blockChain.block = newBlock;

}



/**
 * BlockChainLoad - Reads data from a file and creates a new block chain
 *
 * @param file Data file to read from
 *
 * @return BlockChain created from the file
 *
*/
BlockChain BlockChainLoad(ifstream& file){
        string line;
        BlockChain returnItem = BlockChain();
        string sender = "";
        string reciever = "";
        unsigned int ammount = 0;
        string time = "";
        while((!file.eof())&&(file >> sender >> reciever >> ammount >> time)){
                BlockChainAppendTransaction(returnItem, ammount, sender,
                        reciever, time);
        }
        return returnItem;
}


/**
 * BlockChainDump - Prints the data of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 *
 * BlockChain info:
 * <n>.
 * Sender Name: <name>
 * Receiver Name: <name>
 * Transaction Value: <value>
 * Transaction Timestamp: <time>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDump(const BlockChain& blockChain, ofstream& file){
        file << "BlockChain info:";
        file << endl;
        Block* block = blockChain.block;
        if (block != nullptr){
                BlockDump(*block, file);
        }
}



/**
 * BlockChainDumpHashed - Prints the *hashed data* of all transactions in the BlockChain to a given file
 *
 * Data will be printed in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to print
 * @param file File to print to
 *
*/
void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file){
        Block* block = blockChain.block;
        if (block != nullptr){
                BlockDumpHashed(*block, file);
        }
}


/**
 * BlockChainVerifyFile - verifies that the file contains correct hashed messages of the given BlockChain
 *
 * Input file is expected to contain data in the following format:
 * <hashed message>
 * <hashed message>
 * ...
 * <hashed message>
 *
 * @param blockChain BlockChain to verify
 * @param file File to read from
 *
 * @return true if the file is valid, false otherwise
*/
bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file){
        Block* block = blockChain.block;
        if (block != nullptr){
                return BlockVerifyFile(*block, file);
        }
        return (!(file.eof()));
}


/**
 * BlockChainCompress - Compresses the given block chain based on the transaction's data.
 * All consecutive blocks with the same sender and receiver will be compressed to one Block.
 *
 * @param blockChain BlockChain to compress
*/
void BlockChainCompress(BlockChain& blockChain){
        Block* block = blockChain.block;
        if (block != nullptr){
                BlockCompress(*block);
        }
}


/**
 * BlockChainTransform - Update the values of each transaction in the BlockChain
 *
 * @param blockChain BlockChain to update
 * @param function a pointer to a transform function
*/
void BlockChainTransform(BlockChain& blockChain, updateFunction function){
        BlockTransform(*(blockChain.block), function);
}


void DestroyBlockChain(BlockChain& blockChain){
        DestroyBlocks(*(blockChain.block));
}



/**
 * Block section
 */






int BlockGetSize(const Block& block){
        Block* before = block.next;
        int previous = (before == nullptr) ? 0 : BlockGetSize(*before);
        return (1 + previous);
}



int BlockPersonalBalance(const Block& block, const string& name){
    int prevBalance = (block.next == nullptr)?0:BlockPersonalBalance(*block.next,name);
    string infoSender = block.info.sender;
    string infoReciever = block.info.receiver;
    if(name == infoSender){
        prevBalance -= int(block.info.value);
    } else if (name == infoReciever) {
        prevBalance += int(block.info.value);
    }
    return prevBalance;
}


void BlockDump(const Block& block, ofstream& file){
        BlockDump(block, file, 1);
}

void BlockDump(const Block& block, ofstream& file, int num){
        file << num << "." << endl;
        Transaction transaction = block.info;
        TransactionDumpInfo(transaction,file);
        file << endl;
        file << block.time;
        Block* nextBlock = block.next;
        if (nextBlock == nullptr){
                return;
        }
        file << endl;
        BlockDump(*nextBlock, file, ++num);
}

void BlockDumpHashed(const Block& block, ofstream& file){
        Transaction transaction = block.info;
        file << TransactionHashedMessage(transaction);
        /**
        *
        * if(transaction != nullptr){
        *         file << TransactionHashedMessage(transaction);
        * }
         */

        Block* nextBlock = block.next;
        if (nextBlock != nullptr){
                file << endl;
                BlockDumpHashed(*nextBlock, file);
        }
}


bool BlockVerifyFile(const Block& block, std::ifstream& file){
        Transaction transaction = block.info;
        Block* nextBlock = block.next;
        bool thisCompare = true;
        string fileHash;
        //FIXME add null pointer check from here
        file >> fileHash;
        thisCompare = TransactionVerifyHashedMessage(transaction, fileHash);
        //FIXME until here
        if (!thisCompare){
                return false;
        }
        if (nextBlock == nullptr){return thisCompare;}
        return BlockVerifyFile(*nextBlock, file);
}





void BlockCompress(Block& block){
        //FIXME make sure that it is not possible to call this with a null reference
        Block* prev = &block;
        Block* curr = block.next;
        if(curr == nullptr){return;}
        Transaction transaction = block.info;
        string mySender = transaction.sender;
        string myReciever = transaction.receiver;
        while (curr != nullptr){
                string currSender = curr->info.sender;
                string currReciever = curr->info.receiver;
                if((mySender != currSender)||(myReciever != currReciever)){
                        prev = curr;
                        curr = curr->next;
                        continue;
                }
                block.info.value += curr->info.value;
                Block* temp = curr;
                prev->next = curr->next;
                curr = curr->next;
                DestroyBlock(*temp);
        }
        BlockCompress(*block.next);
}



void BlockTransform(Block& block, updateFunction function){
        /**
         * general idea:
         * first apply blockchaintransform call to all previous blockchains' then apply
         * the function to the current blockchains
         */
        Block* nextBlock = block.next;
        if (nextBlock){
                BlockTransform(*nextBlock, function);
        }
        int oldVal = int(block.info.value);
        block.info.value = function(oldVal);
}


void DestroyBlock(Block& block){
        //TODO COMPLETE
        block.next = nullptr;
        //delete[] block;
}


void DestroyBlocks(Block& block){
        Block* next = block.next;
        if(next != nullptr){DestroyBlocks(*block.next);}
        DestroyBlock(block);
}



