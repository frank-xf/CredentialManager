#ifndef _bnb_Credential_Delegate_H_
#define _bnb_Credential_Delegate_H_

QT_BEGIN_NAMESPACE

class delegate_type
{
private:

    delegate_type(const delegate_type&) = delete;
    delegate_type& operator=(const delegate_type&) = delete;

protected:

    delegate_type() = default;

public:

    virtual bool OnAddCredential() { return false; }
    virtual bool OnAddPlatform(unsigned int id1) { return false; }
    virtual bool OnAddAccount(unsigned int id1, unsigned int id2) { return false; }
    virtual bool OnAddProperty(unsigned int id1, unsigned int id2, unsigned int id3) { return false; }

    virtual bool OnUpdatePassword(unsigned int id1) { return false; }
    virtual bool OnUpdateCredential(unsigned int id1) { return false; }

    virtual bool OnUpdatePlatform(unsigned int id1, unsigned int id2) { return false; }
    virtual bool OnUpdateAccount(unsigned int id1, unsigned int id2, unsigned int id3) { return false; }
    virtual bool OnUpdateProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) { return false; }

    virtual bool OnRemoveCredential(unsigned int id1) { return false; }
    virtual bool OnRemovePlatform(unsigned int id1, unsigned int id2) { return false; }
    virtual bool OnRemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3) { return false; }
    virtual bool OnRemoveProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4) { return false; }

    virtual bool SwitchNode(unsigned int eType, unsigned int id) { return false; }
    virtual void OnClickedNew() { }
    virtual void OnClickedOpen() { }
    virtual void OnClickedAbout() { }

    virtual ~delegate_type() = 0 { }

};  // class delegate_type

QT_END_NAMESPACE

#endif // _bnb_Credential_Delegate_H_

