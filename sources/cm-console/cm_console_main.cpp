
#include <iomanip>
#include <iostream>
#include <clocale>
#include <random>
#include <map>

#include "../cm-core/cm_core.h"

int main(int argc, char *argv[])
{
    std::random_device rd;

    std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 e(seed);
    std::normal_distribution<double> normal_dist(20,);

    std::map<int, int> hist;
    for (int n = 0; n < 10000; ++n) {
        ++hist[std::lround(normal_dist(e))];
    }

    for (auto p : hist) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
            << p.first << ' ' << std::string(p.second, '*') << '\n';
    }

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