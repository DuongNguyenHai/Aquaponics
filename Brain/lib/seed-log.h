// Nguyen Hai Duong
// Oct 29 2016

#ifndef SEED_LOG_H
#define SEED_LOG_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <inttypes.h>	// int64_t
#include <stdlib.h>		// exit()

namespace TREE {

extern const char *g_program_name;

#define PRINT_MONITOR true  // print infor on terminal
#define PRINT_FILE true     // print infor to file (log file)

struct LogMessageEnvelope {
  enum Severity {
    Error = -2,
    Warning = -1,
    Info = 0,
    Verbose = 1
  };
  // An 'enum Severity' value, or a positive number indicating verbosity level.
  int severity;
  const char *func;
  const char *file;
  const char *log;
  int logLevel;
  int32_t line;
};

class MessageLogger {

public:

	MessageLogger(LogMessageEnvelope::Severity severity, const char *func,
	              const char *file, int32_t line, const char *LOGFILE, int DEBUG_LEVEL);
	~MessageLogger();
	inline std::ostream &stream() { return ss_; }

private:
	LogMessageEnvelope envelope_;
	std::ostringstream ss_;
  
};

#define SEED_ERROR \
	MessageLogger(LogMessageEnvelope::Error, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE, DEBUG_LEVEL).stream()
#define SEED_WARNING \
	MessageLogger(LogMessageEnvelope::Warning, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE, DEBUG_LEVEL).stream()
#define SEED_LOG \
	MessageLogger(LogMessageEnvelope::Info, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE, DEBUG_LEVEL).stream()
#define SEED_VLOG \
	MessageLogger(LogMessageEnvelope::Verbose, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE, DEBUG_LEVEL).stream()

}

#endif