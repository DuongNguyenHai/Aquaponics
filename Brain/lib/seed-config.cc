#include "seed-config.h"

namespace TREE {

bool PRINT_MONITOR = true;			// define for print on terminal in Log function.
bool PRINT_FILE = true;			// define for print to file in Log function.
int32_t DEBUG_LEVEL = 1;			// define for debug in database function.


std::string DEFINE_ARGS[3][2] = 	{ 	{"PRINT_MONITOR", Tp_BOOL},
										{"PRINT_FILE", Tp_BOOL},
										{"DEBUG_LEVEL", Tp_INT}
									};

const char *usage = 	"Need a config file: \"log/sys-config.cfg\" \n"
						"Usage : <option> = <content>\n"
						"ex : \n"
						"PRINT_MONITOR=true\n"
						"PRINT_FILE=true\n"
						"DEBUG_LEVEL=1\n";
} // end of namespace TREE