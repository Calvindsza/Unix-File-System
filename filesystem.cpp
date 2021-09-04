//
//  filesystem.cpp
//  421OSFileSystem
//
//  Created by Calvin on 5/4/21.
//  Copyright © 2021 Calvin. All rights reserved.
//

#include "filesystem.hpp"

Block::Block() {
    number = -1;
    dir = false;
    frwd = nullptr;
    back = nullptr;
    name = "";
}

// BLOCK CLASS
Block::Block(int number, bool isDir, std::string name) {
    this->dir = isDir;
    this->name = name;
    this->number = number;
    
    if (number == 0) {
        frwd = this;
        back = this;
    }
    else {
        frwd = nullptr;
        back = nullptr;
    }
}

// Block Destructor
Block::~Block() {
    std::cout << "BLOCK " << this->getNumber() << " NOW EMPTY\n";
    reset();
}
    
// Get Block Number
int Block::getNumber(){
    return number;
}

// True if Block is Dir, False if Block is User File
bool Block::isDir() {
    return dir;
}

// Set FRWD Pointer
void Block::setFrwd(Block* frwd) {
    this->frwd = frwd;
}

// Get FRWD Pointer
Block* Block::getFrwd() {
    return frwd;
}

// Set BACK Pointer
void Block::setBack(Block* back) {
    this->back = back;
}

// Get BACK Pointer
Block* Block::getBack() {
    return back;
}

// Reset Block
void Block::reset() {
    number = -1;
    dir = false;
    frwd = nullptr;
    back = nullptr;
    name = "";
}

// Get Block Name
std::string Block::getName() {
    return name;
}



// Directory Class
Directory::Directory(int number, std::string name): Block(number, true, name) {
    free = -1;
    for (int i = 0; i < DIR_SIZE; i++) {
        entry[i] = nullptr;
    }
}

Directory::~Directory() {
    std::cout << "DIRECTORY " << this->getName() << " DELETED\n";
}

// get the entry block* of name
Directory* Directory::getDirEntry(std::string name) {
    Directory* temp;
    temp = this;
    
    for (int i = 0 ; i < DIR_SIZE; i++) {
        if (entry[i] != nullptr && entry[i]->getName() == name && entry[i]->isDir() == true) {
            temp = (Directory*)entry[i];
            break;
        }
    }
    return temp;
}

// get the file block* of name
Block* Directory::getFileEntry(std::string name) {
    Block* temp;
    temp = nullptr;
    
    for (int i = 0 ; i < DIR_SIZE; i++) {
        if (entry[i] != nullptr && entry[i]->getName() == name && entry[i]->isDir() == false) {
            temp = entry[i];
            break;
        }
    }
    return temp;
}

// return the Block* of entry;
Block** Directory::getEntryHead() {
    return entry;
}

// return the block number of the to be deleted Block*
int Directory::deleteNumber(Block* toDelete) {
    int blockNumber;
    blockNumber = -1;

    // set entry[i] == file to nullptr
    for (int i = 0; i < DIR_SIZE; i++) {
        if (entry[i]->getNumber() == toDelete->getNumber()) {
            blockNumber = entry[i]->getNumber();
            entry[i] = nullptr;
            break;
        }
    }
    return blockNumber;
}

// Return size of the Directory Block
int Directory::getSize() {
    int size;
    size = 0;
    
    for (int i = 0; i < DIR_SIZE; i++) {
        if (entry[i] != nullptr) {
            size++;
        }
    }
    return size;
}

// Add/allocate a new Block
void Directory::addEntry(Block* newEntry) {
    for (int i = 0; i < DIR_SIZE; i++) {
        if (entry[i] == nullptr) {
            entry[i] = newEntry;
            break;
        }
    }
}

void Directory::setFree(int index) {
    if (this->getNumber() == 0) {
        free = index;
    }
    else
        free = -1;
}

