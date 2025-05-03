#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TEMP_FILE "/tmp/heredoc_temp.txt"

void heredoc_child(const char *delimiter) {
    char *line;
    int fd = open(TEMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        exit(1);

    signal(SIGINT, SIG_DFL); // Let Ctrl+C terminate the child

    while (1) {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }
        dprintf(fd, "%s\n", line);
        free(line);
    }
    close(fd);
    exit(0);
}

int handle_heredoc(const char *delimiter) {
    pid_t pid;
    int status;

    signal(SIGINT, SIG_IGN); // Parent ignores SIGINT during heredoc

    pid = fork();
    if (pid == 0)
        heredoc_child(delimiter);

    waitpid(pid, &status, 0);

    signal(SIGINT, SIG_DFL); // Restore SIGINT in parent

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
        // Ctrl+C was pressed
        printf("\n");
        return 1; // Indicate interruption
    }
    return 0; // Success
}

int main() {
    if (handle_heredoc("END")) {
        printf("Heredoc interrupted by SIGINT.\n");
    } else {
        printf("Heredoc completed.\n");
    }
    return 0;
}
