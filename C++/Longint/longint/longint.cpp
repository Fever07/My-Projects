#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

class Longint
{

	vector<int> num;
	bool sign;

	void refreshZeros() 
	{
		int i = num.size() - 1;
		while (num[i--] == 0 && num.size() > 1)
			num.pop_back();
	}

	static Longint changedSign(Longint temp)
	{
		Longint zero;
		if (temp == zero)
		{
			temp.sign = false;
			return temp;
		}
		else
		{
			temp.changeSign();
			return temp;
		}
	}

public:
	
	void prod(int t)
	{
		if (t == 0)
		{
			num.clear();
			num.push_back(0);
			sign = false;
		}
		else
		{
    		int temp = 0;
			for (int i = 0; i < num.size(); i++)
			{
		   		num[i] *= t;
	    		num[i] += temp;
    			temp = num[i] / 10;
				num[i] -= (temp*10);
    		}
			if (temp)
		   		num.push_back(temp);
		}
	}

	Longint()
	{
		num.push_back(0);
		sign = false;
	}

	Longint(int a)
	{
		sign = (a < 0);
		do
		{
			if (a >= 0)
				num.push_back(a % 10);
			else
				num.push_back((-a) % 10);
			a /= 10;
		}
		while (a);
	}

	Longint(string s)
	{
		int j = 0;
		if (s[0] == '-')
		{
			sign = true;
			j = 1;
		}
		else 
			sign = false;
		for (int i = s.size() - 1 ; i >= j; i--)
			num.push_back(s[i] - '0');
	}

	Longint(int val, int pwr)
	{
		Longint ans(1);
		for (int i = 0; i < pwr; i++)
		{
			Longint temp(val);
			ans *= temp;
		}
		num = ans.num;
	}

	void changeSign() 
	{
		sign = 1 - sign;
	}

	friend ostream& operator<<(ostream& os, const Longint& x)
	{
		if (x.sign)
			os << '-';
		for ( int i=x.num.size()-1; i >= 0; i--)
			os << x.num[i];
		return os;
	}

	Longint& operator= (const Longint& x)
	{
		num = x.num;
		sign = x.sign;
		return *this;
	}

	void set(int a)
	{
		num.clear();
		sign = (a < 0);
		do
		{
			if (a >= 0)
				num.push_back(a % 10);
			else
				num.push_back((-a) % 10);
			a /= 10;
		}
		while (a);
	}

	Longint operator+ (const Longint& x) const
	{
		Longint temp = *this;
		Longint tempx = x;
		int n = x.num.size();
		if (tempx.sign)
		{
			tempx.changeSign();
			return temp - tempx;
		} else
			if (temp.sign)
				return tempx + temp;
		if (n <= num.size()) 
		{
			for (int i = 0; i < n; i++) 
			{
				temp.num[i] += x.num[i];
				if (temp.num[i] >= 10)
				{
					if (i + 1 == num.size())
						temp.num.push_back(1);
					else
						temp.num[i + 1]++;
					temp.num[i] -= 10;
				}
			}
			temp.refreshZeros();
			return temp;
		}
		else
			return x + *this;
	}

	Longint operator- (const Longint& x) const
	{
		Longint temp = *this;
		Longint tempx = x;
		int n = x.num.size(), m = num.size();
		if (temp.sign)
		{
			temp.changeSign();
			return changedSign(temp + tempx);
		}
		else
			if (x.sign)
			{
				tempx.changeSign();
				return temp + tempx;
			}
		if (n <= m)
		{
			for (int i = 0; i < n; i++)
			{
				temp.num[i] -= tempx.num[i];
				if (temp.num[i] < 0)
				{
					if (i + 1 == num.size())
						return changedSign(tempx - temp);
					else
					{
						temp.num[i+1]--;
						temp.num[i] += 10;
					}
				}

			}
			temp.refreshZeros();
			return temp;
		}
		else
			return changedSign(tempx - temp);
	}
		
