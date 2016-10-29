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

	#if PRINT_MONITOR
		printf("%s %s\n", header_str.c_str(), message);
	#endif

	#if PRINT_FILE
		std::ofstream logFile(pfile, std::ios::out | std::ios::app);
		if (logFile.is_open()) {
			logFile << header_str.c_str() << " " << message << std::endl;
			logFile.close();
		}
	#endif

}

MessageLogger::MessageLogger(LogMessageEnvelope::Severity severity,
                             const char *func, const char *file, int32_t line, const char *LOGFILE, int DEBUG_LEVEL) {
	// Obviously, we assume the strings survive the destruction of this object.
	envelope_.severity = severity;
	envelope_.func = func;
	envelope_.file = GetShortFileName(file);  // Pointer inside 'file'.
	envelope_.line = line;
	envelope_.log = LOGFILE;
	envelope_.logLevel = DEBUG_LEVEL;
}

MessageLogger::~MessageLogger() {

	std::string str = ss_.str();
	while (!str.empty() && str[str.length() - 1] == '\n')
		str.resize(str.length() - 1);

	if(envelope_.severity == LogMessageEnvelope::Verbose) {
		if(envelope_.logLevel > 1)
			SendToLog(envelope_, envelope_.log, str.c_str());
	}else if((envelope_.logLevel > 0) ||
			 (envelope_.severity == LogMessageEnvelope::Warning) ||
			 (envelope_.severity == LogMessageEnvelope::Error) ) {

				SendToLog(envelope_, envelope_.log, str.c_str());
			}
}

}