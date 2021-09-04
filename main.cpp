//
//  main.cpp
//  421OSFileSystem
//
//  Created by Calvin on 5/4/21.
//  Copyright © 2021 Calvin. All rights reserved.
//

#include <iostream>
#import "filesystem.hpp"
using namespace std;
#define ARGLEN  50500
#define MAXARGS 3

void parse( char* input, char* args[] ) {
    input[strlen(input)-1] = '\0';  // fgets reads \n, so overwrite it
    args[0] = strtok(input, " ");
    args[1] = strtok(NULL, " ");
    args[2] = strtok(NULL, "\0");   // for writing long string
}


int main(int argc, const char * argv[]) {
    
    char arg [ARGLEN];
    char* input[MAXARGS];
    Disk* disk1;
    
    disk1 = new Disk();
    
    while (strcmp(arg, "exit\n") != 0) {
        printf("FileSystem:~$ ");
        if (fgets(arg, ARGLEN, stdin) == NULL) {
            perror( "read input failed" );
            exit(1);
        }
        else {
            if (strcmp(arg, "exit\n") == 0) // exit
                exit(0);
            
            parse(arg, input);
            if ((strcmp(input[0], "CREATE") == 0) && (input[2]!= nullptr) && (input[1]!= nullptr)) { // CREATE U or D
                disk1->create(input[1][0], input[2]);
                disk1->display();
            }
            else if ((strcmp(input[0], "OPEN") == 0) && (input[2] != nullptr) && (input[1]!= nullptr)) { // OPEN U
                disk1->open(input[1][0], input[2]);
                disk1->display();
            }
            else if (strcmp(input[0], "CLOSE") == 0) { // CLOSE
                disk1->close();
                disk1->display();
            }
            else if ( (strcmp(input[0], "DELETE") == 0) && (input[1]!= nullptr) ) { // DELETE
                disk1->deleteNew(input[1]);
                disk1->display();
            }
            else if ( (strcmp(input[0], "WRITE") == 0) && (input[2]!= nullptr) && (input[1]!= nullptr) ) {  // WRITE n 'data'
                
                if ( (input[2][0] == '\'') && (input[2][strlen(input[2])-1] == '\'') ) {
                    
                    char trim[strlen(input[2])-2]; // trim off the single quotes
                    for (int i = 0; i < strlen(input[2])-2; i++) {
                        trim[i] = input[2][i+1];
                    }
                    disk1->write(atoi(input[1]), trim);
                    disk1->display();
                }
                else {
                    cout << "ERROR: USE SINGLE QUOTES \' \' AROUND WRITE INPUT" << endl;
                    disk1->display();
                }
            }
            else if ((strcmp(input[0], "READ") == 0) && (input[1]!= nullptr)) { // READ n
                disk1->read(atoi(input[1]));
                disk1->display();
            }
            else if ((strcmp(input[0], "SEEK") == 0) && (input[1]!= nullptr) && (input[2]!= nullptr)) { // SEEK base offset
                disk1->seek(atoi(input[1]), atoi(input[2]));
                disk1->display();
            }
            else {
                cout << "ERROR: INVALID INPUT" << endl; // Invalid input
                disk1->display();
            }
        }
    }
    return 0;
}

//- TESTS:
//CREATE U F1
//CLOSE
//CREATE D D1
//OPEN U F1
//CREATE U F2   // should throw error
//CLOSE
//CREATE U D1/F2
//CLOSE
//OPEN U F1
//WRITE 1300 'sample' // sample has fewer than 'n' bytes so blanks are appended to 'data' to make it 'n' bytes
//
//- Rewind: SEEK -1 0
//READ 3    // output should be 'sam'
//
//SEEK -1 0
//READ 10

//READ 1500
//SEEK -1 0
//
//WRITE 10 '12345678910'
//SEEK 0 -1
//READ 8  // output should be: 12345678(EOF)
//
//
//- OPEN/CLOSE TEST:
//CLOSE
//OPEN I D1/D2/F4
//CREATE U D1/D3/F5
//
//- DELETE TEST:
//DELETE D1/D2
