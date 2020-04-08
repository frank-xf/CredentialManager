
#include <iostream>
#include <clocale>

#include "../cm-core/cm_core.h"

int main()
{
    std::setlocale(LC_ALL, "en_US.utf8");

    xf::credential::memory_t data;
    xf::credential::LoadFile(R"()", data);

    std::cout << "string-> " << data << std::endl;
}
