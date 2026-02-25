#pragma once
#include <cstdlib>
#include <signal.h>
#include <string>
#include <unistd.h>

class TestHttpServer
{
    pid_t pid_ = -1;

  public:
    TestHttpServer(int port, const std::string& directory)
    {
        pid_ = fork();
        if (pid_ == 0)
        {
            execlp("python3", "python3", "-m", "http.server", std::to_string(port).c_str(),
                   "--directory", directory.c_str(), nullptr);
            std::exit(1); // If exec fails
        }
        sleep(1); // allow server to start
    }

    ~TestHttpServer()
    {
        if (pid_ > 0)
        {
            kill(pid_, SIGTERM);
        }
    }
};
