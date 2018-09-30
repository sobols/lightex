#pragma once

#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/spirit/home/support/char_encoding/unicode.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>

#include <string>

namespace lightex2 {

class String {
  public:
    using value_type = boost::spirit::char_encoding::unicode::char_type;

    String& operator=(const std::string& data) {
      data_ = data;
      return *this;
    }
    String& operator=(std::string&& data) {
      data_ = std::move(data);
      return *this;
    }

    const std::string& Get() const {
      return data_;
    }

    void PushBack(value_type codePoint) {
      using InsertIter = std::back_insert_iterator<std::string> ;
      InsertIter outIter(data_);
      boost::utf8_output_iterator<InsertIter> utf8Iter(outIter);
      *utf8Iter++ = codePoint;
    }

  private:
    std::string data_;
};

}  // namespace lightex2

namespace boost { namespace spirit {  namespace x3 { namespace traits {
  template<>
  struct push_back_container<lightex2::String, void> {
    template <typename T>
    static bool call(lightex2::String& c, T&& val) {
      c.PushBack(val);
      return true;
    }
  };
}}}} // namespace boost::spirit::x3::traits
