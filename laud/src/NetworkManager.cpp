#include "NetworkManager.h"

namespace laud {

bool NetworkManager::initSockets() const {
#ifdef _WIN32
  WSADATA wsa_data;
  return WSAStartup(MAKEWORD(2, 2), &swa_data) == 0;
#else
  return true;
#endif
}

void NetworkManager::cleanupSockets() const {
#ifdef _WIN32
  WSACleanup();
#endif
}

void NetworkManager::init() const {
  this->initSockets();
  this->cleanupSockets();
}

void NetworkManager::closeSocket(int sock) const {
#ifdef _WIN32
  closesocket(sock);
#else
  close(sock);
#endif
}

} // namespace laud