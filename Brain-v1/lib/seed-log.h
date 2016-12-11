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

#include "seed-config.h" // configuration for system

namespace TREE {

#define LOGFILE "/home/pi/AquaOs/log/root.log"  // define a log-file for system. its may be redefined to log in specific log-file in each prog.
// #define ABSLOGFILE "../log/root.log"

extern const char *g_program_name;

char *DateAndTime(time_t rawtime);

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
	              const char *file, int32_t line, const char *logFile);
	~MessageLogger();
	inline std::ostream &stream() { return ss_; }

private:
	LogMessageEnvelope envelope_;
	std::ostringstream ss_;
  
};

#define SEED_ERROR \
	MessageLogger(LogMessageEnvelope::Error, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE).stream()
#define SEED_WARNING \
	MessageLogger(LogMessageEnvelope::Warning, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE).stream()
#define SEED_LOG \
	MessageLogger(LogMessageEnvelope::Info, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE).stream()
#define SEED_VLOG \
	MessageLogger(LogMessageEnvelope::Verbose, \
                         __FUNCTION__, __FILE__, __LINE__, LOGFILE).stream()

} // end of namespace TREE

#endif