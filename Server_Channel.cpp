// server_channel.cpp
#include "Server_Channel.h"
#include <algorithm>
#include <cctype>
#include <iostream>

static inline std::string trim(const std::string &s) {
    size_t b = 0;
    while (b < s.size() && std::isspace((unsigned char)s[b])) ++b;
    size_t e = s.size();
    while (e > b && std::isspace((unsigned char)s[e-1])) --e;
    return s.substr(b, e - b);
}

ServerChannel::State stringToState(const std::string&) { return ServerChannel::IDLE; }

void ServerChannel::start() {
    std::cout << "[Server] Starting..." << std::endl;
    state = RUNNING;
    // ask the polymorphic socket to prepare for connections on bindPort
    channelSocket->waitForConnect(bindPort);
}

void ServerChannel::stop() {
    std::cout << "[Server] Stopping..." << std::endl;
    state = STOPPED;
    channelSocket->shutdown();
}

void ServerChannel::send(const std::string& message) {
    channelSocket->send(message);
}

void ServerChannel::receive() {
    // Compatibility: call tick once to process incoming data
    tick();
}

void ServerChannel::tick() {
    if (state != RUNNING) return;

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
            // ensure newline-terminated reply for clients
            if (reply.back() != '\n') reply.push_back('\n');
            channelSocket->send(reply);
        }
    }
}

std::string ServerChannel::handleCommand(const std::string& cmd) {
    // Commands:
    // CONNECT -> "CONNECTED"
    // SET_THRESHOLD:<n> -> "ACK" and store threshold
    // GET_TEMP -> "TEMP:<value>"
    const std::string c = trim(cmd);
    if (c == "CONNECT") {
        return std::string("CONNECTED");
    }
    const std::string setPref = "SET_THRESHOLD:";
    if (c.rfind(setPref, 0) == 0) {
        std::string num = c.substr(setPref.size());
        try {
            int val = std::stoi(trim(num));
            threshold = val;
            return std::string("ACK");
        } catch (...) {
            return std::string("ERR");
        }
    }
    if (c == "GET_TEMP") {
        // For now return the stored lastTemp (mocked). Format with one decimal.
        char buf[64];
        snprintf(buf, sizeof(buf), "TEMP:%.1f", lastTemp);
        return std::string(buf);
    }

    // Unknown command
    return std::string("ERR_UNKNOWN");
}
