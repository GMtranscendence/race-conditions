#include <iostream>
#include <string>
// Required by for routine
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>   // Declaration for exit()
#include <unistd.h>
using namespace std;
int globalVariable = 2;


int main(){
 string sIdentifier;
 int iStackVariable = 20;
 pid_t pID = vfork();

 if (pID == 0){
    // Code only executed by child process
    sIdentifier = "Child Process with pid: " ;
    globalVariable++;
    iStackVariable++;

  }
  else if (pID < 0){
      cerr << "Failed to fork" << endl;
      exit(1);
  }

  else{
    // Code only executed by parent process
    sIdentifier = "Parent Process with child's pid: ";
    cout << sIdentifier << pID;

  }
  // Code executed by both parent and child.
  cout << " Global variable: " << globalVariable;
  cout << " Stack variable: "  << iStackVariable << endl;
}
