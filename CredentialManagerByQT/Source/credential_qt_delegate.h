#ifndef _bnb_Credential_QT_Delegate_H_
#define _bnb_Credential_QT_Delegate_H_

namespace delegate_ns
{
    struct account_delegate
    {
        virtual bool OnViewCredential(bnb::account_type* pa) = 0;
        virtual bool OnEditAccount(bnb::account_type* pa) = 0;
        virtual bool OnRemoveAccount(bnb::account_type* pa) = 0;

        virtual ~account_delegate() { }
    };

    struct platform_delegate
    {
        virtual bool OnEditPlatform(bnb::platform_type* pp) = 0;
        virtual bool OnRemovePlatform(bnb::platform_type* pp) = 0;
        virtual bool OnViewCredential(bnb::platform_type* pp) = 0;
        virtual bool OnAddAccount(bnb::platform_type* pp) = 0;
        virtual bool OnAddPlatform() = 0;

        virtual ~platform_delegate() { }
    };

    struct credential_delegate
    {
        virtual bool OnAddPlatform() = 0;
        virtual bool OnAddAccount(bnb::platform_type* pp) = 0;
        virtual bool OnRemovePlatform(bnb::platform_type* pp) = 0;
        virtual bool OnRemoveAccount(bnb::platform_type* pp, bnb::account_type* pa) = 0;
        virtual bool OnEditPlatform(bnb::platform_type* pp) = 0;
        virtual bool OnEditAccount(bnb::platform_type* pp, bnb::account_type* pa) = 0;
        virtual bool OnViewCredential(bnb::platform_type* pp, bnb::account_type* pa) = 0;

        virtual ~credential_delegate() { }
    };

    struct validate_delegate
    {
        virtual bool CanUpdate(const bnb::platform_type& platform) = 0;
        virtual bool CanUpdate(const bnb::platform_type& platform, const bnb::account_type& account) = 0;
        virtual bool ValidateUserName(const bnb::string_type& username) = 0;
        virtual bool ValidatePassword(const bnb::string_type& password) = 0;

        virtual ~validate_delegate() { }
    };
}

#endif  // _bnb_Credential_QT_Delegate_H_
