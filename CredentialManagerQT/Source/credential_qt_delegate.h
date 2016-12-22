#ifndef _bnb_Credential_Delegate_H_
#define _bnb_Credential_Delegate_H_

QT_BEGIN_NAMESPACE

class DelegateMainView
{
private:

    DelegateMainView(const DelegateMainView&) = delete;
    DelegateMainView& operator=(const DelegateMainView&) = delete;

protected:

    using id_type = unsigned int;

    DelegateMainView() = default;

public:

    // virtual bool OnAddCredential() { return false; }
    virtual bool OnAddPlatform(id_type id1) { return false; }
    virtual bool OnAddAccount(id_type id1, id_type id2) { return false; }
    virtual bool OnAddPair(id_type id1, id_type id2, id_type id3) { return false; }

    virtual bool OnUpdatePassword(id_type id1) { return false; }
    virtual bool OnUpdateCredential(id_type id1) { return false; }
    virtual bool OnUpdatePlatform(id_type id1, id_type id2) { return false; }
    virtual bool OnUpdateAccount(id_type id1, id_type id2, id_type id3) { return false; }
    virtual bool OnUpdatePair(id_type id1, id_type id2, id_type id3, id_type id4) { return false; }

    virtual bool OnRemoveCredential(id_type id1) { return false; }
    virtual bool OnRemovePlatform(id_type id1, id_type id2) { return false; }
    virtual bool OnRemoveAccount(id_type id1, id_type id2, id_type id3) { return false; }
    virtual bool OnRemovePair(id_type id1, id_type id2, id_type id3, id_type id4) { return false; }

    virtual bool OnMovePlatform(id_type id1, id_type id2, int offset) { return false; }
    virtual bool OnMoveAccount(id_type id1, id_type id2, id_type id3, int offset) { return false; }
    virtual bool OnMovePair(id_type id1, id_type id2, id_type id3, id_type id4, int offset) { return false; }

	virtual bool OnSortPlatform(id_type id1, int cln, bool ascending) { return false; }
	virtual bool OnSortAccount(id_type id1, id_type id2, int cln, bool ascending) { return false; }
	virtual bool OnSortPair(id_type id1, id_type id2, id_type id3, int cln, bool ascending) { return false; }

    virtual bool SwitchNode(unsigned int eType, id_type id) { return false; }
    virtual void OnClickedNew() { }
    virtual void OnClickedOpen() { }
    virtual void OnClickedAbout() { }

    virtual ~DelegateMainView() = 0 { }

};  // class DelegateMainView

class DelegateTableView
{
private:

    DelegateTableView(const DelegateTableView&) = delete;
    DelegateTableView& operator=(const DelegateTableView&) = delete;

protected:

    DelegateTableView() = default;

public:

    virtual void OnAdd() = 0;
    virtual void OnEdit(unsigned int id) = 0;
    virtual void OnRemove(unsigned int id) = 0;
    virtual void OnMove(unsigned int id, int offset) = 0;
	virtual void OnSort(int cln, bool ascending) = 0;

    virtual ~DelegateTableView() = 0 { }

};  // class DelegateTableView

QT_END_NAMESPACE

#endif // _bnb_Credential_Delegate_H_

