//
// Created by csl on 9/12/22.
//

#ifndef SPP_DATETIME_H
#define SPP_DATETIME_H

#include <ostream>
#include "config.h"

namespace ns_spp {
    struct Gregorian;
    struct Julian;
    struct JulianDay;
    struct ModJulianDay;
    struct GPSTime;
    struct BDTime;


    struct Gregorian {
        unsigned short year;
        unsigned short month;
        unsigned short day;
        unsigned short hour;
        unsigned short minute;
        BigDouble second;

        Gregorian(unsigned short year, unsigned short month, unsigned short day,
                  unsigned short hour = 0, unsigned short minute = 0,
                  const std::string &sec = "0.0");

        Gregorian(unsigned short year, unsigned short month, unsigned short day,
                  unsigned short hour, unsigned short minute,
                  BigDouble sec);

        Gregorian();

        [[nodiscard]] JulianDay toJulianDay() const;

        [[nodiscard]] ModJulianDay toModJulianDay() const;

        [[nodiscard]] GPSTime toGPSTime() const;

        [[nodiscard]] BDTime toBDTime() const;

        [[nodiscard]] unsigned short dayOfYear() const;

    public:
        friend std::ostream &operator<<(std::ostream &os, const Gregorian &dateTime) {
            os << "Gregorian['y': " << dateTime.year << ", 'mon': " << dateTime.month << ", 'd': " << dateTime.day
               << ", 'h': " << dateTime.hour << ", 'min': " << dateTime.minute << ", 's': " << dateTime.second << ']';
            return os;
        }

        bool operator==(const ns_spp::Gregorian &rhs) const {
            return year == rhs.year &&
                   month == rhs.month &&
                   day == rhs.day &&
                   hour == rhs.hour &&
                   minute == rhs.minute &&
                   std::abs(static_cast<long double>(second - rhs.second)) < Config::Threshold::DOUBLE_EQ;
        }

        bool operator!=(const ns_spp::Gregorian &rhs) const {
            return !(rhs == *this);
        }

    };

    struct Julian {
    public:
        BigDouble days;

    protected:

        explicit Julian(const std::string &days);

        explicit Julian(BigDouble days);

        Julian() = default;

    public:

        bool operator==(const Julian &rhs) const {
            return std::abs(static_cast<long double>(days - rhs.days)) < Config::Threshold::DOUBLE_EQ;
        }

        bool operator!=(const Julian &rhs) const {
            return !(rhs == *this);
        }

        [[nodiscard]] virtual Gregorian toGregorian() const = 0;
    };

    struct JulianDay : public Julian {

    public:

        explicit JulianDay(const std::string &days = "0.0");

        explicit JulianDay(const BigDouble &days);

        JulianDay();

        [[nodiscard]] Gregorian toGregorian() const override;

        [[nodiscard]] ModJulianDay toModJulianDay() const;

        [[nodiscard]] GPSTime toGPSTime() const;

        [[nodiscard]] BDTime toBDTime() const;

        friend std::ostream &operator<<(std::ostream &os, const JulianDay &day) {
            os << "JulianDay['d': " << day.days << "]";
            return os;
        }
    };

    struct ModJulianDay : public Julian {

    public:

        explicit ModJulianDay(const std::string &days = "0.0");

        explicit ModJulianDay(const BigDouble &days);

        ModJulianDay();

        [[nodiscard]] Gregorian toGregorian() const override;

        [[nodiscard]] JulianDay toJulianDay() const;

        [[nodiscard]] GPSTime toGPSTime() const;

        [[nodiscard]] BDTime toBDTime() const;

        friend std::ostream &operator<<(std::ostream &os, const ModJulianDay &day) {
            os << "ModJulianDay['d': " << day.days << "]";
            return os;
        }
    };

    struct NavTime {
    public:
        unsigned int week;
        BigDouble secOfWeek;

    protected:
        explicit NavTime(unsigned int week = 0, const std::string &secOfWeek = "0.0");

        explicit NavTime(unsigned int week, BigDouble secOfWeek);

    public:
        [[nodiscard]] virtual ModJulianDay toModJulianDay() const = 0;

        [[nodiscard]] virtual Gregorian toGregorian() const = 0;

        [[nodiscard]] virtual JulianDay toJulianDay() const = 0;
    };

    struct GPSTime : public NavTime {
    public:

        explicit GPSTime(unsigned int week = 0, const std::string &secOfWeek = "0.0");

        explicit GPSTime(unsigned int week, const BigDouble &secOfWeek);

        [[nodiscard]] ModJulianDay toModJulianDay() const override;

        [[nodiscard]] Gregorian toGregorian() const override;

        [[nodiscard]] JulianDay toJulianDay() const override;

        friend std::ostream &operator<<(std::ostream &os, const GPSTime &gpsTime) {
            os << "GPSTime['w': " << gpsTime.week << ", 'sow': " << gpsTime.secOfWeek << ']';
            return os;
        }

        bool operator==(const ns_spp::GPSTime &rhs) const {
            return week == rhs.week &&
                   std::abs(static_cast<long double>(secOfWeek - rhs.secOfWeek)) < Config::Threshold::DOUBLE_EQ;
        }

        bool operator!=(const ns_spp::GPSTime &rhs) const {
            return !(rhs == *this);
        }

    };

    struct BDTime : public NavTime {
    public:

        explicit BDTime(unsigned int week = 0, const std::string &secOfWeek = "0.0");

        explicit BDTime(unsigned int week, const BigDouble &secOfWeek);

        [[nodiscard]] ModJulianDay toModJulianDay() const override;

        [[nodiscard]] Gregorian toGregorian() const override;

        [[nodiscard]] JulianDay toJulianDay() const override;

        friend std::ostream &operator<<(std::ostream &os, const BDTime &bdTime) {
            os << "BDTime['w': " << bdTime.week << ", 'sow': " << bdTime.secOfWeek << ']';
            return os;
        }

        bool operator==(const ns_spp::BDTime &rhs) const {
            return week == rhs.week &&
                   std::abs(static_cast<long double>(secOfWeek - rhs.secOfWeek)) < Config::Threshold::DOUBLE_EQ;
        }

        bool operator!=(const ns_spp::BDTime &rhs) const {
            return !(rhs == *this);
        }

    };

}

#endif //SPP_DATETIME_H