// display directory content
void Directory::display(std::string indent) {
    // display name, type, block number, and size
    std::cout << indent << this->getName() << " | D | "; // display name, (D)irectory
    std::cout << "BLOCK: " << this->getNumber() << " | SIZE: " << this->getSize() << std::endl; //
    
    // display back pointer block #
    if (this->getBack() == nullptr) {
        std::cout << indent << "-BACK: nullptr" << std::endl;
    }
    else {
        std::cout << indent << "-BACK: " << this->getBack()->getNumber() << std::endl;
    }
    
    // display forward pointer block #
    if (this->getFrwd() == nullptr) {
        std::cout << indent << "-FRWD: nullptr" << std::endl;
    }
    else {
        std::cout << indent << "-FRWD: " << this->getFrwd()->getNumber() << std::endl;
    }
    
    if (this->getNumber() == 0) {
        std::cout << indent << "-FREE: " << free << std::endl;
    }
    
    // display unused filler
    std::cout << indent << "-FILLER: UNUSED" << std::endl;
    
    if (isEmpty() == false) {
        indent += "\t";
        for (int i = 0; i < DIR_SIZE; i++) {
            if (entry[i] != nullptr) {
                if (entry[i]->isDir() == true) {
                    ((Directory*)entry[i])->display(indent);
                }
                else {
                    ((File*)entry[i])->display(indent);
                }
            }
        }
    }
}

 // check whether the directory is empty
bool Directory::isEmpty() {
    bool flag;
    flag = true;
    
    for (int i = 0; i < DIR_SIZE; i++) {
        if (entry[i] != nullptr) {
            flag = false;
            break;
        }
    }
    return flag;
}


// FILE CLASS
File::File(int number, std::string name): Block(number, false, name) {
    for (int i = 0; i < FILE_SIZE; i++) {
        data[i] = '\0';
    }
}

// destructor of file
File::~File() {
    std::cout << "FILE " << this->getName() << " DELETED"<< std::endl;
}

// write input into file
void File::writeFile(int& count, std::string& input, int& current) {
    for (; current < FILE_SIZE && 0 < count; count--) {
        if (input != "") {
            data[current] = input[0];
            input = input.substr(input.find(input[0])+1);
            current++;
            cursor++;
        }
        else {
            data[current] = ' ';
            cursor++;
            current++;
        }
    }
}

// read and display file data
void File::readFile(int& count, int& current) {
    for (; current < FILE_SIZE && 0 < count; count--) {
        if (data[current] != '\0') {
            std::cout << data[current];
            cursor++;
            current++;
        }
        else {
            break;
        }
    }
}

// display file content
void File::display(std::string indent) {
    // display file name
    std::cout << indent << this->getName() << " | U | ";
    std::cout << "BLOCK: " << this->getNumber();
    
    // display file linkers
    Block* temp;
    Block* size;
    temp = this->getFrwd();
    size = this;
    
    while (temp != nullptr) {
        std::cout << "<--B  F-->" << temp->getNumber();
        size = temp;
        temp = temp->getFrwd();
    }
    
    // display file size
    std::cout << " | SIZE: " << ((File*)size)->getSize() << std::endl;
}

// return eof index of data[]
int File::getEnd() {
    int current;
    bool flag;
    current = 0;
    flag = false;
    
    for (int i = 0; i < FILE_SIZE; i++) {
        if (data[i] == '\0') {
            current = i;
            flag = true;
            break;
        }
    }
    
    File* next;
    next = (File*)this->getFrwd();
    
    if (flag == false && next != nullptr) {
        return next->getEnd()+504;
    }
    else {
        return current;
    }
}

// return the size of the directory block
int File::getSize() {
    int size;
    size = 0;
    
    for (int i = 0; i < FILE_SIZE; i++) {
        if (data[i] != '\0') {
            size++;
        }
    }
    return size;
}


// DISK CLASS
Disk::Disk() {
    freeSpaceList[0] = false;
    sector[0] = new Directory(0, "root");
    for (int i = 1; i < DISK_SIZE; i++) {
        freeSpaceList[i] = true;
        sector[i] = nullptr;
    }
    close();
}

// destructor of disk
Disk::~Disk() {
    std::cout << "DISK DELETED\n";
}

// return the index of first available sector in the disk
int Disk::index() {
    int index;
    index = -1;
    
    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeSpaceList[i] == true) {
            index = i;
            break;
        }
    }
    return index;
}

// check the open status;
bool Disk::isOpen() {
    bool flag;
    flag = true;
    if (openBlock == -1 && openMode == -1 && cursor == -1) {
        flag = false;
    }
    return flag;
}

