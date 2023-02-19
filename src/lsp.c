#include "lsp.h"
#include "error.h"
#include "process.h"
#include "server_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void lsp_init(void) {}

void init_server(int input_pipe[], int output_pipe[]) {
    char buffer[1000];

    // Write data to the LSP server
    if (write(input_pipe[1], "{\"jsonrpc\":\"2.0\",\"method\":\"initialize\",\"params\":{}}\r\n",
              54) < 0) {
        perror("Error writing to input pipe");
        exit(1);
    }

    // Read data from the LSP server
    if (read(output_pipe[0], buffer, 256) < 0) {
        perror("Error reading from output pipe");
        exit(1);
    }

    printf("Response from LSP server: %s", buffer);
}

ResultCode lsp_inject(const char *program_name, int input_pipe[], int output_pipe[]) {
    pid_t pid;

    // Create input and output pipes
    if (pipe(input_pipe) < 0 || pipe(output_pipe) < 0) {
        return RC_PIPE_CREATION_ERROR;
    }

    // Fork a new process to handle the LSP server
    pid = fork_process();
    if (pid < 0) {
        return RC_FORK_FAILED;
    } else if (pid == 0) {
        // Child process
        launch_server(input_pipe, output_pipe, program_name);
    } else {
        // Parent process
        close(input_pipe[0]);  // Close unused read end of input pipe
        close(output_pipe[1]); // Close unused write end of output pipe
    }
    return RC_OK;
}

int lsp_listener(FileTable fileTable, CXIndex index, int client_input_pipe,
                 int client_output_pipe) {
    char buffer[1000];
    int nbytes = read(client_input_pipe, buffer, 1000);
    if (nbytes == -1) {
        perror("client read");
    }
    write(client_output_pipe, buffer, strlen(buffer));
    return EXIT_SUCCESS;
}
