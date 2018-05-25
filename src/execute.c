#include "Data_Processing.h"
#include "Multiply.h"
#include "Single_Data_Transfer.h"
#include "Branch.h"
#include "execute.h"

//------------EXECUTE FUNCTIONS-------------
//function to execute correct command
typedef int (*execution)(void);

//execution function table
static execution ex_table[4] = {
  dataProcessing,
  multiply,
  singleDataTransfer,
  branch
};

//function to call execution function table
void execute_fn(int execute) {
  if(execute != -1) {
    ex_table[execute]();
  }
} 