### Function LOG

There are 4 types of print log :

*	Print LOG : print a normal logs.
	SEED_LOG << "print something ...";

*	Print WARNING : print a warning and something's not right but it's not harmful for system
	SEED_WARNING << "print something ...";

*	Print ERROR : print error what can harmful for system. ex : some errors can terminate system if it was not in controled.
	SEED_ERROR << "print something ...";

*	Print VERBOSE : print LOG in detail.
	SEED_VLOG << "print something ...";

### CONFIG FOR PRINT-LOG

Choose print on terminal and log file :

```objc

-	#define PRINT_MONITOR true  // print infor on terminal
-	#define PRINT_FILE true     // print infor to file (log file)

```
When you wanna print log to file. Let put the #define below

	#define LOGFILE "text.log" // all logs will print in text.log

in header file. ex: database wanna print log file. put that #define in database.h

Control what type of log will print. put #define below in header file.

```objc

	#ifndef DEBUG_LEVEL
	#define DEBUG_LEVEL 1
	#else
	#undef DEBUG_LEVEL
	#define DEBUG_LEVEL 1
	#endif

```
There are 3 level of DEBUG_LEVEL from 0->2. Higher level will debug more information.
-  	DEBUG_LEVEL 0 means : show WARNING, ERROR but no LOG.
-	DEBUG_LEVEL 1 means : show WARNING, ERROR and LOG.
-	DEBUG_LEVEL 2 means : show all.
