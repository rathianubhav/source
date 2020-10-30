#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {
    namespace value {
        enum type {
            int_t,
            short_t,
            long_t,
            double_t,
            char_t,
            float_t,
            string_t,
            null,
        };

        class obj {
            private:
                union data_t
                {
                    int int_val;
                    short short_val;
                    long long_val;
                    float float_val;
                    double double_val;
                } data;
                std::string s_val;
                value::type type;

            public:
                obj() : type(value::null) {}

                obj(value::type t)
                    : type(t) {}

                obj(int v) : type(value::int_t)
                    { data.int_val = v;}

                obj(short w) : type(value::short_t)
                    { data.short_val = w;}

                obj(long x) : type(value::long_t)
                    { data.long_val = x;}

                obj(float y) : type(value::float_t)
                    { data.float_val = y;}

                obj(double z) : type(value::double_t)
                    { data.double_val = z;}

                obj(std::string v)
                    : type(value::string_t)
                    { s_val = v;}

                int int_val() const {return data.int_val;}
                short short_val() const {return data.short_val;}
                long long_val() const {return data.long_val;}
                float float_val() const {return data.float_val;}
                double double_val() const {return data.double_val;}
                std::string str_val() const {return s_val;}
                value::type get_type() const {return type;}
                obj operator+(const obj& other)
                {
                    if (type == other.type)
                    {
                        switch (type) {
                            case value::int_t:
                                return obj(data.int_val + other.data.int_val);
                            case value::float_t:
                                return obj(data.float_val + other.data.float_val);
                            case value::short_t:
                                return obj(data.short_val + other.data.short_val);
                            case value::long_t:
                                return obj(data.long_val + other.data.long_val);
                            case value::double_t:
                                return obj(data.double_val + other.data.double_val);

                        }
                        throw std::runtime_error("invalid data");
                    }

                    throw std::runtime_error("failed to add diff. datatype");
                }

                obj operator-(const obj& other)
                {
                    if (type == other.type)
                    {
                        switch (type) {
                            case value::int_t:
                                return obj(data.int_val - other.data.int_val);
                            case value::float_t:
                                return obj(data.float_val - other.data.float_val);
                            case value::short_t:
                                return obj(data.short_val - other.data.short_val);
                            case value::long_t:
                                return obj(data.long_val - other.data.long_val);
                            case value::double_t:
                                return obj(data.double_val - other.data.double_val);
                        }
                        throw std::runtime_error("invalid data");
                    }

                    throw std::runtime_error("failed to sum diff. datatype");
                }

                obj operator*(const obj& other)
                {
                    if (type == other.type)
                    {
                        switch (type) {
                            case value::int_t:
                                return obj(data.int_val * other.data.int_val);
                            case value::float_t:
                                return obj(data.float_val * other.data.float_val);
                            case value::short_t:
                                return obj(data.short_val * other.data.short_val);
                            case value::long_t:
                                return obj(data.long_val * other.data.long_val);
                            case value::double_t:
                                return obj(data.double_val * other.data.double_val);
                        }
                        throw std::runtime_error("invalid data");
                    }

                    throw std::runtime_error("failed to sum diff. datatype");
                }

                obj operator/(const obj& other)
                {
                    if (type == other.type)
                    {
                        switch (type) {
                            case value::int_t:
                                return obj(data.int_val / other.data.int_val);
                            case value::float_t:
                                return obj(data.float_val / other.data.float_val);
                            case value::short_t:
                                return obj(data.short_val / other.data.short_val);
                            case value::long_t:
                                return obj(data.long_val / other.data.long_val);
                            case value::double_t:
                                return obj(data.double_val / other.data.double_val);
                        }
                        throw std::runtime_error("invalid data");
                    }

                    throw std::runtime_error("failed to sum diff. datatype");
                }

                friend ostream& operator<<(ostream& os, const obj&);
        };
        ostream& operator<<(ostream&, const obj&);
    }

}