// display the hierarchy structure of the disk
void Disk::display() {
    std::cout << "------------------- FileSystem -------------------\n\n";
    ((Directory*)sector[0])->setFree(index());
    ((Directory*)sector[0])->display("");
    count();
    
    std::cout << "\nMODE: ";
    switch(openMode) {
        case 0:
            std::cout << "I\t";
            break;
        case 1:
            std::cout << "O\t";
            break;
        case 2:
            std::cout << "U\t";
            break;
        default:
            std::cout << "NA\t";
            break;
    }
    
    std::cout << "BLOCK: ";
    if (openBlock == -1) {
        std::cout << "NA\t";
    }
    else {
        std::cout << sector[openBlock]->getName() << "\t";
    }
    
    std::cout << "CURSOR: ";
    if (cursor == -1) {
        std::cout << "NA\t";
    }
    else {
        std::cout << cursor << "\t";
    }
    
    std::cout << "\n___________________________________________________\n\n" ;
}

// # Free Blocks, # Directory Blocks, # File Blocks
void Disk::count() {
    int f, d, u;
    f = 0;
    d = 0;
    u = 0;
    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeSpaceList[i] == 1) {
            f++;
        }
        else {
            if (sector[i]->isDir() == true)
                d++;
            else
                u++;
        }
    }
    std::cout << "\n#F: " << f << std::endl;
    std::cout << "#D: " << d << std::endl;
    std::cout << "#U: " << u << std::endl;
}

// create a new block
void Disk::create(char type, std::string name) {
    std::string original = name;
    
    if (type != 'U' && type != 'D') {
        std::cout << "CREATE ERROR: " << name << " MUST SPECIFY TYPE - 'U' OR 'D'\n";
    }
    else if (isOpen() == true && type == 'U') {
        std::cout << "CREATE ERROR: " << name << " FAILED - " << sector[openBlock]->getName() << " IS OPEN. ENTER 'CLOSE' BEFORE CREATING ANOTHER FILE\n";
    }
    else {
        Directory* parent;
        // get the parent directory for the coming block
        parent = helpFind((Directory*)sector[0], name);
        
        if (parent == nullptr && index() != -1) {
            std::cout << "CREATE ERROR: FILE NAME " << name << " INVALID\n";
        }
        else if (parent == nullptr && index() == -1) {
            std::cout << "CREATE ERROR: " << name << " ALL BLOCKS ARE USED\n";
        }
        else {
            while (name.find('/') != name.npos) {
                name = name.substr(name.find('/')+1);
            }
            
            Block* newBlock;
            newBlock = helpCreate(type, name);
            if (newBlock != nullptr) {
                parent->addEntry(newBlock);
                std::cout << "Created " << name << std::endl;
                
                if (type == 'U') {
                    open('O', original);
                }
            }
            else {
                std::cout << "CREATE ERROR: " << name << " ALL BLOCKS ARE USED\n";
            }
        }
    }
}

// open a block
void Disk::open(char mode, std::string name) {
    if (isOpen() == true) {
        std::cout << "OPEN ERROR: " << name << " FAILED - " << sector[openBlock]->getName() << " IS OPEN. ENTER 'CLOSE' BEFORE CREATING ANOTHER FILE\n";
    }
    else if (mode != 'I' && mode != 'U' && mode != 'O')
    {
        std::cout << "OPEN ERROR: " << name << " FAILED - ENTER OPEN MODE 'I', 'U', 'O'\n";
    }
    else {
        Block* file;
        file = findBlock(name);
        
        if (file == nullptr) {
            std::cout << "OPEN ERROR: INVALID FILE NAME " << name << std::endl;
        }
        else {
            openBlock = file->getNumber();
            cursor = 0;
            if (mode == 'I') {
                openMode = 0;
                seek(-1, 0);
            }
            else if (mode == 'O') {
                openMode = 1;
                cursor = ((File*)sector[openBlock])->getEnd();
            }
            else {
                openMode = 2;
                seek(-1, 0);
            }
        }
    }
}

// closed the last open block (reset)
void Disk::close() {
    openBlock = -1;
    openMode = -1;
    cursor = -1;
}

