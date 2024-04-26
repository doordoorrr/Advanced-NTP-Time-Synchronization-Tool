#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define NTP_SERVER "pool.ntp.org"
#define NTP_PORT 123
#define NTP_PACKET_SIZE 48
#define NTP_TIMESTAMP_DELTA 2208988800ull

typedef struct {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint32_t ref_ts_sec;
    uint32_t ref_ts_frac;
    uint32_t orig_ts_sec;
    uint32_t orig_ts_frac;
    uint32_t recv_ts_sec;
    uint32_t recv_ts_frac;
    uint32_t trans_ts_sec;
    uint32_t trans_ts_frac;
} ntp_packet;

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    ntp_packet packet;

    // Open socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Resolve NTP server address
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    int ret = getaddrinfo(NTP_SERVER, NULL, &hints, &result);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    // Copy the resolved address to serv_addr
    memcpy(&serv_addr, result->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(result);

    // Set server port
    serv_addr.sin_port = htons(NTP_PORT);

    // Prepare NTP packet
    memset(&packet, 0, sizeof(packet));
    packet.li_vn_mode = (0x03 << 6) | (0x03 << 3) | 0x03;

    // Send NTP request
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR sending packet");

    // Receive NTP response
    n = recv(sockfd, &packet, sizeof(packet), 0);
    if (n < 0)
        error("ERROR receiving response");

    // Close socket
    close(sockfd);

    // Get NTP time
    time_t ntp_time = (ntohl(packet.trans_ts_sec) - NTP_TIMESTAMP_DELTA);
    printf("NTP time: %s", ctime(&ntp_time));

    // Get local time
    time_t local_time = time(NULL);
    printf("Local time: %s", ctime(&local_time));

    // Calculate and print time difference
    double time_diff = difftime(local_time, ntp_time);
    printf("Time difference: %.2f seconds\n", time_diff);

    return 0;
}
