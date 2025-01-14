//
// Created by csl on 9/12/22.
//

#ifndef SPP_TEST_CONFIG_HPP
#define SPP_TEST_CONFIG_HPP

#include "gtest/gtest.h"
#include "datetime.h"
#include "coordinate.h"

TEST(config, Config) {
    using namespace ns_spp;

    EXPECT_EQ(Config::Author::name, "unsigned-long2");
    EXPECT_EQ(Config::Author::eMail, "3079625093@qq.com");
    EXPECT_EQ(Config::Author::address, "WuHan China");

    EXPECT_DOUBLE_EQ(Config::Threshold::DOUBLE_EQ, 1E-15);
    EXPECT_DOUBLE_EQ(Config::Threshold::POSITION, 1E-8);
    EXPECT_DOUBLE_EQ(Config::Threshold::ITERATE, 1E-12);

    EXPECT_EQ(Config::TimeSystem::GPSTOrigin,
              Gregorian(1980, 1, 6, 0, 0, BigDouble("0")).toModJulianDay());

    EXPECT_EQ(Config::TimeSystem::BDTOrigin,
              Gregorian(2006, 1, 1, 0, 0, BigDouble("0")).toModJulianDay());

    EXPECT_EQ(Config::RefEllipsoid::WGS1984, RefEllipsoid(6378137.0, 1.0 / 298.257223563));
    EXPECT_EQ(Config::RefEllipsoid::CGCS2000, RefEllipsoid(6378137.0, 1.0 / 298.257222101));

}

#endif //SPP_TEST_CONFIG_HPP
