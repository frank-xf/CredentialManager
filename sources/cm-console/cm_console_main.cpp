
#include <iomanip>
#include <iostream>
#include <clocale>
#include <random>
#include <map>

#include "../cm-core/cm_core.h"

int main(int argc, char *argv[])
{


    return 0;
}


void recover()
{

    std::setlocale(LC_ALL, "en_US.utf8");

    xf::credential::CredentialMgr mgr;
    mgr.Load(R"(resources/credential-demo.xml)");
    mgr.Save(R"(outputs/credential-test.xml)");

    xf::credential::string_t str;
    mgr.Serialize(str);

    std::cout << str << std::endl;
}