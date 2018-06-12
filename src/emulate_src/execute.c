#include "execute.h"

//------------EXECUTE FUNCTIONS-------------
//function to execute correct command
typedef void (*execution)(void);

//execution function table
//for the 4 possible functions
static execution ex_table[4] = {
  dataProcessing,
  multiply,
  singleDataTransfer,
  branch
};

//function to call execution function table
void execute_fn(int execute) {
  //execute = -1 represents the 0 instruction
  if(execute != -1) {
    ex_table[execute]();
  }
} 

