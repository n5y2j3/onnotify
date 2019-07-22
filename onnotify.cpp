/* 각 계정마다 누적 금액을 저장할 수 있도록
#include<eosio/eosio.hpp>
#include<eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}

[[eosio::on_notify("eosio.token::transfer")]]
void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
Receiver receiver(get_self(), get_self().value);
auto itr = receiver.find(to.value);
if(itr == receiver.end()) {
receiver.emplace(from, [&](auto& row) {
row.user = to;
row.balance = quantity;
});
} else {
receiver.modify(itr, from, [&](auto& row) {
row.balance += quantity;
});
}
} else {
Sender sender(get_self(), get_self().value);
auto itr = sender.find(from.value);
if(itr == sender.end()) {
sender.emplace(to, [&](auto& row) {
row.user = from;
row.balance = quantity;
});
} else {
sender.modify(itr, to, [&](auto& row) {
row.balance += quantity;
});
}
}
}
private:
TABLE trxlist {
name user;
asset balance;

uint64_t primary_key() const { return user.value; }
};

typedef multi_index<"eossender"_n, trxlist> Sender;
typedef multi_index<"eosreceiver"_n, trxlist> Receiver;
};
*/

/* 입금 내역 조회(in)
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}


[[eosio::on_notify("eosio.token::transfer")]]

void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
outs myTable(get_self(), get_self().value);
if(myTable.begin() == myTable.end()) {
myTable.emplace(from, [&](auto& row) {
row.balance = quantity;
});
} else {
auto itr = myTable.begin();
myTable.modify(itr, from, [&](auto& row) {
row.balance += quantity;
});
}
} 

if(to == get_self()) {
ins myTable(get_self(), get_self().value);
if(myTable.begin() == myTable.end()) {
myTable.emplace(to, [&](auto& row) {
row.balance = quantity;
});
} else {
auto itr = myTable.begin();
myTable.modify(itr, to, [&](auto& row) {
row.balance += quantity;
});
}
}
}

private:
TABLE outstruct {
asset balance;

uint64_t primary_key() const { return balance.symbol.code().raw(); }
};

typedef multi_index<"out"_n, outstruct> outs;
typedef multi_index<"in"_n, outstruct> ins;
};

*/

/* 출금 내역 조회(out)
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
    public:
    using contract::contract;

    ACTION dummy(){}
    
    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
            check(from == get_self(),"no out");

            outs myTable(get_self(), get_self().value);
            if(myTable.begin() == myTable.end()) {
                myTable.emplace(from, [&](auto& row) {
                    row.balance = quantity;
                });
            } else {
                auto itr = myTable.begin();
                
                myTable.modify(itr, from, [&](auto& row) {
                    row.balance += quantity;
                });
            }

    }
    private:
    TABLE outstruct { 
        asset balance;

        uint64_t primary_key() const { return balance.symbol.code().raw(); }
    };

    typedef multi_index<"out"_n, outstruct> outs;
};
*/