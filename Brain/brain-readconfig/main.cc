#include "seed-config.h"
#include "seed-parse-config.h" // Print information and write to a log-file
#include "seed-log.h"

using namespace TREE;


int main(int argc, char const *argv[])
{
	ParseOptions po("root.conf");
	
	std::cout << "PRINT_MONITOR : " << ((PRINT_MONITOR) ? "true" : "false") << "\n"
				<< "PRINT_FILE : " << ((PRINT_FILE) ? "true" : "false") << "\n"
				<< "DB_DATABASE_LV : " << DB_DATABASE_LV << "\n";
	return 0;
}