#pragma once

#include "cm_type.h"

namespace xf::credential
{
    inline const char* version() { return "1.2.0"; }

    using string_t = std::string;

    struct pair_item : public credential_t {
        string_t key, value;
        pair_item() : credential_t(credential_type::ct_pair) { }
    };

    struct account_item : public credential_t {
        string_t account, description;
        account_item() : credential_t(credential_type::ct_account) { }
    };

    struct platform_item : public credential_t {
        string_t name, url, description;
        platform_item() : credential_t(credential_type::ct_platform) { }
    };

    struct credential_item : public credential_t {
        string_t username, version;
        credential_item() : credential_t(credential_type::ct_credential) { }
    };

    struct pair_t : public node_t<pair_item, pair_t> {
        using base_type::base_type;
    };

    struct account_t : public node_t<account_item, account_t>, public list_t<pair_t> {
        using base_type::base_type;
    };

    struct platform_t : public node_t<platform_item, platform_t>, public list_t<account_t> {
        using base_type::base_type;
    };

    class CredentialMgr : public credential_item, public list_t<platform_t>
    {






    };  // class CredentialMgr

}   // namespace xf::credential