// Delete block
void Disk::deleteNew(std::string name) {
    Block* toBeDelete;
    Directory* parent;
    int blockNum;
    
    blockNum = -1;
    toBeDelete = findBlock(name);
    parent = findParent((Directory*)sector[0],name);
    
    // Delete File block
    if (toBeDelete != nullptr && parent != nullptr && toBeDelete->isDir() == false) {
        blockNum = toBeDelete->getNumber();
        deleteBlock(parent, toBeDelete);
    }
    // Delete Dir block
    else if (toBeDelete!= nullptr && parent != nullptr && toBeDelete->isDir() == true) {
        blockNum = toBeDelete->getNumber();
        deleteBlock(parent, (Directory*)toBeDelete);
    }
    else {
        std::cout << "DELETE ERROR: INVALID FILE NAME " << name << std::endl;
    }
    
    if (blockNum == openBlock) {
        close();
    }
}

// WRITE
void Disk::write(int count, std::string input) {
    if (isOpen() == true && openMode == 0) {
        std::cout << "WRITE ERROR: " << sector[openBlock]->getName() << " IS IN INPUT MODE\n";
    }
    else if (isOpen() == false) {
        std::cout << "WRITE ERROR: NO FILE OPEN\n";
    }
    else {
        int currentCursor, blockNum;
        File* next;
        
        currentCursor = cursor;
        blockNum = openBlock;
        next = (File*)(sector[openBlock]->getFrwd());
        
        while (currentCursor >= FILE_SIZE && next != nullptr) {
            openBlock = next->getNumber();
            currentCursor -= FILE_SIZE;
            next = (File*)(sector[openBlock]->getFrwd());
        }
        
        if (currentCursor >= FILE_SIZE) {
            std::cout << "WRITE ERROR: CURSOR IS GREATER THAN BLOCK " << sector[openBlock]->getName() << std::endl;
        }
        else {
            write2(count, input, currentCursor);
        }
        openBlock = blockNum;
    }
}

// WRITE helper function
void Disk::write2(int& count, std::string& input, int& current) {
    Block* newFile;
    int totalToWrite, wrote;
    
    newFile = sector[openBlock];
    totalToWrite = current + count;
    wrote = 0;
    
    while (totalToWrite >= FILE_SIZE) {
        // count the total # of wrote
        current = current % FILE_SIZE;
        if (FILE_SIZE > count) {
            wrote = count - current;
        }
        else {
            wrote = FILE_SIZE - current;
        }

        ((File*)sector[openBlock])->writeFile(count, input, current);
        totalToWrite -= wrote;
        
        // Allocate next free block
        newFile = helpCreate('U', sector[openBlock]->getName());
        
        if (newFile != nullptr) {
            sector[openBlock]->setFrwd(newFile);
            newFile->setBack(sector[openBlock]);
            openBlock = newFile->getNumber();
        }
        else {
            break;
        }
    }
    
    if (totalToWrite < FILE_SIZE && newFile != nullptr) {
        current = current % FILE_SIZE;
        ((File*)sector[openBlock])->writeFile(count, input, current);
        if (count > 0) {
            std::cout << "WRITE ERROR: ALL BLOCKS USED\n";
        }
    }
    else {
        std::cout << "WRITE ERROR: ALL BLOCKS USED\n";
    }
}


// READ
void Disk::read(int count) {
    if (isOpen() == true && openMode == 1) {
        std::cout << "READ ERROR: " << sector[openBlock]->getName() << " IS IN OUTPUT MODE\n";
    }
    else if (isOpen() == false) {
        std::cout << "READ ERROR: NO FILE OPEN\n";
    }
    else {
        int current, blockNum;
        Block* next;

        current = cursor;
        blockNum = openBlock;
        next = sector[openBlock];
        
        while (current >= FILE_SIZE && next->getFrwd() != nullptr) {
            next = next->getFrwd();
            openBlock = next->getNumber();
            current -= FILE_SIZE;
        }
        
        if (current >= FILE_SIZE) {
            std::cout << "READ ERROR: CURSOR REACHED SIZE OF " << sector[openBlock]->getName() << std::endl;
        }
        else {
            int totalRead, readed;
            totalRead = current + count;
            next = sector[openBlock];
            readed = 0;
            
            while (totalRead >= FILE_SIZE) {
                current = current % FILE_SIZE;
                if (FILE_SIZE > count) {
                    readed = count - current;
                }
                else {
                    readed = FILE_SIZE - current;
                }

                ((File*)sector[openBlock])->readFile(count, current);
                totalRead -= readed;
                
                next = sector[openBlock]->getFrwd();
                if (next != nullptr) {
                    openBlock = next->getNumber();
                }
                else {
                    break;
                }
            }
            
            if (totalRead < FILE_SIZE && next != nullptr) {
                current = current % FILE_SIZE;
                ((File*)sector[openBlock])->readFile(count, current);
                if (count > 0) {
                    std::cout << "(EOF) End Of File\n";
                }
            }
            else {
                std::cout << "(EOF) End Of File\n";
            }
        }
        openBlock = blockNum;
    }
}

