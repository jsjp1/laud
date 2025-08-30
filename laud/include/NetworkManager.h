#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace laud {

class NetworkManager {
public:
  NetworkManager() = default;
  ~NetworkManager() = default;

  void init();
  void setupForP2P(const char *ip, const char *port);
  void listen() const;
  void closeSocket() const;

private:
  void setServerAddress(const char *ip, const char *port);
  void sendDummyMessage() const;
  void listenForDummy() const;

private:
  bool initSockets() const;
  void cleanupSockets() const;

  const int listen_port = 8002;
  int contact_sock;
  int listen_sock;
  sockaddr_in contact_host;
  sockaddr_in listen_host;
};

} // namespace laud

#endif