#define SPECS __attribute__ ((noinline))

namespace ton {

    // map stub implementation
    template<typename K, typename V>
    class map {

    private:

         V *v = (V*)0xdeadbeef;

    public:

        map() {}

        SPECS bool has_key(const K& k) {
            return true;
        }

        SPECS V &operator[](const K& k) {
            return *v;
        }

    };

}
