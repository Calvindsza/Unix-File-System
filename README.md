# Unix-File-System
Working Unix file storage system with Create, Read, Write, Open, Delete and Seek functionality, written in C++

## Commands

### CREATE *type name*

type: type of file to be created. U: User Data File, D: Directory File

name: name of file to create

Create File `CREATE U file1`

Create Directory `CREATE D Dir1`

---
### OPEN *mode name*

*mode*: how you want to openn the specified file

I - Input: READ and SEEK

O - Output: WRITE 

U - Update: READ, WRITE and SEEK

*name*: name of file

---    
### READ *n*

*n*: number of bytes to be read. **READ** can only be performed after **OPEN I** or **OPEN U** and must be followed by **CLOSE**

---
### WRITE *n data*

Writes *n* bytes of *data* into an **OPEN**ed file

---
### SEEK *base offset*

*base* Indicates starting point of file 

-1: beginning of the file

0: current position in the file

1: EOF

*offset* Indicates the number of bytes from <base> to which file reading pointer should be moved to

--- 
### DELETE *name*

Deletes a file provided its name
    
---
### CLOSE

Closes a file. Can only be used after a file is **OPEN**ed or **CREATE**d
