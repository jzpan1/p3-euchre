#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <fstream>

using namespace std;

const int pack_size = 24;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST(test_pack_stream_ctor) {
    const int pack_size = 24;
    ifstream pack_file("pack.in");
    Pack pack(pack_file);
    Card current;
    
    for (int i = 0; i < pack_size; i++) {
        current = pack.deal_one();
        ASSERT_EQUAL(NINE + i % 6, current.get_rank());
        ASSERT_EQUAL(i/(pack_size / 4), current.get_suit());
    }
}

TEST(test_pack_reset) {
    Pack pack; 
    Card first = pack.deal_one();
    pack.reset();
    Card first2 = pack.deal_one();

    ASSERT_EQUAL(first, first2);
}

TEST(test_pack_empty) {
    Pack pack;
    Card current;
    
    
    for (int i = 0; i < pack_size -1; i++) {
        current = pack.deal_one();
        ASSERT_FALSE(pack.empty());
    }

    current = pack.deal_one();
    ASSERT_TRUE(pack.empty());
}

TEST(test_shuffle) {
    Pack pack;
    Card first = pack.deal_one();
    //Permutation in ( 0 1 3 7 15 6 13 2 5 11 23 22 20 16 8 17 10 21 18 12 )( 4 9 19 14 4 )
    pack.shuffle();
    Card third;
    pack.deal_one();
    pack.deal_one();
    third = pack.deal_one();

    ASSERT_EQUAL(third, first);
}

TEST_MAIN()
