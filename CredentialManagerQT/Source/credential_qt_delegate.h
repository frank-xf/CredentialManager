#ifndef _bnb_Credential_Delegate_H_
#define _bnb_Credential_Delegate_H_

QT_BEGIN_NAMESPACE

class DelegateType
{
private:

    DelegateType(const DelegateType&) = delete;
    DelegateType& operator=(const DelegateType&) = delete;

protected:

    using id_type = bnb::Credential::id_type;

    DelegateType() = default;

public:

    virtual bool OnAddPlatform(id_type credentialId) = 0;
    virtual bool OnAddAccount(id_type credentialId, id_type platformId) = 0;
    virtual bool OnAddPair(id_type credentialId, id_type platformId, id_type accountId) = 0;

    virtual bool OnUpdatePassword(id_type credentialId) = 0;
    virtual bool OnUpdateCredential(id_type credentialId) = 0;
    virtual bool OnUpdatePlatform(id_type credentialId, id_type platformId) = 0;
    virtual bool OnUpdateAccount(id_type credentialId, id_type platformId, id_type accountId) = 0;
    virtual bool OnUpdatePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) = 0;

    virtual bool OnRemoveCredential(id_type credentialId) = 0;
    virtual bool OnRemovePlatform(id_type credentialId, id_type platformId) = 0;
    virtual bool OnRemoveAccount(id_type credentialId, id_type platformId, id_type accountId) = 0;
    virtual bool OnRemovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) = 0;

    virtual bool OnMovePlatform(id_type credentialId, id_type platformId, int offset) = 0;
    virtual bool OnMoveAccount(id_type credentialId, id_type platformId, id_type accountId, int offset) = 0;
    virtual bool OnMovePair(id_type credentialId, id_type platformId, id_type accountId, id_type pairId, int offset) = 0;

	virtual bool OnSortPlatform(id_type credentialId, int cln, bool ascending) = 0;
	virtual bool OnSortAccount(id_type credentialId, id_type platformId, int cln, bool ascending) = 0;
	virtual bool OnSortPair(id_type credentialId, id_type platformId, id_type accountId, int cln, bool ascending) = 0;

    virtual void SwitchToNode(id_type credentialId) = 0;
    virtual void SwitchToNode(id_type credentialId, id_type platformId) = 0;
    virtual void SwitchToNode(id_type credentialId, id_type platformId, id_type accountId) = 0;
    virtual void SwitchToNode(id_type credentialId, id_type platformId, id_type accountId, id_type pairId) = 0;
    virtual void SwitchToView(unsigned int eType, id_type id) = 0;

    virtual ~DelegateType() = 0 { }

};

QT_END_NAMESPACE

#endif // _bnb_Credential_Delegate_H_

