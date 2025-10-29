// test_udp_socket.h
#ifndef TEST_UDP_SOCKET_H
#define TEST_UDP_SOCKET_H

#include <iostream>
#include <string>
#include <cassert>
#include "Socket.h"

// A simple in-memory mock implementing Socket for unit tests.
// It simulates message delivery between two endpoints via setPeer.
class MockSocket : public Socket {
private:
    bool tcpFlag{false};
    bool connected{false};
    MockSocket* peer{nullptr};
    std::string inbox;
public:
    MockSocket(bool isTCP = false) : tcpFlag(isTCP) {}
    ~MockSocket() override = default;

    // Mark server as ready to accept (no real networking).
    void waitForConnect(int /*port*/) override {
        connected = true;
    }

    // Mark client as connected (no real networking).
    void connect(const std::string & /*host*/, int /*port*/) override {
        connected = true;
    }

    // Link two mock endpoints
    void setPeer(MockSocket* p) { peer = p; }

    // Send places data into peer's inbox if peer exists and connected.
    unsigned int send(const std::string &msg, const sockaddr_in* /*dest*/ = nullptr) override {
        if (!peer || !peer->connected) return 0;
        peer->inbox = msg;
        return static_cast<unsigned int>(msg.size());
    }

    // Receive reads and clears this socket's inbox.
    unsigned int receive(std::string &out, sockaddr_in* /*src*/ = nullptr) override {
        out = inbox;
        unsigned int n = static_cast<unsigned int>(inbox.size());
        inbox.clear();
        return n;
    }

    void shutdown() override {
        connected = false;
    }

    bool isTCP() const override { return tcpFlag; }
};

// Test: morphing from server to client via base pointer and verify send/receive.
inline void test_udp_morph_server_client() {
    MockSocket server(false);
    MockSocket client(false);

    // Link peers both ways to simulate two-way communication
    server.setPeer(&client);
    client.setPeer(&server);

    Socket* serverPtr = &server;
    Socket* clientPtr = &client;

    // Server prepares to accept, client connects
    serverPtr->waitForConnect(12345);
    clientPtr->connect("127.0.0.1", 12345);

    // Client -> Server
    const std::string cmsg = "hello from client";
    unsigned int sent = clientPtr->send(cmsg);
    assert(sent == cmsg.size());
    std::string recvBuf;
    unsigned int recvd = serverPtr->receive(recvBuf);
    assert(recvd == cmsg.size());
    assert(recvBuf == cmsg);

    // Server -> Client
    const std::string smsg = "reply from server";
    sent = serverPtr->send(smsg);
    assert(sent == smsg.size());
    std::string recvBuf2;
    recvd = clientPtr->receive(recvBuf2);
    assert(recvd == smsg.size());
    assert(recvBuf2 == smsg);

    // Shutdown server; further sends should not be delivered
    serverPtr->shutdown();
    const std::string afterShutdown = "after shutdown";
    sent = clientPtr->send(afterShutdown);
    // peer is shut down, send should return 0
    assert(sent == 0);
    recvd = serverPtr->receive(recvBuf);
    assert(recvd == 0 && recvBuf.empty());

    std::cout << "[test_udp_morph_server_client] PASSED\n";
}

// Test: polymorphic usage and switching pointer targets.
inline void test_polymorphic_switching() {
    MockSocket a(false), b(false);
    a.setPeer(&b);
    b.setPeer(&a);

    Socket* ptr = &a;
    ptr->waitForConnect(1111);
    ptr = &b;
    ptr->connect("127.0.0.1", 1111);

    // Use a pointer to a base to send/receive
    Socket* sender = &b;
    Socket* receiver = &a;
    const std::string msg = "polymorphic test";
    unsigned int sent = sender->send(msg);
    assert(sent == msg.size());
    std::string out;
    unsigned int got = receiver->receive(out);
    assert(got == msg.size());
    assert(out == msg);

    std::cout << "[test_polymorphic_switching] PASSED\n";
}

// Run all tests in this header.
inline void run_all_socket_tests() {
    test_udp_morph_server_client();
    test_polymorphic_switching();
    std::cout << "All MockSocket tests PASSED\n";
}

#endif // TEST_UDP_SOCKET_H