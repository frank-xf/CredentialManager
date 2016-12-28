#include <iostream>
#include <string>

#include "../CredentialManager/Credential/Credential.h"

void fun1();
void fun2();

class TypeA {
public:
    TypeA() = default;
    TypeA(int m) : a(m) {}
    TypeA(int m, int n) : a(m), b(n) {}
    TypeA(int m, int n, int x) : a(m), b(n), c(x) {}

    void Show() {
        std::cout << a << " " << b << " " << c << std::endl;
    }

    int a{1};
    int b{2};
    int c{3};
};

int sub(int a, int b) { return a - b; }

int main()
{
	//fun1();
    /*
    TypeA a;
    TypeA b(10);
    TypeA c(10, 20);
    TypeA d(10, 20, 30);

    a.Show();
    b.Show();
    c.Show();
    d.Show();

    */

    return (0);
}

void fun1()
{
	/*
	bnb::Credential bc;

	auto ptr1 = bc->Insert(bnb::platform_type("platform1", bnb::platform_data("http://")));
	auto pacc1 = ptr1->m_Value.Insert(bnb::account_type("account3"));
	auto pacc2 = ptr1->m_Value.Insert(bnb::account_type("account2"));

	pacc1->m_Value.Insert(bnb::string_type("k1"))->m_Value = "v1";
	pacc1->m_Value.Insert(bnb::string_type("k2"))->m_Value = "v2";
	pacc2->m_Value.Insert(bnb::string_type("k1"))->m_Value = "v1";
	pacc2->m_Value.Insert(bnb::string_type("k4"))->m_Value = "v4";
	pacc2->m_Value.Insert(bnb::string_type("k3"))->m_Value = "v3";

	auto ptr2 = bc->Insert(bnb::platform_type("platform2", bnb::platform_data("http://")));
	auto pacc3 = ptr2->m_Value.Insert(bnb::account_type("account7"));
	auto pacc4 = ptr2->m_Value.Insert(bnb::account_type("account6"));
	auto pacc5 = ptr2->m_Value.Insert(bnb::account_type("account5"));

	pacc3->m_Value.Insert(bnb::string_type("k1"))->m_Value = "v1";
	pacc3->m_Value.Insert(bnb::string_type("k5"))->m_Value = "v5";
	pacc3->m_Value.Insert(bnb::string_type("k3"))->m_Value = "v3";
	pacc5->m_Value.Insert(bnb::string_type("k7"))->m_Value = "v7";
	pacc5->m_Value.Insert(bnb::string_type("k2"))->m_Value = "v2";
	pacc4->m_Value.Insert(bnb::string_type("k1"))->m_Value = "v1";

	bnb::memory_type mt;

	bc.UpdateTime();
	std::cout << bc.Save(mt) << std::endl;

	std::cout << mt.c_str() << std::endl << "-----";
	*/
	// bnb::Encoding(mt, (const unsigned char*)bc.GetWord().c_str(), bc.GetWord().size());

	const char strText[] = R"(<?xml version="1.0" encoding="UTF-8"?>
<credential user="just a name" time="1466185402" comment="just a comment">
    <platform name="platform1" url="http://" comment="">
        <account name="account2" display="">
            <pair name="k1"><![CDATA[v1]]></pair>
            <pair name="k4"><![CDATA[v4]]></pair>
            <pair name="k3"><![CDATA[v3]]></pair>
        </account>
        <account name="account3" comment="">
            <pair name="k1"><![CDATA[v1]]></pair>
            <pair name="k2"><![CDATA[v2]]></pair>
        </account>
    </platform>
    <platform name="platform2" url="http://" comment="">
        <account name="account5" comment="">
            <pair name="k7"><![CDATA[v7]]></pair>
            <pair name="k2"><![CDATA[v2]]></pair>
        </account>
        <account name="account6" comment="">
            <pair name="k1"><![CDATA[v1]]></pair>
        </account>
        <account name="account9" comment="">
            <pair name="k4"><![CDATA[v4]]></pair>
            <pair name="k5"><![CDATA[v5]]></pair>
            <pair name="k3"><![CDATA[v3]]></pair>
        </account>
        </platform>
    <platform name="platform4" url="http://" comment="">
        <account name="account5" comment=""/>
        <account name="account6" comment="">
            <pair name="k1"><![CDATA[v1]]></pair>
        </account>
        <account name="account9" comment="">
            <pair name="k8"><![CDATA[v8]]></pair>
            <pair name="k6"><![CDATA[v6]]></pair>
            <pair name="k9"><![CDATA[v9]]></pair>
        </account>
    </platform>
    <platform name="platform7" url="http://" comment=""/>
</credential>)";

	bnb::Credential xx;

	xx.FromXml(bnb::memory_type((const unsigned char*)strText, sizeof(strText)));

	// xx.SetUser(L"Frank");
	// xx.UpdateTime();

    auto f = [&xx](int n) {
        bnb::memory_type mt;

        std::cout << (xx.ToXml(mt) ? "true":"false")<< std::endl;
        std::cout << mt.c_str() << std::endl;
        std::cout << "----------------------------- " <<n<< std::endl << std::endl;
    };

    f(0);

    xx.Clear();
    f(1);
    std::cout<< (xx.IsEmpty() ? "true" : "false") <<std::endl;
    std::cout << xx.Size() << std::endl;
    f(2);
    xx.PreorderTraversal([](const bnb::platform_node& item) {});
    xx.PostorderTraversal([](const bnb::platform_node& item) {});
    xx.PreorderAction([](const bnb::platform_node& item) {return true;});
    xx.PostorderAction([](const bnb::platform_node& item) {return true;});
    // xx.bnb::list_type<bnb::platform_node>::Find(12);
    xx.Find(10);

    auto child = xx.PushBack({ L"heh" });
    xx.PushBack({ L"heh2" });
    xx.PushBack({ L"heh3" });
    xx.PushBack({ L"heh4" });
    f(3);
    xx.PushFront({ L"bnb" });
    xx.PushFront({ L"bnb2" });
    xx.PushFront({ L"bnb3" });
    xx.PushFront({ L"bnb4" });
    f(4);
    auto c2 = child->PushBack({ L"a1" });
    child->PushBack({ L"a2" });
    child->PushBack({ L"a3" });
    f(10);
    auto c3 = c2->PushBack({ L"a1" });
    c2->PushBack({ L"a2" });
    c2->PushBack({ L"a3" });
    f(11);
    xx.Move({ L"heh" }, 2);
    f(5);
    c2->Move({ L"a3" }, -1);
    f(12);
    xx.Sort();
    f(6);
    xx.Update({ L"heh" }, { L"fff" });
    f(7);
    xx.Remove({ L"bnb" });
    f(8);
    xx.RemoveIf([](const bnb::platform_type& item) {return true;});
    f(9);


	xx.Save("def.credential");
}


class TBase {
public:

	TBase() { fun(); }

	void fun() { std::cout << "I\'m base." << std::endl; }
};

class T1 : public TBase {
	void fun() { std::cout << "I\'m T1." << std::endl; }
};
class T2 : public TBase {
	void fun() { std::cout << "I\'m T2." << std::endl; }
};

void Print(void* ptr)
{
	std::cout << (ptr ? "yes" : "null") << std::endl;
}

void fun2()
{
	TBase* tb = new TBase();
	TBase* t1 = new T1();
	TBase* t2 = new T2();


}
