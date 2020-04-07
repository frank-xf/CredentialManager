#include "../cm-core/cm_core.h"


int main()
{
    xf::credential::CredentialMgr mgr;

    mgr.Load(R"(E:\Workspace\Practice\Test\fb.xml)");
    mgr.Save(R"(E:\Workspace\Practice\Test\fb-new.xml)");





    return 0;
}
