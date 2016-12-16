#ifndef _bnb_Credential_Delegate_H_
#define _bnb_Credential_Delegate_H_

QT_BEGIN_NAMESPACE

class delegate_model_type
{
private:

    delegate_model_type(const delegate_model_type&) = delete;
    delegate_model_type& operator=(const delegate_model_type&) = delete;

protected:

    delegate_model_type() = default;

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

    virtual bool SwitchNode(unsigned int id) { return false; }

    virtual ~delegate_model_type() = 0 { }

};  // class delegate_model_type

class delegate_ui_type
{
private:

    delegate_ui_type(const delegate_ui_type&) = delete;
    delegate_ui_type& operator=(const delegate_ui_type&) = delete;

protected:

    delegate_ui_type() = default;

public:

    virtual bool OnClickedNew() { return false; }
    virtual bool OnClickedOpen(unsigned int id1) { return false; }
    virtual bool OnClickedAbout(unsigned int id1, unsigned int id2) { return false; }

    virtual ~delegate_ui_type() = 0 { }

};  // class delegate_ui_type

QT_END_NAMESPACE

#endif // _bnb_Credential_Delegate_H_

