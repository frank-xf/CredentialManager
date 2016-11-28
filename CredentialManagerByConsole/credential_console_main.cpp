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


int main()
{
	// fun1();

    TypeA a;
    TypeA b(10);
    TypeA c(10, 20);
    TypeA d(10, 20, 30);

    a.Show();
    b.Show();
    c.Show();
    d.Show();


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
            <property name="k1"><![CDATA[v1]]></property>
            <property name="k4"><![CDATA[v4]]></property>
            <property name="k3"><![CDATA[v3]]></property>
        </account>
        <account name="account3" comment="">
            <property name="k1"><![CDATA[v1]]></property>
            <property name="k2"><![CDATA[v2]]></property>
        </account>
    </platform>
    <platform name="platform2" url="http://" comment="">
        <account name="account5" comment="">
            <property name="k7"><![CDATA[v7]]></property>
            <property name="k2"><![CDATA[v2]]></property>
        </account>
        <account name="account6" comment="">
            <property name="k1"><![CDATA[v1]]></property>
        </account>
        <account name="account9" comment="">
            <property name="k4"><![CDATA[v4]]></property>
            <property name="k5"><![CDATA[v5]]></property>
            <property name="k3"><![CDATA[v3]]></property>
        </account>
        </platform>
    <platform name="platform4" url="http://" comment="">
        <account name="account5" comment=""/>
        <account name="account6" comment="">
            <property name="k1"><![CDATA[v1]]></property>
        </account>
        <account name="account9" comment="">
            <property name="k8"><![CDATA[v8]]></property>
            <property name="k6"><![CDATA[v6]]></property>
            <property name="k9"><![CDATA[v9]]></property>
        </account>
    </platform>
    <platform name="platform7" url="http://" comment=""/>
</credential>)";

	bnb::memory_type mt;

	bnb::Credential xx(L"123");

	xx.FromXml(bnb::memory_type((const unsigned char*)strText, sizeof(strText)));

	// xx.SetUser(L"Frank");
	xx.UpdateTime();
	std::cout << xx.ToXml(mt) << std::endl;

	std::cout << mt.c_str() << std::endl << "-----";

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
