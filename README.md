# csc2770-prog-challenge-3-adburchfie42
Advanced NTP Time Synchronization Tool

This repository hosts a sophisticated C program engineered to synchronize the local system time with a designated Network Time Protocol (NTP) server. Through intricate socket programming methodologies, this tool establishes a UDP socket connection to the renowned NTP server "pool.ntp.org" and orchestrates the exchange of precisely formatted NTP packets. Employing the renowned getaddrinfo() function for dynamic address resolution, the program adeptly navigates network intricacies to ensure seamless communication with the NTP server.

Upon receiving the NTP response, the program meticulously extracts the transmitted timestamp, compensates for NTP protocol specifics, and computes the accurate time differential between the local system clock and the NTP time source. Robust error handling mechanisms safeguard against network anomalies, while meticulous attention to protocol intricacies guarantees the reliability and precision of time synchronization.
