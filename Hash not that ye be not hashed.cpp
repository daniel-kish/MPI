#include <iostream>
#include <unordered_set>
#include <array>
#include <random>
#include <numeric>

using field = std::array< std::array<int,3>,3 >;
void print(int n)
{ 
    std::cout << n << ' ';
}

template <class T, std::size_t N>
void print(std::array<T,N> const& f)
{
    for(const auto& e : f){
        print(e);
    }
    std::cout << '\n';
}

namespace std
{
	template<> struct hash<field>
	{
		typedef field argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const
		{
			std::string s1;
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					s1.push_back(char( s[i][j] ));

			return std::hash<std::string>()(s1);
		}
	};
}
 
field rand_field()
{
	field f;
	static std::mt19937 g;
	
	std::vector<int> v(9);
	std::iota(begin(v), end(v), 1);
	std::shuffle(begin(v), end(v), g);
	
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			f[i][j] = v[i * 3 + j];
		}
	return f;
}

int main()
{  
    std::unordered_set<field> c;

	for (int i = 0; i < 10; ++i)
		c.insert(rand_field());

	for (const auto& e : c)
	{
		print(e);
		std::cout << std::hash<field>{}(e) << '\n';
		std::cout << '\n';
	}
	
    auto search = c.find(*c.begin());
    if(search != c.end()) {
        std::cout << "Found\n";
		print(*search);
    }
    else {
        std::cout << "Not found\n";
    }
    
}