// SEEK
void Disk::seek(int base, int offset) {
    if (isOpen() == true && openMode == 1) {
        std::cout << "SEEK ERROR: " << sector[openBlock]->getName() << " IN OUTPUT MODE\n";
    }
    else if (isOpen() == false) {
        std::cout << "SEEK ERROR: NO FILE OPEN\n" << std::endl;
    }
    else {
        if (base == -1 && offset >= 0) {
            cursor = 0;
            cursor += offset;
        }
        // Beginning
        else if (base == -1 && offset < 0) {
            std::cout << "SEEK ERROR: CANNOT REWIND\n";
        }
        
        // Current position
        else if (base == 0) {
            if (cursor + offset < 0) {
                std::cout << "SEEK ERROR: CANNOT REWIND\n";
            }
            else {
                cursor += offset;
            }
        }
        
        // End of file
        else if (base == 1) {
            if (((File*)sector[openBlock])->getEnd() + offset < 0) {
                std::cout << "SEEK ERROR: CANNOT REWIND\n";
            }
            else {
                cursor = ((File*)sector[openBlock])->getEnd() + offset;
            }
        }
    }
}

Block* Disk::findBlock(std::string name) {
    std::string original, name1;
    original = name;
    name1 = name.substr(name.find('/')+1);
    
    while (name1.find('/') != name1.npos) {
        name1 = name1.substr(name1.find('/')+1);
    }
    
    if (name1.find('/') && name1.length() <= 9) {
        Block* temp;
        temp = nullptr;
        
        for (int i = 0; i < DISK_SIZE; i++) {
            if (sector[i]!= nullptr && sector[i]->getName() == name1) {
                temp = sector[i];
                break;
            }
        }
        return temp;
    }
    else {
        return nullptr;
    }
}

Directory* Disk::findParent(Directory* super, std::string name) {
    if (name.find('/') != name.npos) {
        std::string name1, name2;
        name1 = name.substr(0, name.find('/'));
        name2 = name.substr(name.find('/')+1);
        
        Directory* sub;
        sub = super->getDirEntry(name1);
        
        //  If subDir exist in parentDir
        if(sub != super) {
            return findParent(sub, name2);
        }
        else {
            return nullptr;
        }
    }
    else if (name.find('/') && name.length() <= 9) {
        Block* file;
        Directory* sub;
        sub = super->getDirEntry(name);
        file = super->getFileEntry(name);
        
        // sub exist as a directory in parent directory super
        if (sub != super && file == nullptr) {
            return super;
        }
        // file exist as a file in parent directory super
        else if (sub == super && file != nullptr) {
            return super;
        }
        // NEITHER sub or file exist in parent directory super
        else {
            return nullptr;
        }
    }
    else
        return nullptr;
}

// (for create) return the Block* of newly allocated block
Block* Disk::helpCreate(char type, std::string name) {
    Block* newBlock;
    int sectorNum;
    
    // initial
    sectorNum = index();
    newBlock = nullptr;
    
    if (index() != -1) {
        if (type == 'U') {
            sector[sectorNum] =  new File (sectorNum, name);
        }
        else {
            sector[sectorNum] = new Directory (sectorNum, name);
        }
        freeSpaceList[sectorNum] = false;
        newBlock = sector[sectorNum];
        std::cout << "Allocated Block " << sectorNum << std::endl;
    }
    
    return newBlock;
}

