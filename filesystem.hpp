//
//  filesystem.hpp
//  421OSFileSystem
//
//  Created by Calvin on 5/4/21.
//  Copyright © 2021 Calvin. All rights reserved.
//

#ifndef filesystem_hpp
#define filesystem_hpp

#include <stdio.h>
#include <string>
#include <iostream>

const int DISK_SIZE = 100; // 100 blocks (sectors)
const int FILE_SIZE = 504; // 512(block size) - 4(frwd) + 4(back) = 504
const int DIR_SIZE = 31;
static int openBlock = -1;
static int openMode = -1;   // 0 = input, 1 = output, 2 = update
static int cursor = -1;

class Block {
    private:
        int number;
        Block* frwd;
        Block* back;
        std::string name;
        bool dir;
    
    public:
        Block();                                    // default constructor for block
        Block(int number, bool isDir, std::string name); // constructor for directory or file block
        virtual~Block();                            // virtual destructor
        int getNumber();                            // return block number
        bool isDir();  // return the type of block
        void setFrwd(Block* frwd);   // set frwd block pointer
        void setBack(Block* back);   // set back block pointer
        void reset();  // reset a Block* to nullptr
        Block* getFrwd();  // return frwd block pointer
        Block* getBack();  // return back block pointer
        std::string getName();   // return block name
};




class Directory: public Block {
    private:
        Block* entry[DIR_SIZE];
        int free;  // ROOT: block number of first unused block
    
    public:
        Directory(int number, std::string name);
        ~Directory();
        Directory* getDirEntry(std::string name);  // Get directory block* for name
        Block* getFileEntry(std::string name);  // Get File block* for name
        Block** getEntryHead();
        int deleteNumber(Block* file);  // Return Block# to be deleted
        int getSize();  // Return Dir block size
        void addEntry(Block* newEntry);  // Add/allocate a new Block
        void setFree(int index);  // root block
        void display(std::string indent);  // Display File System
        bool isEmpty();  // Check if Directory is empty
};



class File: public Block {
    private:
        char data[FILE_SIZE];
    
    public:
        File(int number, std::string name);
        ~File();
        void writeFile(int& count, std::string& input, int& current); // WRITE
        void readFile(int& count, int& current); // READ (display)
        void display(std::string indent); // Print file content
        int getEnd();  // Return EOF
        int getSize(); // Return File block size
};


// Disk Class
class Disk: public Block {
    private:
        Block* sector[DISK_SIZE];
        bool freeSpaceList[DISK_SIZE];  // true if F(free), false if D/U
    
    public:
        Disk();
        ~Disk();
        int index();  // return the index of first available sector in the disk
        bool isOpen();  // check the open status;
        void display();  // display the hierarchy structure of the disk
        void count();   // count the type of file in the disk
        void create(char type, std::string name);   // create a new block
        void open(char mode, std::string name);  // open a block
        void close();   // closed the last open block (reset)
        void deleteNew(std::string name);   // delete the name block
        void write(int count, std::string input);  // WRITE
        void write2(int &count, std::string &input, int &current); // WRITE helper
        void read(int count);  // READ
        void seek(int base, int offset);  // SEEK
        Block* findBlock(std::string name);  // return the Block* of name file block
        Directory* findParent(Directory* super, std::string name); // Return Block of parent Dir
        Block* helpCreate(char type, std::string name); // return the Block* of newly allocated block
        Directory* helpFind(Directory* super, std::string name); // Return Block* of parentDir (overwrite)
        void deleteBlock(Directory* super, Directory* dir); // Delete subDir block
        void deleteBlock(Directory* super, Block* file); // Delete file block from superDir
};
#endif /* filesystem_hpp */
