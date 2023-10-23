#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>

int main() {
    mqd_t mq;
    struct mq_attr attr;
    char buffer[1024];
    unsigned int prio = 1;
    ssize_t bytes_read;

    // Create the message queue
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    mq_unlink("/my_queue"); // Remove the queue if it already exists
    mq = mq_open("/my_queue", O_CREAT | O_RDWR, 0666, &attr);

    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    printf("Enter a message: ");
    scanf("%1023s", buffer);  // Read user input, limit to 1023 characters

    // Send the user input as a message
    mq_send(mq, buffer, strlen(buffer), prio);

    // Receive a message
    bytes_read = mq_receive(mq, buffer, sizeof(buffer), &prio);
    buffer[bytes_read] = '\0'; // Null-terminate the received message

    printf("Received message: %s\n", buffer);

    mq_close(mq);
    mq_unlink("/my_queue");

    return 0;
}
