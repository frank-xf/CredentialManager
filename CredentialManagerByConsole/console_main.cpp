#include <iostream>
#include <string>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

int main()
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
<credential user="" time="1466185402">
 <platform name="platform1" url="http://" display="">
  <account name="account2" display="">
   <property name="k1"><![CDATA[v1]]></property>
   <property name="k4"><![CDATA[v4]]></property>
   <property name="k3"><![CDATA[v3]]></property>
  </account>
  <account name="account3" display="">
   <property name="k1"><![CDATA[v1]]></property>
   <property name="k2"><![CDATA[v2]]></property>
  </account>
 </platform>
 <platform name="platform2" url="http://" display="">
  <account name="account5" display="">
   <property name="k7"><![CDATA[v7]]></property>
   <property name="k2"><![CDATA[v2]]></property>
  </account>
  <account name="account6" display="">
   <property name="k1"><![CDATA[v1]]></property>
  </account>
  <account name="account9" display="">
   <property name="k4"><![CDATA[v4]]></property>
   <property name="k5"><![CDATA[v5]]></property>
   <property name="k3"><![CDATA[v3]]></property>
  </account>
 </platform>
</credential>)";

    bnb::memory_type mt;

    bnb::Credential xx("123");

    xx.FromXml(bnb::memory_type((const unsigned char*)strText, sizeof(strText)));

    xx.SetUser("Frank");
    xx.UpdateTime();
    std::cout << xx.ToXml(mt) << std::endl;

    std::cout << mt.c_str() << std::endl << "-----";

    return (0);
}