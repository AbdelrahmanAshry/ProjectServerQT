// server_channel.cpp
#include "Server_Channel.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <cstdio>

static inline std::string trim(const std::string &s) {
    size_t b = 0;
    while (b < s.size() && std::isspace((unsigned char)s[b])) ++b;
    size_t e = s.size();
    while (e > b && std::isspace((unsigned char)s[e-1])) --e;
    return s.substr(b, e - b);
}

void ServerChannel::start() {
    std::cout << "[Server] Starting..." << std::endl;
    state = RUNNING;
    // ask the polymorphic socket to prepare for connections on bindPort
    if (channelSocket) channelSocket->waitForConnect(bindPort);
}

void ServerChannel::stop() {
    std::cout << "[Server] Stopping..." << std::endl;
    state = STOPPED;
    if (channelSocket) channelSocket->shutdown();
}

void ServerChannel::send(const std::string& message) {
    if (channelSocket) channelSocket->send(message);
}

void ServerChannel::receive() {
    // Compatibility: call tick once to process incoming data
    tick();
}

void ServerChannel::tick() {
    if (state != RUNNING) return;
    if (!channelSocket) return;

    std::string in;
    unsigned int n = channelSocket->receive(in);
    if (n == 0) {
        // no data available
        return;
    }

    // Support possibly multiple lines in the buffer; split on newline
    size_t pos = 0;
    while (pos < in.size()) {
        size_t nl = in.find('\n', pos);
        std::string line;
        if (nl == std::string::npos) {
            line = in.substr(pos);
            pos = in.size();
        } else {
            line = in.substr(pos, nl - pos);
            pos = nl + 1;
        }
        line = trim(line);
        if (line.empty()) continue;

        std::string reply = handleCommand(line);
        if (!reply.empty()) {
            if (reply.back() != '\n') reply.push_back('\n');
            channelSocket->send(reply);
        }
    }
}

std::string ServerChannel::handleCommand(const std::string& cmd) {
    const std::string c = trim(cmd);
    if (c == "CONNECT") {
        return std::string("CONNECTED");
    }

    const std::string setPref = "SET_THRESHOLD:";
    if (c.rfind(setPref, 0) == 0) {
        std::string num = trim(c.substr(setPref.size()));
        try {
            int val = std::stoi(num);
            threshold = val;
            return std::string("ACK");
        } catch (...) {
            return std::string("ERR");
        }
    }

    if (c == "GET_TEMP") {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "TEMP:%.1f", lastTemp);
        return std::string(buf);
    }

    return std::string("ERR_UNKNOWN");
}
