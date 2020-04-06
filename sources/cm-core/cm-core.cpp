
#include <ctime>
#include "cm_core.h"



namespace xf::credential
{
    void credential_t::Updated(action_type at, credential_type ct)
    {
        time = std::time(nullptr);
    }

    void f()
    {
        CredentialMgr mgr;

        mgr.Traversal([](platform_t& p) {

            p.GetData().type;
            p.Traversal([](account_t& a) {
                        
                a.GetData().type;
                a.Traversal([](pair_t& p) {

                    p.GetData().type;

                            });
                        
                        });

                      });
    }

}   // namespace xf::credential