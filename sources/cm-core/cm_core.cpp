#include <ctime>
#include "cm_core.h"


namespace xf::credential
{
    void credential_t::Updated(action_type at, credential_type ct)
    {
        time = std::time(nullptr);
    }


    bool CredentialMgr::Serialize(memory_t& mem) const
    {
        return false;
    }

    bool CredentialMgr::Deserialize(const memory_t& mem)
    {
        return false;
    }

    bool CredentialMgr::Load(const char* file)
    {
        return false;
    }

    bool CredentialMgr::Save(const char* file)
    {
        return false;
    }

    bool CredentialMgr::ValidateName(const string_t& strName)
    {
        return false;
    }

    bool CredentialMgr::Encoding(memory_t& mt, const byte_t* key, std::size_t n)
    {
        return false;
    }

    bool CredentialMgr::Decoding(memory_t& mt, const byte_t* key, std::size_t n)
    {
        return false;
    }

    bool CredentialMgr::Check(const char* file)
    {
        return false;
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