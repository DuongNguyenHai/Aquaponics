// Nguyen Hai Duong
// Oct 29 2016

#include "seed-log.h"

namespace TREE {

const char *g_program_name = NULL;
// If the program name was set (g_program_name != ""), the function
// GetProgramName returns the program name (without the path) followed by a
// colon, e.g. "gmm-align:".  Otherwise it returns the empty string "".
const char *GetProgramName() {
  return g_program_name == NULL ? "" : g_program_name;
}
// Given a filename like "/a/b/c/d/e/f.cc",  GetShortFileName
// returns "e/f.cc".  Does not currently work if backslash is
// the filename separator.
const char *GetShortFileName(const char *filename) {
	const char *last_slash = strrchr(filename, '/');
	if (!last_slash) {
		return filename;
	} else {
		while (last_slash > filename && last_slash[-1] != '/')
			last_slash--;
		return last_slash;
	}
}
// Print log/error/warning/verbose on terminal an log file
static void SendToLog(	const LogMessageEnvelope &envelope,
						const char *pfile,
                      	const char *message) {

	// Otherwise, use Kaldi default logging.
	std::stringstream header;
	if (envelope.severity == LogMessageEnvelope::Info)
		header << "LOG (";
	else if (envelope.severity == LogMessageEnvelope::Warning)
		header << "WARNING (";
	else if (envelope.severity == LogMessageEnvelope::Error)
		header << "ERROR (";
	else
		header << "VERBOSE (";
	
	header << GetProgramName() << envelope.func << "():"
	     << envelope.file << ':' << envelope.line << ")";

	std::string header_str = header.str();

	if (PRINT_MONITOR)
		printf("%s %s\n", header_str.c_str(), message);

	if (PRINT_FILE) {
		std::ofstream logFile(pfile, std::ios::out | std::ios::app);
		// printf("What the hell is going on\n");
		if (logFile.is_open()) {
			logFile << header_str.c_str() << " " << message << std::endl;
			logFile.close();
		} else {
			printf("Error open file: \"%s\" \n", pfile);
		}
	}

}

MessageLogger::MessageLogger(LogMessageEnvelope::Severity severity,
                             const char *func, const char *file, int32_t line, const char *logFile) {
	// Obviously, we assume the strings survive the destruction of this object.
	envelope_.severity = severity;
	envelope_.func = func;
	envelope_.file = GetShortFileName(file);  // Pointer inside 'file'.
	envelope_.line = line;
	envelope_.log = logFile;
	envelope_.logLevel = DEBUG_LEVEL;
}

MessageLogger::~MessageLogger() {

	std::string str = ss_.str();
	while (!str.empty() && str[str.length() - 1] == '\n')
		str.resize(str.length() - 1);

	// Print error and exit if program got error message
	if(envelope_.severity == LogMessageEnvelope::Error) {
		SendToLog(envelope_, envelope_.log, str.c_str());
		exit(EXIT_FAILURE);
	}

	// Log corresponds to DEBUG_LEVEL
	switch(envelope_.logLevel) {
		case 1: {
			if(envelope_.severity != LogMessageEnvelope::Verbose)
				SendToLog(envelope_, envelope_.log, str.c_str());
		} break;
		case 2: {
			SendToLog(envelope_, envelope_.log, str.c_str());
		} break;
		default:break;
	}

}

} // end of namspace TREE