
#include <iostream>
#include <clocale>

#include "../cm-core/cm_core.h"

int main(int argc, char *argv[])
{
    // std::setlocale(LC_ALL, "en_US.utf8");

    xf::credential::memory_t data;
    xf::credential::LoadFile(R"(resources/credential-demo.xml)", data);

    std::cout << data << std::endl;
}
