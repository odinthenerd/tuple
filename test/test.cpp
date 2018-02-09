#include <../src/tuple.hpp>
#include <cassert>
#include <string>


struct empty{
	constexpr empty(){}
};

template<typename T>
struct move_only{
	T data_;
    move_only(const move_only&);
    move_only& operator=(const move_only&);
public:
    move_only(T data) : data_(data) {}
    move_only(move_only&& x)
        : data_(x.data_) {x.data_ = T{};}
    move_only& operator=(move_only&& x)
        {data_ = x.data_; x.data_ = T{}; return *this;}

    int get() const {return data_;}

    bool operator==(const move_only& x) const {return data_ == x.data_;}
    bool operator< (const move_only& x) const {return data_ <  x.data_;}
};

int main(){
	auto t = kvasir::tuple<int,long,bool,char>{};
	int a = kvasir::get<0>(t);
	char b = kvasir::get<char>(t);
    {
        typedef kvasir::tuple<> T;
        T t0;
        T t1;
        t0.swap(t1);
    }
    {
        typedef kvasir::tuple<move_only<int>> T;
        T t0(move_only<int>(0));
        T t1(move_only<int>(1));
        t0.swap(t1);
        assert(kvasir::get<0>(t0) == 1);
        assert(kvasir::get<0>(t1) == 0);
    }
    {
        typedef kvasir::tuple<move_only<int>, move_only<int>> T;
        T t0(move_only<int>(0), move_only<int>(1));
        T t1(move_only<int>(2), move_only<int>(3));
        t0.swap(t1);
        assert(kvasir::get<0>(t0) == 2);
        assert(kvasir::get<1>(t0) == 3);
        assert(kvasir::get<0>(t1) == 0);
        assert(kvasir::get<1>(t1) == 1);
    }
    {
        typedef kvasir::tuple<move_only<int>, move_only<int>, move_only<int>> T;
        T t0(move_only<int>(0), move_only<int>(1), move_only<int>(2));
        T t1(move_only<int>(3), move_only<int>(4), move_only<int>(5));
        t0.swap(t1);
        assert(kvasir::get<0>(t0) == 3);
        assert(kvasir::get<1>(t0) == 4);
        assert(kvasir::get<2>(t0) == 5);
        assert(kvasir::get<0>(t1) == 0);
        assert(kvasir::get<1>(t1) == 1);
        assert(kvasir::get<2>(t1) == 2);
    }
	{
        typedef kvasir::tuple<double&, std::string, int> T;
        double d = 1.5;
        const T t(d, "high", 5);
        assert(kvasir::get<0>(t) == 1.5);
        assert(kvasir::get<1>(t) == "high");
        assert(kvasir::get<2>(t) == 5);
        kvasir::get<0>(t) = 2.5;
        assert(kvasir::get<0>(t) == 2.5);
        assert(kvasir::get<1>(t) == "high");
        assert(kvasir::get<2>(t) == 5);
        assert(d == 2.5);

//        kvasir::get<1>(t) = "four";
}
{
    {
        typedef kvasir::tuple<int> T;
        const T t(3);
        assert(kvasir::get<0>(t) == 3);
    }
    {
        typedef kvasir::tuple<std::string, int> T;
        const T t("high", 5);
        assert(kvasir::get<0>(t) == "high");
        assert(kvasir::get<1>(t) == 5);
    }
    {
        typedef kvasir::tuple<double, int> T;
        constexpr T t(2.718, 5);
        static_assert(kvasir::get<0>(t) == 2.718, "");
        static_assert(kvasir::get<1>(t) == 5, "");
    }
    {
        typedef kvasir::tuple<empty> T;
        constexpr T t{empty{}};
        constexpr empty e = kvasir::get<0>(t);
        ((void)e); // Prevent unused warning
    }
    {
        typedef kvasir::tuple<double&, std::string, int> T;
        double d = 1.5;
        const T t(d, "high", 5);
        assert(kvasir::get<0>(t) == 1.5);
        assert(kvasir::get<1>(t) == "high");
        assert(kvasir::get<2>(t) == 5);
        kvasir::get<0>(t) = 2.5;
        assert(kvasir::get<0>(t) == 2.5);
        assert(kvasir::get<1>(t) == "high");
        assert(kvasir::get<2>(t) == 5);
        assert(d == 2.5);
    }
}
	return 0;
}
