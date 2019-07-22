/* 각 계정마다 누적 금액을 저장할 수 있도록
/#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify: public contract {
    public:
    using contract::contract;

    ACTION dummy() {}

    [[eosio::on_notify("eosio.token::transfer")]]
        void ontransfer(name from, name to, asset quantity, std::string memo) {

        if(from == get_self()) {
            outs outerTable(get_self(), get_self().value);

            auto itr = outerTable.find(to.value);
            if(itr == outerTable.end()) {
                outerTable.emplace(to, [&](auto& row) {
                    row.outer = to;
                    row.outbalance = quantity;
                });
            } else {
                outerTable.modify(itr, from, [&](auto& row) {
                    row.outbalance += quantity;
                });
            }
        } else if(to == get_self()) {
            inputs inerTable(get_self(), get_self().value);

            auto itr = inerTable.find(to.value);
            if(itr== inerTable.end()) {
                inerTable.emplace(from, [&](auto& row) {
                    row.iner = to;
                    row.inbalance = quantity;
                });
            } else {
                auto itr = inerTable.begin();
                inerTable.modify(itr, from, [&](auto& row) {
                    row.iner = to;
                    row.inbalance += quantity;
                });
            }
        }
    }
    */

/* 

    private:
        TABLE outstruct {
            asset outbalance;
            name outer;

            uint64_t primary_key() const { return outer.value; }
        };

        TABLE instruct {
            asset inbalance;
            name iner;

            uint64_t primary_key() const { return iner.value; }
        };
    typedef multi_index<"outss"_n, outstruct> outs;
    typedef multi_index<"inss"_n, instruct> inputs;
};

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