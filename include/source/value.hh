#pragma once

#include <bits/stdc++.h>

using namespace std;

namespace source {
    namespace value {
        enum type {
            int_t,
            null,
        };

        class obj {
            private:
                union data_t
                {
                    int int_val;
                } data;

                value::type type;

            public:
                obj() : type(value::null) {}

                obj(int v) : type(value::int_t)
                    { data.int_val = v;}

                int int_val() const {return data.int_val;}

                value::type get_type() const {return type;}
                obj operator+(const obj& other)
                {
                    if (type == other.type)
                    {
                        switch (type) {
                            case value::int_t:
                                return obj(data.int_val + other.data.int_val);
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