// CREATE: return the Block* of the parent directory (if exist, delete and recreate)
Directory* Disk::helpFind(Directory* super, std::string name) {
    if (name.find('/') != name.npos) {
        std::string name1, name2;
        name1 = name.substr(0, name.find('/'));
        name2 = name.substr(name.find('/')+1);
        
        Directory* sub;
        sub = super->getDirEntry(name1);
        
        // if directory name exist in parent directory super
        if(sub != super){
            return helpFind(sub, name2);
        }

        else {
            Block* newEntry;
            newEntry = helpCreate('D', name1);
            if (newEntry != nullptr) {
                super->addEntry(newEntry);
                return helpFind((Directory*)newEntry, name2);
            }
            else {
                return nullptr;
            }
        }
    }
    else if (name.find('/') && name.length() <= 9) {
        Block* file;
        Directory* sub;
        sub = super->getDirEntry(name);
        file = super->getFileEntry(name);
        
        // sub doesn't exist as a directory in parent directory super
        if (sub == super && file == nullptr) {
            return super;
        }
        // sub exist as a directory in parent directory super
        else if (sub != super) {
            deleteBlock(super, sub);
            return super;
        }
        // file exist as a file in parent directory super
        else {
            deleteBlock(super, file);
            return super;
        }
    }
    else
        return nullptr;
}

// delete a allocated dir block dir from directory super
void Disk::deleteBlock(Directory* super, Directory* dir) {
    int deleteNum;
    
    // empty directory
    if (dir->isEmpty() == true) {
        deleteNum = super->deleteNumber(dir);
        
        if (deleteNum == -1) {
            std::cout << "DELETE ERROR: FILE " << dir->getName() << " NOT FOUND IN DIRECTORY " << super->getName() << std::endl;
        }
        else {
            delete sector[deleteNum];
            sector[deleteNum]=nullptr;
            freeSpaceList[deleteNum] = true;
        }
    }
    // directory not empty
    else {
        // empty dir
        Block** entry;
        entry = dir->getEntryHead();
        
        for (int i = 0; i < DIR_SIZE; i++) {
            if (entry[i] != nullptr) {
                if (entry[i]->isDir() == true) {
                    deleteBlock(dir, (Directory*)entry[i]);
                }
                else {
                    deleteBlock(dir, entry[i]);
                }
            }
        }
        
        // delete the empty dir from super
        deleteNum = super->deleteNumber(dir);
        
        if (deleteNum == -1) {
            std::cout << "DELETE ERROR: FILE " << dir->getName() << " NOT FOUND IN DIRECTORY " << super->getName() << std::endl;
        }
        else {
            delete sector[deleteNum];
            sector[deleteNum]=nullptr;
            freeSpaceList[deleteNum] = true;
        }
    }
}

// delete a allocated block file from directory super
void Disk::deleteBlock(Directory* super, Block* file) {
    int deleteNum;

    // no linker
    if (file->getBack() == nullptr && file->getFrwd() == nullptr) {
        deleteNum = super->deleteNumber(file);
        
        if (deleteNum == -1) {
            std::cout << "DELETE ERROR: FILE " << file->getName() << " NOT FOUND IN DIRECTORY " << super->getName() << std::endl;
        }
        else {
            delete sector[deleteNum];
            sector[deleteNum]=nullptr;
            freeSpaceList[deleteNum] = true;
        }
    }
    // first/middle/last node in the link list
    else {
        Block* front;
        Block* back;
        front = file;
        back = nullptr;
        
        // get to the last node
        while (front->getFrwd() != nullptr) {
            front = front->getFrwd();
            back = front->getBack();
        }
        
        // delete the last node and traverse back
        while (back != nullptr) {
            deleteNum = front->getNumber();
            front->setBack((Block*)nullptr);
            back->setFrwd((Block*)nullptr);
            
            delete sector[deleteNum];
            sector[deleteNum]=nullptr;
            freeSpaceList[deleteNum] = true;
            
            front = back;
            back = front->getBack();
        }
        
        // delete the first node
        deleteNum = super->deleteNumber(file);
        delete sector[deleteNum];
        sector[deleteNum]=nullptr;
        freeSpaceList[deleteNum] = true;
    }
}
