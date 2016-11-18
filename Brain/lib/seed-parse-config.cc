#include "seed-parse-config.h"

namespace TREE { 

// Define a file which is used for write log

#ifdef LOGFILE
#undef LOGFILE
#define LOGFILE "seed-config.log"
#else
#define LOGFILE "seed-config.log"
#endif	

// Convert string to lowercase
static bool ToLowerCase(std::string &str){
    int i = 0;
    // char *str = strdup(s);
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
static bool ToInt(const std::string &str, int32_t &result) {
	try {
	  std::size_t lastChar;
	  result = std::stoi(str, &lastChar, 10);
	  return lastChar == str.size();
	} 
	catch (std::invalid_argument&) {
	  return false;
	}
	catch (std::out_of_range&) {
	  return false;
	}
}
// Check debug level : level must be : 0,1 or 2
static bool Check_Args_DEBUG_LV(int32_t i) {
	if( 0<=i && i<=2 )
		return true;
	else {
		return false;
	}
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

	bool flag_err = false;
	for (unsigned int i = 0; i < NUM_ROW(DEFINE_ARGS); i++) {

		for (unsigned int j = 0; j < NUM_ROW(DEFINE_ARGS); j++) {
			if(strcmp(args[i][0].c_str(), DEFINE_ARGS[j][0].c_str())==0) {
				switch (j) {
					case 0 : {	// Set PRINT_MONITOR
						int val = ToBool(args[i][1]);
						if(val!=-1)
							PRINT_MONITOR = val;
						else {
							SEED_WARNING << "Config : \"" << args[i][0] << "=" << args[i][1] << "\" is wrong \n";
							flag_err=true;
						}
					} break;
					case 1 : { // Set PRINT_FILE
						int val = ToBool(args[i][1]);
						if(val!=-1)
							PRINT_FILE = val;
						else {
							SEED_WARNING << "Config : \"" << args[i][0] << "=" << args[i][1] << "\" is wrong \n";
							flag_err=true;
						}
					} break;
					case 2 : { // Set DEBUG_LEVEL
						if(ToInt(args[i][1], DEBUG_LEVEL)) {
							if(!Check_Args_DEBUG_LV(DEBUG_LEVEL)) {
								SEED_WARNING << "Config : \"" << args[i][0] << "=" << args[i][1] << "\" out of range \n";
								flag_err=true;
							}
						}
						else {
							SEED_WARNING << "Config : \"" << args[i][0] << "=" << args[i][1] << "\" is wrong \n";
							flag_err=true;
						}
					} break;
					default: break;
				}

				break;
			}
			if(j==NUM_ROW(DEFINE_ARGS)-1) {	// not found in config array
				SEED_WARNING << "Config : \"" << args[i][0] << "=" << args[i][1] << "\" is not config\n";
				flag_err=true;
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



