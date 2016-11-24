#include "seed-log.h" // Print information and write to a log-file
using namespace TREE;

#define LOGFILE "main.log"

void foo() {
	SEED_LOG << "What the hell is going on !";
}

int main(int argc, char const *argv[])
{
	
	foo();
	SEED_WARNING << "U should notice on ... !!";
	SEED_ERROR << "There're something wrong !!!";
	SEED_VLOG << "Show more information.";
	return 0;
}