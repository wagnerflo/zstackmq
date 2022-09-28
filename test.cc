#include <stdint.h>
#include <list>
#include <bit>
#include <functional>
#include <iostream>
#include <variant>

void to_little_endian() {
  if constexpr (std::endian::native == std::endian::big) {
    // return ...;
  }
  else {
    // return ...;
  }
}

template <typename R>
class Type {
  public:
    R& ref;
    Type(R& r) : ref(r) { /* empty */ }
};

template <unsigned short L, typename R>
class FixedLengthType : public Type<R> {
  public:
    const unsigned short len = L;
    using Type<R>::Type;
};

template <unsigned short P, typename R>
class PrefixedLengthType : public Type<R> {
  public:
    const unsigned short prefix_len = P;
    using Type<R>::Type;
};

class  UINT8 : public FixedLengthType< 8,  uint8_t> { /* empty */ };
class UINT16 : public FixedLengthType<16, uint16_t> { /* empty */ };
class UINT32 : public FixedLengthType<32, uint32_t> { /* empty */ };
// class IEEEADDR : ...
class   BUFFER8 :  public FixedLengthType<  8, std::string> { /* empty */ };
class  BUFFER16 :  public FixedLengthType< 16, std::string> { /* empty */ };
class  BUFFER18 :  public FixedLengthType< 18, std::string> { /* empty */ };
class  BUFFER32 :  public FixedLengthType< 32, std::string> { /* empty */ };
class  BUFFER42 :  public FixedLengthType< 42, std::string> { /* empty */ };
class BUFFER100 :  public FixedLengthType<100, std::string> { /* empty */ };

class  BUFFER_UINT8 : public PrefixedLengthType < 8, std::string> { /* empty */ };
class BUFFER_UINT16 : public PrefixedLengthType <16, std::string> { /* empty */ };
class BUFFER_UINT32 : public PrefixedLengthType <32, std::string> { /* empty */ };

typedef
  std::list<
    std::variant<
      UINT8,
      UINT16,
      UINT32,
      BUFFER8,
      BUFFER16,
      BUFFER18,
      BUFFER32,
      BUFFER42,
      BUFFER100,
      BUFFER_UINT8,
      BUFFER_UINT16,
      BUFFER_UINT32
    >
  >
  bindlist;

class Message {
  public:
    void serialize() {
      for (auto i : bind_payload()) {
        std::visit([](auto&& arg) {
          std::cout << static_cast<void*>(&arg.ref) << std::endl;
        }, i);
      }
    };

  protected:
    virtual const bindlist bind_payload() = 0;
};

class A : public Message {
  public:
    uint8_t abc = 42;
    uint16_t def = 82;
    std::string ghi;

  protected:
    const bindlist bind_payload() {
      return {
        UINT8(abc),
        UINT16(def),
        BUFFER_UINT8(ghi)
      };
    }
};

// uint8_t  -> as little endian
// uint16_t -> as little endian
// uint32_t -> as little endian
// IEEEADDR -> ???
// std::string


int main() {
  A a;
  std::cout << static_cast<void*>(&a.abc) << std::endl;
  std::cout << static_cast<void*>(&a.def) << std::endl;
  a.serialize();

  return 0;
}
