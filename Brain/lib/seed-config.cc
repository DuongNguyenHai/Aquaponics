#include "seed-config.h"

namespace TREE {

bool PRINT_MONITOR = true;			// define for print on terminal in Log function.
bool PRINT_FILE = true;			// define for print to file in Log function.
int32_t DEBUG_LEVEL = 2;			// define for debug in database function.
int32_t INTERVAL_REQUEST = 0;
std::vector<int> SCHEDULE;
std::string SCHEDULE_TYPE[2] = {"schedule", "timer"};
std::string LOG_FILE ("root.log");

std::string DEFINE_ARGS[5][2] = 	{ 	{"PRINT_MONITOR", Tp_BOOL},
										{"PRINT_FILE", Tp_BOOL},
										{"DEBUG_LEVEL", Tp_INT},
										{"SCHEDULE", Tp_TIME},
										{"LOG_FILE",Tp_STR}
									};

const char *usage = 	"Need a config file: \"config/root.conf\" \n"
						"Usage : <option> = <content>\n"
						"ex : \n"
						"PRINT_MONITOR=true\n"
						"PRINT_FILE=true\n"
						"DEBUG_LEVEL=1\n"
						"SCHEDULE={\"schedule\":[\"7h\",\"12h30'30\",\"18h\",\"0h\"]}\n"
						"#SCHEDULE={\"timer\":[1800]}\n"
						"LOG_FILE=/home/username/root/root.log";
} // end of namespace TREE