#include "NetworkManager.h"
#include "Logger.h"

#include <thread>

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

void NetworkManager::init() {
  this->initSockets();
  this->cleanupSockets();

  this->contact_sock = socket(AF_INET, SOCK_DGRAM, 0);
  this->listen_sock = socket(AF_INET, SOCK_DGRAM, 0);

  if (this->contact_sock == 0 || this->listen_sock == 0) {
    LOG_ERROR("socket create error");
    return;
  }
}

void NetworkManager::setupForP2P(const char *ip, const char *port) {
  this->setServerAddress(ip, port);

  std::thread listen_thread(&NetworkManager::listenForDummy, this);
  this->sendDummyMessage();

  // TODO: if listenForDummy not finish well, process break
  // TODO: add timeout
  listen_thread.join();

  LOG_INFO("Setup for P2P communication done well");
}

void NetworkManager::setServerAddress(const char *ip, const char *port) {
  // set contact's host info
  this->contact_host.sin_family = AF_INET;
  this->contact_host.sin_port = htons(std::atoi(port));
  inet_pton(AF_INET, ip, &this->contact_host.sin_addr);

  // set my host info
  this->listen_host.sin_family = AF_INET;
  this->listen_host.sin_port = htons(this->listen_port);
  this->listen_host.sin_addr.s_addr = INADDR_ANY;

  if (bind(this->listen_sock, (sockaddr *)&this->listen_host,
           sizeof(this->listen_host)) < 0) {
    LOG_ERROR("bind error");
    close(this->listen_sock);
    return;
  }
}

void NetworkManager::sendDummyMessage() const {
  // send dummy message to contact for P2P communication
  const char *message = "";

  sendto(this->contact_sock, message, strlen(message), 0,
         (sockaddr *)&this->contact_host, sizeof(this->contact_host));

  LOG_DEBUG("Send message to contact port: ",
            ntohs(this->contact_host.sin_port));
}

void NetworkManager::listenForDummy() const {
  // listen dummy message from contact for P2P communication using thread
  LOG_DEBUG("Listening on port for dummy message: ",
            ntohs(this->listen_host.sin_port));

  char buf[1];
  sockaddr_in sender_in{};
  socklen_t senderLen = sizeof(sender_in);

  while (true) {
    ssize_t n = recvfrom(this->listen_sock, buf, sizeof(buf), 0,
                         (sockaddr *)&sender_in, &senderLen);

    if (n > 0) {
      LOG_DEBUG("Received dummy message");
      break;
    }
  }
}

void NetworkManager::listen() const {
  LOG_DEBUG("Listening on port: ", ntohs(this->listen_host.sin_port));

  // TODO: accept void data
}

void NetworkManager::closeSocket() const {
#ifdef _WIN32
  closesocket(this->contact_sock);
#else
  close(this->contact_sock);
#endif
}

} // namespace laud