#include "seed-config.h"

namespace TREE {

bool PRINT_MONITOR = true;			// define for print on terminal in Log function.
bool PRINT_FILE = true;			// define for print to file in Log function.
int32_t DEBUG_DATABASE_LV = 2;			// define for debug in database function.


std::string DEFINE_ARGS[3][2] = 	{ 	{"PRINT_MONITOR", Tp_BOOL},
												{"PRINT_FILE", Tp_BOOL},
												{"DEBUG_DATABASE_LV", Tp_INT}
											};

const char *usage = 	"Write configs in file : sys-config.cfg \n"
							"Usage : <option> = <content>\n"
							"ex : \n"
							"PRINT_MONITOR=true\n"
							"PRINT_FILE=true\n"
							"DEBUG_DATABASE_LV=2\n";
} // end of namespace TREE