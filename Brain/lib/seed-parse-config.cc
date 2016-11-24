#include "seed-parse-config.h"

namespace TREE { 

// Define a file which is used for write log

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE "seed-config.log"
#else
#define LOGFILE "seed-config.log"
#endif	

static bool flag_err = false;

// Convert string to lowercase
static bool ToLowerCase(std::string &str){
    int i = 0;
    while (str[i]){
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        i++;
    }
    return true;
}

// convert string to bool
static int ToBool(std::string str) {
	ToLowerCase(str);
	if(strcmp(str.c_str(), "true")==0) {
		return 1;
	}
	else if (strcmp(str.c_str(), "false")==0) {
		return 0;
	}
	else 
		return -1;
}

// Convert string to int
bool ToInt(std::string str, int &result) {
	try {
	  	result = std::stoi(str);
	  	return true;
	} 
	catch (std::invalid_argument&) {
	  	return false;
	}
	catch (std::out_of_range&) {
	  	return false;
	}
}
// Check debug level : level must in range [0,3]
static bool Check_Args_DEBUG_LEVEL(int32_t num) {
	return (0<=num && num<=3) ? true : false;
}
// must be > 0
static bool Check_Args_INTERVAL_REQUEST(int32_t num) {
	return (num>=0) ? true : false;
}

static void PrintWarning(std::string arg, std::string val, std::string log) {
	SEED_WARNING << "Config : \"" << arg << "=" << val << "\" "<< log <<"\n";
	flag_err=true;
}

ParseFile::ParseFile(const char *file) {
	index = 0;
	fileName = file;
}

ParseFile::~ParseFile() {}

bool ParseFile::ParseArgs(std::string args[][2], int maxSize) {
	
	pfile.open(fileName.c_str());

	if (pfile.is_open()) {

		while ( getline (pfile, str) ) {
			if(*str.begin()=='#') // detect comment line
				continue;
			str.erase(std::remove(str.begin(), str.end(), ' '),str.end());	// remove whitespace
			str.erase(std::remove(str.begin(), str.end(), '\t'),str.end()); // remove tab

			int strLeng = str.length();
			if( (strLeng<2) || *str.begin()=='#')	// detect empty or line only contain tab .. or comment line
				continue;

			std::size_t found = str.find('=');
			if(found!=std::string::npos) {
				std::string header = str.substr(0,str.find('='));
				std::string content = str.substr(str.find('=')+1,strLeng);
				args[index][0] = header;
				args[index][1] = content;
				index++;
				
				if(index >= maxSize)
					return RET_SUCCESS;
			}
		}
	} else {
		return RET_FAILURE;
	}

	pfile.close();
	return RET_SUCCESS;
}

ParseOptions::ParseOptions(const char *file) : ParseFile(file) {
	GetOptions();
}

ParseOptions::~ParseOptions() {}

// Get option from the file
void ParseOptions::GetOptions() {

	std::string args[NUM_ROW(DEFINE_ARGS)][2];

	if(ParseArgs(args, NUM_ROW(args)) == RET_FAILURE) {// parse option from file.
		SEED_LOG << "Need a config file : \"sys-config.cfg\" in a \"log\" folder. All configuration was written in it";
		PrintUsage();
	}

	for (unsigned int i = 0; i < NUM_ROW(args); i++) {
	
		if(!args[i][0].empty())	{	// Check wherether args[i][0] is empty or not.
			for (unsigned int j = 0; j < NUM_ROW(args); j++) {
				if(strcmp(args[i][0].c_str(), DEFINE_ARGS[j][0].c_str())==0) {
					switch (j) {
						case 0 : {	// Set PRINT_MONITOR
							int val = ToBool(args[i][1]);
							if(val!=-1)
								PRINT_MONITOR = val;
							else
								PrintWarning(args[i][0], args[i][1], "is wrong");
						} break;
						case 1 : { // Set PRINT_FILE
							int val = ToBool(args[i][1]);
							if(val!=-1)
								PRINT_FILE = val;
							else
								PrintWarning(args[i][0], args[i][1], "is wrong");
						} break;
						case 2 : { // Set DEBUG_LEVEL
							if(ToInt(args[i][1], DEBUG_LEVEL)) { // string convert to int
								if(!Check_Args_DEBUG_LEVEL(DEBUG_LEVEL)) {	// out of range
									DEBUG_LEVEL = 1;	// must set DEBUG_LEVEL > 0 for use SEED_WARNING
									PrintWarning(args[i][0], args[i][1], "out of range");
								}
							}
							else
								PrintWarning(args[i][0], args[i][1], "is wrong");

						} break;
						case 3 : {	// set INTERVAL_REQUEST
							if(ToInt(args[i][1], INTERVAL_REQUEST)) { // string convert to int
								if(!Check_Args_INTERVAL_REQUEST(INTERVAL_REQUEST)) // out of range
									PrintWarning(args[i][0], args[i][1], "out of range");
							}
							else
								PrintWarning(args[i][0], args[i][1], "is wrong");
						}
						default: break;
					}

					break;
				}
				if(j==NUM_ROW(DEFINE_ARGS)-1) {	// not found in config array
					PrintWarning(args[i][0], args[i][1], "is not config");
				}
			}
		
		}

	}

	if(flag_err)
		PrintUsage();
}

void ParseOptions::PrintUsage() {
	SEED_ERROR << "\n\n" << usage << "\n";
}

}	// end of namespace TREE



