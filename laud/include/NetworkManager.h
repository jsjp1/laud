#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

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

  void init() const;
  void closeSocket(int sock) const;

private:
  bool initSockets() const;
  void cleanupSockets() const;
};

} // namespace laud

#endif