	bool operator>= (const Longint &x)
	{
		if (sign)
			if (x.sign)
				return changedSign(x) >= changedSign(*this);
			else
				return false;
		if (x.sign)
			return true;
		if (num.size() > x.num.size())
			return true;
		if (num.size() < x.num.size())
			return false;
		int i = num.size() - 1;
		while ( num[i] == x.num[i] ) 
		{
			i--;
			if ( i == -1 ) break;
		}
		if ( i == -1 )
			return true;
		if ( num[i] >= x.num[i] )
			return true;
		else 
			return false;
	}

	bool operator== (const Longint& x)
	{
		if ((*this).num.size() != x.num.size())
			return false;
		else
		{
			int i = 0;
			while (((*this).num[i] == x.num[i])&&(i < x.num.size()))
			{
				i++;
				if (i == x.num.size()) 
					break;
			}
			if ( i == x.num.size() )
				return true;
			else
				return false;
		}
	}

	void up()
	{
		Longint zero;
		if (!(*this == zero))
		{
    		num.push_back(0);
	    	for (int i = num.size() - 1; i > 0; i--)
			    num[i] = num[i-1];
		    num[0] = 0;
		}
	}

	Longint operator* (const Longint& x)
	{
		if (sign)
			return changedSign(changedSign(*this) * x);
		if (x.sign)
			return changedSign(*this * changedSign(x));
		vector<Longint> arr;
		for (int i = 0; i < x.num.size(); i++)
		{
			Longint p;
			p = *this;
			p.prod(x.num[i]);
			arr.push_back(p);
			for (int j = 0; j < i; j++)
				arr[i].up();
		};
		Longint ans;
		ans = accumulate(arr.begin(), arr.end(), ans);
		return ans;
	}

	Longint operator/ (const Longint& x)
	{
		if (sign)
			return changedSign(changedSign(*this) / x);
		if (x.sign)
			return changedSign(*this / changedSign(x));
		Longint ans;
		Longint temp;
		Longint dividend;
		dividend = *this;
		for (int i = dividend.num.size() - 1; i >= 0; i--)
		{
			temp.up();
			temp += dividend.num[i];
			int count = 0;
			while (temp >= x)
			{
				temp -= x;
				count++;
			}
			ans.up();
			Longint need(count);
			ans += need;
		}
		return ans;
	}

	void operator+= (const Longint& x)
	{
		*this = *this + x;
	}

	void operator-= (const Longint& x)
	{
		*this = *this - x;
	}

	void operator*= (const Longint& x)
	{
		*this = *this * x;
	}

	int mod_2()
	{
		return num[0] % 2;
	}

	int mod_3()
	{
		int res = 0;
		res = accumulate(num.begin(), num.end(), res);
		res %= 3;
		if (sign)
			if (res == 0)
				return res;
			else
				return 3 - res;
		else
			return res;
	}

	int mod_5()
	{
		return num[0] % 5;
	}

	int size()
	{
		return num.size();
	}
};



int main()
{
	string sx, sy;
	cout << "Enter x, y: " << endl;
	cin >> sx;
	cin >> sy;
	Longint x(sx), y(sy);
	cout << "x : " << x << endl;
	cout << "y : " << y << endl;
	cout << "Sum : " << endl;
	cout << x + y << endl;
	cout << "Difference : " << endl;
	cout << x - y << endl;
	cout << "Product : " << endl;
	cout << x * y << endl;
	cout << "Quotient : " << endl;
	cout << x / y << endl;
	cout << y / x << endl;
	cout << "x mod 2, 3, 5: " << endl;
	cout << x.mod_2() << " " << x.mod_3() << " " << x.mod_5() << " " << endl;
	cout << "y mod 2, 3, 5: " << endl;
	cout << y.mod_2() << " " << y.mod_3() << " " << y.mod_5() << " " << endl;
	return 0;
}