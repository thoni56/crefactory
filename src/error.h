#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

typedef enum {
    RC_OK = 0,
    RC_PIPE_CREATION_ERROR = -100,
    RC_FORK_FAILED = -101,
    RC_WRITING_TO_SERVER = -102,
    RC_READING_FROM_SERVER = -103,
    RC_WRITING_TO_CLIENT = -104,
    RC_READING_FROM_CLIENT = -105,
    RC_BROKEN_INPUT_FROM_CLIENT = -106,
    // Options handling
    RC_UNKNOWN_OPTION = -200,
    RC_NO_MODE_SELECTED = -201
} ResultCode;

#endif
