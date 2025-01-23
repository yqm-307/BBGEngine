#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "util/other/Uuid.hpp"

BOOST_AUTO_TEST_SUITE(TestUUID)

BOOST_AUTO_TEST_CASE(t_uuid)
{
    util::other::Uuid uid1;
    util::other::Uuid uid2;

    BOOST_TEST_MESSAGE("uuid1: " << uid1.ToString());
    BOOST_TEST_MESSAGE("uuid2: " << uid2.ToString());
}

BOOST_AUTO_TEST_SUITE_END()